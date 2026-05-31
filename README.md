# Melbourne Train Departures API

A lightweight FastAPI service that provides simplified public transport departure information for Melbourne train stations.

The API acts as a wrapper around the AnyTrip departures endpoint, extracting only the information needed for downstream applications. In this project, we use the ESP32 display.

## Features

* Simple REST API
* Supports any stop on the PTV network
* Returns upcoming departures in a compact JSON format
* Converts departure times into local Melbourne time
* Provides:

  * Destination
  * Scheduled departure time
  * Estimated departure time
  * Minutes until departure
  * Delay information
  * Platform
  * Route and trip identifiers

## Example

Request departures for Flinders Street Station Platform 1:

```http
GET /departures?stop_id=1071&platform=1
```

Response:

```json
{
  "stop": {
    "id": "au3:G1071-P1",
    "name": "Flinders Street",
    "platform": "1"
  },
  "departures": [
    {
      "dest": "Frankston",
      "time": "15:04",
      "delay": 120,
      "remain": 6
    },
    {
      "dest": "Mordialloc",
      "time": "15:12",
      "delay": null,
      "remain": 14
    },
    {
      "dest": "Cheltenham",
      "time": "15:18",
      "delay": 60,
      "remain": 20
    }
  ]
}
```

### Field Descriptions

#### `stop`

Information about the requested station platform.

| Field      | Description                |
| ---------- | -------------------------- |
| `id`       | Unique platform identifier |
| `name`     | Station name               |
| `platform` | Platform number            |

#### `departures`

List of upcoming departures from the specified platform.

| Field    | Type           | Description                                                       |
| -------- | -------------- | ----------------------------------------------------------------- |
| `dest`   | string         | Destination of the service                                        |
| `time`   | string         | Departure time in Melbourne local time (`HH:MM`)                  |
| `delay`  | integer | null | Delay in seconds. `null` if live delay information is unavailable |
| `remain` | integer        | Minutes remaining until departure                                 |


## API Endpoint

### Get Departures

```http
GET /departures
```

Query parameters:

| Parameter | Description                    |
| --------- | ------------------------------ |
| stop_id   | PTV stop identifier            |
| platform  | Platform number                |
| limit     | Number of departures to return |

Example:

```http
GET /departures?stop_id=1071&platform=1&limit=5
```

## Development

Install dependencies:

```bash
pip install -r requirements.txt
```

Run locally:

```bash
uvicorn app:app --reload
```

Open:

```text
http://localhost:8000/docs
```

to access the automatically generated FastAPI documentation.

## Live Example
```
https://esp32-ptv-display-um8x.vercel.app/departures?stop_id=G1071&platform=P1&limit=3
```

