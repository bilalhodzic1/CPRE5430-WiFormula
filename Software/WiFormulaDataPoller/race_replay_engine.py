import json
from datetime import datetime
import time
import asyncio
import websockets

with open("position.json", "r") as file:
    position_data = json.load(file)

with open("drivers.json", "r") as driver_file:
    driver_data = json.load(driver_file)

with open("flaginfo.json", "r") as flag_file:
    flag_data = json.load(flag_file)

for item in position_data:
    item['date'] = datetime.fromisoformat(item['date'])

for item in flag_data:
    item['date'] = datetime.fromisoformat(item['date'])

position_data = sorted(position_data, key=lambda x: x['date'])

flag_data = sorted(flag_data, key=lambda x: x['date'])

allowed_flags = {"GREEN", "DOUBLE YELLOW", "YELLOW", "CLEAR", "CHEQUERED"}

race_position_changes = position_data[20:]

first_position_time = min(event["date"] for event in race_position_changes)

filtered_flags = [
    flag for flag in flag_data
    if flag["date"] >= first_position_time
]

FLAG_TO_NUMBER = {
    "GREEN": 1,
    "RED" : 2,
    "YELLOW": 3,
    "DOUBLE YELLOW": 3,
    "CLEAR": 1,
    "CHEQUERED" : 4
}

allowed_flag_data = [item for item in filtered_flags if item['flag'] in allowed_flags]

hadjars_race = [item for item in race_position_changes if item['driver_number'] == 6]

all_events = hadjars_race + allowed_flag_data
all_events.sort(key=lambda e: e["date"])

async def connect_to_websocket(uri):
    async with websockets.connect(uri) as ws:
        race_start_time = all_events[0]["date"]   # first recorded instance
        sim_start_real = time.monotonic()

        for event in all_events:
            event["offset"] = (event["date"] - race_start_time).total_seconds()

        current_index = 0

        while current_index < len(all_events):
            elapsed = time.monotonic() - sim_start_real

            while (
                current_index < len(all_events)
                and all_events[current_index]["offset"] <= elapsed
            ):
                event = all_events[current_index]

                # Position change
                if "position" in event:
                    print(
                        f"[{event['date'].time()}] "
                        f"Driver {event['driver_number']} -> P{event['position']}"
                    )

                # Flag event
                elif "flag" in event:
                    print("Flag event")
                    flag_json = {
                        "flag_value" : FLAG_TO_NUMBER.get(event['flag'], -1)
                    }
                    await ws.send(json.dumps(flag_json))

                current_index += 1

            time.sleep(0.05)

if __name__ == "__main__":
    websocket_uri = "ws://207.211.177.254:8080/formula-data-stream?device=PYTHON&mac=NOMAC"
    asyncio.run(connect_to_websocket(websocket_uri))