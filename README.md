# Melbourne Public Transport Departure Display

A complete end-to-end public transport departure display system built using an ESP32 microcontroller, a custom backend API, and real-time Melbourne public transport data.

The project consists of two main components:

1. **Backend API** – fetches and simplifies transport data into a compact JSON format suitable for embedded devices.
2. **ESP32 OLED Display** – connects to Wi-Fi, requests departure data from the API, and displays upcoming departures on a small OLED screen.


## System Architecture

```text
┌──────────────────────┐
│ AnyTrip / PTV Data   │
└──────────┬───────────┘
           │ JSON
           ▼
┌──────────────────────┐
│ Backend API          │
│ (FastAPI / Vercel)   │
└──────────┬───────────┘
           │ JSON
           ▼
┌──────────────────────┐
│ ESP32                │
│ WiFi + HTTP Client   │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ SSD1306 OLED Display │
└──────────────────────┘
```

---

## Repository Structure

```text
.
├── ptv-api/
│   ├── README.md
│   └── display.ino
│
├── esp32/
│   ├── README.md
│   └── app.py
│
└── README.md
```

## Components

### Backend API

Located in:

```text
api/
```

The API is responsible for:

* Retrieving public transport departure data
* Normalising and simplifying responses
* Converting timestamps into local Melbourne time
* Returning compact JSON suitable for low-power devices
* Reducing the amount of processing required on the ESP32

See:

```text
api/README.md
```

for setup, deployment and API documentation.


### ESP32 OLED Display

Located in:

```text
esp32/
```

The ESP32 firmware is responsible for:

* Connecting to Wi-Fi
* Calling the backend API
* Parsing JSON responses
* Displaying departures on an SSD1306 OLED display
* Refreshing data periodically

See:

```text
esp32/README.md
```

for hardware setup, wiring diagrams, Arduino IDE installation and firmware instructions.


## Data
* Melbourne Public Transport Data
* AnyTrip-derived departure feeds


## Future Improvements

* Multiple station support
* Platform selection buttons
* Historical departure statistics
* Arrival predictions
* Battery-powered portable unit
* GTFS-Realtime integration
* 3D printed enclosure

---
