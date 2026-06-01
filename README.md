# 🎵 S-CLASS Smart Music Hub (ESP32 Spotify Controller)

![ESP32](https://img.shields.io/badge/ESP32-S3-blue?style=flat-square&logo=espressif)
![LVGL](https://img.shields.io/badge/LVGL-v8.x-green?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

S-CLASS Smart Music Hub is a premium, highly responsive Spotify controller built on the ESP32-8048S070C (7-inch capacitive touch display). Designed to bridge the gap between robust engineering and digital arts, this project brings a commercial-grade UI/UX to your DIY setup—perfect for studio monitors, DJ decks, or a modern desktop companion.

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
Don't want to spend hours designing the UI, configuring SquareLine Studio, or dealing with API token generation? 

I have prepared a **Complete Premium Package** on my Trakteer!
👉 **[Support me on Trakteer & Download the Full Project here](LINK_TRAKTEER_KAMU_DI_SINI)**

**What's inside the Premium ZIP?**
1. 📁 **SquareLine Studio Project (.spj):** Fully editable UI project. Change colors, layouts, or add your DJ group's logo effortlessly!
2. 📁 **Ready-to-Compile PlatformIO Folder:** 100% plug-and-play. No library version hunting required.
3. 📄 **Comprehensive PDF Guide:** Step-by-step instructions on wiring, driver installations, and exactly how to generate your Spotify API Tokens without the headache.
4. 🎨 **HD Assets:** All transparent PNG icons used in the project.

---

## 👨‍💻 Author
Built with passion by **Valentino**. 
An Electrical Engineering project exploring the intersection of IoT, Audio Tech, and Visual Interface Design.