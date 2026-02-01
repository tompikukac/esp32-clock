import os
import time
import whisper
import requests

from dotenv import load_dotenv

dotenv_path = "../../Docker/.env"
# file_path = "./var/lib/graphite/whisper/tompi/test.wsp"
file_path = "./var/lib/graphite/whisper/tompi/home/kert/humidity.wsp"
# file_path = "./var/lib/graphite/whisper/tompi/home/kert/temperature.wsp"
root_dir = "./var/lib/graphite/whisper/tompi/home/"



def load_config():
    load_dotenv(dotenv_path)
    config = {
        "INFLUXDB_TOKEN": os.getenv("INFLUXDB_TOKEN"),
        # "INFLUX_ORG": os.getenv("INFLUX_ORG"),
        # "INFLUX_BUCKET": os.getenv("INFLUX_BUCKET"),
        # "INFLUX_URL": os.getenv("INFLUX_URL")
    }
    return config
def walk_devices(root_dir):
    devices = {}

    for entry in os.scandir(root_dir):
        if not entry.is_dir():
            continue

        name = entry.name
        temp = None
        hum = None

        for f in os.scandir(entry.path):
            if not f.is_file():
                continue
            if f.name == "temperature.wsp":
                temp = f.path
            elif f.name == "humidity.wsp":
                hum = f.path

        if temp or hum:
            devices[name] = {
                "temperature": temp,
                "humidity": hum
            }

    return devices

def read_whisper_archive(file_path):
    info = whisper.info(file_path)

    # select lowest resolution (largest retention)
    archive = max(info["archives"], key=lambda a: a["secondsPerPoint"])
    step = archive["secondsPerPoint"]
    retention = archive["retention"]

    end = int(time.time())
    start = end - retention

    points = whisper.fetch(file_path, start, end)[1]

    data = {}
    for i, value in enumerate(points):
        if value is None:
            continue
        ts = start + i * step
        data[ts] = value

    return data

def merge_temp_humidity(temp_path, hum_path, name):
    temp_data = read_whisper_archive(temp_path) if temp_path else {}
    hum_data  = read_whisper_archive(hum_path)  if hum_path else {}

    timestamps = sorted(set(temp_data) | set(hum_data))
    lines = []

    for ts in timestamps:
        fields = []

        if ts in temp_data:
            fields.append(f"temperature={temp_data[ts]}")
        if ts in hum_data:
            fields.append(f"humidity={hum_data[ts]}")

        if not fields:
            continue

        line = f"environment,name={name} " + ",".join(fields) + f" {ts}"
        lines.append(line)

    return lines



def write_to_influx(lines, url, org, bucket, token, batch_size=100):
    headers = {
        "Authorization": f"Token {token}",
        "Content-Type": "text/plain; charset=utf-8"
    }

    write_url = (
        f"{url}/api/v2/write"
        f"?org={org}&bucket={bucket}&precision=s"
    )
    total_batches = (len(lines) + batch_size - 1) // batch_size
    print(f"Total batches: {total_batches}")

    for i in range(0, len(lines), batch_size):
        batch = "\n".join(lines[i:i + batch_size])
        print(f"Writing batch {i // batch_size + 1}/{total_batches} with {batch.count('\n') + 1} points")
        print(f"POST URL: {write_url}")
        time.sleep(0.05)    
        r = requests.post(write_url, headers=headers, data=batch)
        print(f"[DEBUG] Influx response: {r.status_code} {r.text}")
        if r.status_code != 204:
            raise RuntimeError(
                f"Influx write failed: {r.status_code} {r.text}"
            )
        print(f"[OK] wrote batch, lines={batch.count('\n') + 1}")

# def whisper_to_lineprotocol(file_path):
#     metric = os.path.splitext(os.path.basename(file_path))[0]
#     info = whisper.info(file_path)
#     lines = []

#     now = int(time.time())
#     for archive in info['archives']:
#         retention = archive['retention']
#         seconds_per_point = archive['secondsPerPoint']
#         start = now - retention
#         end = now

#         ret = whisper.fetch(file_path, start, end)
#         points = ret[1]

#         for i, value in enumerate(points):
#             if value is None:
#                 continue
#             timestamp = start + i * seconds_per_point
#             line = f'{metric} value={value} {timestamp}'
#             lines.append(line)

#     return lines


config = load_config()
# print("Loaded configuration:")
# print(config["INFLUXDB_TOKEN"])

# info = whisper.info(file_path)
# print(info)

# lines = whisper_to_lineprotocol(file_path)
# for line in lines[:10]:
#     print(line)

all_lines = []
devices = walk_devices(root_dir)
# for name, files in devices.items():
#     print(name, files)


for name, files in devices.items():
    lines = merge_temp_humidity(
        temp_path=files.get("temperature"),
        hum_path=files.get("humidity"),
        name=name
    )
    all_lines.extend(lines)

# for line in all_lines[:10]:
#     print(line)


write_to_influx(
    lines=all_lines,
    url="http://192.168.1.111:8086",
    org="szlab",
    bucket="old",
    token=config["INFLUXDB_TOKEN"]
)