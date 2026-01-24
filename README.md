# Pingling

<img src="https://github.com/easai/Pingling/blob/main/IMG_2814.jpeg" width="300" alt="Pingling" />

Pingling is a compact ESP32‑C3 network monitor that displays real‑time reachability and local time. 

It retrieves accurate time through NTP, applies timezone rules with `tzset()`, and performs periodic checks against a configured host or URL. The OLED display shows the current date, time, timezone, target address, and UP or DOWN status.

## Features

• WiFi connection and NTP time synchronization  
• Correct EST and EDT handling using `tzset()`  
• Reachability checks using HTTP or ICMP  
• SSD1306 OLED output over I2C  
• Continuous status updates at a fixed interval  

## Hardware

<img src="https://github.com/easai/Pingling/blob/main/IMG_2811.jpeg" width="300" alt="Pingling" />

• ESP32‑C3 Super Mini  
• SSD1306 OLED (128x64)  
• I2C wiring  

## Pin Configuration

The project uses the ESP32‑C3 Super Mini default I2C pins:

| Signal | ESP32‑C3 Pin | Description |
|--------|--------------|-------------|
| SDA    | GPIO 8       | I2C data    |
| SCL    | GPIO 9       | I2C clock   |
| VCC    | 3.3V         | OLED power  |
| GND    | GND          | Ground      |

The SSD1306 address is typically `0x3C`.

## Setup Requirements

• WiFi SSID and password  
• Target URL or host for reachability checks  
• Timezone string for correct local time  
• Update interval for display refresh  

Create a `config.h` file as follows. Place the `config.h` file under the src directory.
```c
#ifndef CONFIG_H
#define CONFIG_H

#define SECRET_SSID <your wifi ssid>
#define SECRET_PASS <your wifi password>
#define SITE_URL <your site's url>

#endif
```


