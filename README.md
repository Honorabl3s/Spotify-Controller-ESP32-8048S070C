# 🎵 S-CLASS Smart Music Hub (ESP32 Spotify Controller)

![ESP32](https://img.shields.io/badge/ESP32-S3-blue?style=flat-square&logo=espressif)
![LVGL](https://img.shields.io/badge/LVGL-v8.x-green?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

![S-CLASS UI Preview](https://github.com/Honorabl3s/Spotify-Controller-ESP32-8048S070C/blob/main/Real%20Life%20UI.png)

Highly responsive Spotify controller built on the ESP32-8048S070C (7-inch capacitive touch display). Designed to bridge the gap between robust engineering and digital arts, this project brings a commercial-grade UI/UX to your DIY setup—perfect for studio monitors, DJ decks, or a modern desktop companion.

## ✨ Key Features
* **Highly Responsive UI:** Play/Pause UI elements react instantly to touch, cleverly masking API network delays to provide a seamless user experience.
* **Bilinear Interpolation Engine:** Bypasses standard jagged scaling by mathematically rendering Spotify's 300x300 API images into a pixel-perfect, mathematically cropped circular vinyl cover.
* **Interactive Arc Seek:** Drag the circular progress bar to seek through the track with real-time `MM:SS` feedback.
* **Dynamic Auto-Scroll:** Handles long track titles and multi-artist collaborations with a smooth, constrained horizontal marquee.

## 🛠️ Hardware Requirements
* ESP32-S3 RGB Display 7" (ESP32-8048S070C)
* Micro-USB / Type-C for Power & Programming

## 🚀 Quick Start (Open Source Version)
This repository contains the core `main.cpp` and `platformio.ini` configuration. 
1. Clone this repository.
2. Insert your WiFi credentials and Spotify API keys (`client_id`, `client_secret`, `refresh_token`) in `main.cpp`.
3. Build and upload using PlatformIO.

*(Note: The open-source version requires you to build and configure the GUI widgets manually using your own LVGL setup).*

---

## 💎 Get the Premium Package (Plug & Play)
Don't want to spend hours designing the UI, struggling with PlatformIO library configurations, or figuring out how to fetch the Spotify API Token? This premium package is your ultimate shortcut!

![Premium Package Teaser](https://github.com/Honorabl3s/Spotify-Controller-ESP32-8048S070C/blob/main/ImgCover.png)

Spotify-Controller is an interactive Spotify controller specifically designed for the ESP32-8048S070C. It is perfect for enhancing your desk setup, studio monitors, or even serving as an eye-catching addition to your personal workspace.

👉 **[Support me on Trakteer & Download the Full Project here](LINK_TRAKTEER_KAMU_DI_SINI)**

**📦 What will you get inside this ZIP file?**
* 📁 **Spotify_ControllerV2:** The raw, 100% editable UI project file (`.spj`). You are free to change colors, layouts, or add your own music group's logo.
* 📁 **Spotify-Controller-ESP32-8048S070C-main:** A complete, ready-to-use C++ project. All libraries (LVGL, LovyanGFX, ArduinoJson) are fully configured. Just insert your WiFi & Spotify Token, then click Upload.
* 📄 **Spotify_Controller_ESP32_8048S070C_Setup_Guide.pdf:** A highly beginner-friendly, step-by-step guide, including a secret trick to get the Spotify API Refresh Token without needing to code a web server at all!

Your support means a lot in helping the development of future open-source projects and IoT innovations. Happy building! 🚀

---

## 👨‍💻 Author
Built with passion by **Valentino**. 
An Electrical Engineering project exploring the intersection of IoT, Audio Tech, and Visual Interface Design.