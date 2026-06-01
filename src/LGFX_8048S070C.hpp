#pragma once
#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp> // Tambahkan baris ini
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>   // Tambahkan baris ini

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_RGB     _panel_instance;
    lgfx::Bus_RGB       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_GT911   _touch_instance; // Modul 7" biasanya menggunakan GT911 Capacitive Touch

public:
    LGFX(void) {
        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;
            
            // Konfigurasi Pin RGB (Spesifik untuk ESP32-8048S070C)
            cfg.pin_d0  = 15; // B0
            cfg.pin_d1  = 7;  // B1
            cfg.pin_d2  = 6;  // B2
            cfg.pin_d3  = 5;  // B3
            cfg.pin_d4  = 4;  // B4
            cfg.pin_d5  = 9;  // G0
            cfg.pin_d6  = 46; // G1
            cfg.pin_d7  = 3;  // G2
            cfg.pin_d8  = 8;  // G3
            cfg.pin_d9  = 16; // G4
            cfg.pin_d10 = 1;  // G5
            cfg.pin_d11 = 14; // R0
            cfg.pin_d12 = 21; // R1
            cfg.pin_d13 = 47; // R2
            cfg.pin_d14 = 48; // R3
            cfg.pin_d15 = 45; // R4

            cfg.pin_henable = 41;
            cfg.pin_vsync   = 40;
            cfg.pin_hsync   = 39;
            cfg.pin_pclk    = 42;
            cfg.freq_write  = 12000000;

            // Timing sinkronisasi layar 800x480
           cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 40; // Diperlebar
            cfg.hsync_pulse_width = 48; // Diperlebar
            cfg.hsync_back_porch  = 40; // Diperlebar
            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 1;
            cfg.vsync_pulse_width = 31; // Diperlebar
            cfg.vsync_back_porch  = 13; // Diperlebar
            cfg.pclk_idle_high    = 0;
            cfg.de_idle_high      = 0;
            cfg.pclk_active_neg   = 1;
            
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        {
            auto cfg = _panel_instance.config();
            cfg.memory_width  = 800;
            cfg.memory_height = 480;
            cfg.panel_width   = 800;
            cfg.panel_height  = 480;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            _panel_instance.config(cfg);
        }
        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = 2; // Pin Backlight
            cfg.freq   = 44100;
            cfg.pwm_channel = 7;
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }
        {
            auto cfg = _touch_instance.config();
            cfg.x_min      = 0;
            cfg.x_max      = 799;
            cfg.y_min      = 0;
            cfg.y_max      = 479;
            cfg.pin_int    = -1;
            cfg.pin_rst    = 38;
            cfg.bus_shared = false;
            cfg.offset_rotation = 0;
            cfg.i2c_port   = 1;
            cfg.pin_sda    = 19;
            cfg.pin_scl    = 20;
            cfg.freq       = 400000;
            cfg.i2c_addr   = 0x5D; 
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
        setPanel(&_panel_instance);
    }
};