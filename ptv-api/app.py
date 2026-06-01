from fastapi import FastAPI, Query
import httpx
from datetime import datetime
from zoneinfo import ZoneInfo
from zoneinfo import available_timezones

app = FastAPI()

MELBOURNE_TZ = ZoneInfo("Australia/Melbourne")


def unix_to_local_time(unix_time: int | None) -> str | None:
    if unix_time is None:
        return None

    return datetime.fromtimestamp(unix_time, MELBOURNE_TZ).strftime("%H:%M")


def minutes_until(unix_time: int | None, now_ts: int | None = None) -> int | None:
    if unix_time is None:
        return None

    if now_ts is None:
        now_ts = int(datetime.now(MELBOURNE_TZ).timestamp())

    return round((unix_time - now_ts) / 60)

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.get("/departures")
async def departures(
    stop_id: str = Query(..., example="G1005"),
    platform: str = Query("P1"),
    limit: int = Query(3, ge=1, le=25),
    mode: str = Query("metrotrains"),
):
    url = (
        f"https://anytrip.com.au/api/v3/region/au3/"
        f"departures/au3:{stop_id}-{platform}"
    )

    params = {
        "limit": limit,
        "modes": f"au3:{mode}",
        "offset": 0,
        "useRedis": "true",
    }

    async with httpx.AsyncClient() as client:
        response = await client.get(url, params=params)
        print(f"Request URL: {response.request.url}")
        response.raise_for_status()

    data = response.json()

    raw_departures = data.get("response", {}).get("departures", [])
    raw_stop = data.get("response", {}).get("stop", {})

    simplified = []

    for item in raw_departures:
        trip = item.get("tripInstance", {}).get("trip", {})
        stop_time = item.get("stopTimeInstance", {})
        vehicle = item.get("vehicle", {})


        departure = stop_time.get("departure", {})
        arrival = stop_time.get("arrival", {})

        departure_ts = departure.get("time")
        arrival_ts = arrival.get("time")

        simplified.append({

            "dest": trip.get("headsign", {}).get("headline"),
            # "subtitle": trip.get("headsign", {}).get("subtitle"),
            "time": unix_to_local_time(departure_ts),
            "delay": departure.get("delay"),
            "remain": minutes_until(departure_ts),

        })

    return {
        "stop": {
            "id": raw_stop.get("id", None),
            "name": raw_stop.get("name", {}).get("station_name", None),
            "platform": raw_stop.get("name", {}).get("platform_name", None),
        },
        "departures": simplified,
    }