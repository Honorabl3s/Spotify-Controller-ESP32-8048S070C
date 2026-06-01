#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "LGFX_8048S070C.hpp"
#include <lvgl.h>
#include "ui/ui.h"
#include <TJpg_Decoder.h>


const char* ssid = "tea";
const char* password = "12345678";
const char* client_id = "83ca87883a7341309d8c5a004f268e9d";
const char* client_secret = "f7dd8473c49441059b9a1dd15e291d0d";
const char* refresh_token = "AQC1NK2qV468pk3M2Sdd-LE8kPnQQnJiCLlMkfSH6OcoWiz85_kL7GD96EzWwaC489oVwTdvl7qi_356qJUAdoSrbXmGdoPMOy1p0j8yxFH8aqdn3IwBrrRzs1MT_scMoZw";

String access_token = "";
unsigned long lastTime = 0;
unsigned long timerDelay = 3000; 
bool isPlaying = false;
String currentSongName = "";
String currentImageUrl = "";

LGFX lcd;

static const uint32_t screenWidth  = 800;
static const uint32_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

uint16_t* rawBuffer;
lv_color_t* coverBuffer;
lv_img_dsc_t coverDsc;

uint16_t currentImgWidth = 640;
uint16_t currentImgHeight = 640;

bool tjpg_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    for(int j = 0; j < h; j++) {
        if (y + j >= currentImgHeight) break; 
        for(int i = 0; i < w; i++) {
            if (x + i >= currentImgWidth) break;
            rawBuffer[(y + j) * currentImgWidth + (x + i)] = bitmap[j * w + i];
        }
    }
    return 1;
}

void processImageToHD() {
    uint32_t dst_w = 350;
    uint32_t dst_h = 350;
    uint32_t src_w = currentImgWidth;
    uint32_t src_h = currentImgHeight;
    
    uint32_t x_ratio = ((src_w - 1) << 8) / dst_w;
    uint32_t y_ratio = ((src_h - 1) << 8) / dst_h;
    
    for (int i = 0; i < dst_h; i++) {
        uint32_t y_orig = (i * y_ratio) >> 8;
        uint32_t y_diff = (i * y_ratio) & 0xFF;
        
        for (int j = 0; j < dst_w; j++) {
            uint32_t x_orig = (j * x_ratio) >> 8;
            uint32_t x_diff = (j * x_ratio) & 0xFF;
            
            uint32_t index = y_orig * src_w + x_orig;
            
            uint16_t a = rawBuffer[index];
            uint16_t b = rawBuffer[index + 1];
            uint16_t c = rawBuffer[index + src_w];
            uint16_t d = rawBuffer[index + src_w + 1];
            
            uint32_t w_a = (256 - x_diff) * (256 - y_diff);
            uint32_t w_b = x_diff * (256 - y_diff);
            uint32_t w_c = (256 - x_diff) * y_diff;
            uint32_t w_d = x_diff * y_diff;
            
            uint32_t r = ((a >> 11) & 0x1F) * w_a + ((b >> 11) & 0x1F) * w_b + 
                         ((c >> 11) & 0x1F) * w_c + ((d >> 11) & 0x1F) * w_d;
            uint32_t g = ((a >> 5) & 0x3F) * w_a + ((b >> 5) & 0x3F) * w_b + 
                         ((c >> 5) & 0x3F) * w_c + ((d >> 5) & 0x3F) * w_d;
            uint32_t blue = (a & 0x1F) * w_a + (b & 0x1F) * w_b + 
                            (c & 0x1F) * w_c + (d & 0x1F) * w_d;
                            
            coverBuffer[i * dst_w + j].full = (((r >> 16) & 0x1F) << 11) | (((g >> 16) & 0x3F) << 5) | ((blue >> 16) & 0x1F);
        }
    }
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.pushPixels((uint16_t *)&color_p->full, w * h, true);
    lcd.endWrite();
    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch(&touchX, &touchY);
    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void getSpotifyToken() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  
  http.begin(client, "https://accounts.spotify.com/api/token");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpRequestData = "grant_type=refresh_token&refresh_token=" + String(refresh_token) + 
                           "&client_id=" + String(client_id) + "&client_secret=" + String(client_secret);
  
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode == 200) {
    String response = http.getString();
    JsonDocument doc; 
    deserializeJson(doc, response);
    access_token = doc["access_token"].as<String>();
    Serial.println("Access Token Diperbarui!");
  }
  http.end();
}

void getCurrentlyPlaying() {
  if (access_token == "") return;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  
  http.begin(client, "https://api.spotify.com/v1/me/player/currently-playing");
  http.addHeader("Authorization", "Bearer " + access_token);

  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) { 
    String response = http.getString();
    JsonDocument doc; 
    deserializeJson(doc, response);

    String songName = doc["item"]["name"].as<String>();
    
    String artistName = "";
    JsonArray artistsArray = doc["item"]["artists"].as<JsonArray>();
    for(int i = 0; i < artistsArray.size(); i++) {
        artistName += artistsArray[i]["name"].as<String>();
        if(i < artistsArray.size() - 1) {
            artistName += ", "; 
        }
    }

    isPlaying = doc["is_playing"].as<bool>();

    if (isPlaying) {
        lv_obj_add_flag(ui_BtnPlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_BtnPause, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui_BtnPause, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_BtnPlay, LV_OBJ_FLAG_HIDDEN);
    }

    int progressMs = doc["progress_ms"].as<int>();
    int durationMs = doc["item"]["duration_ms"].as<int>();
    
    int progSec = progressMs / 1000;
    int durSec = durationMs / 1000;
    
    // --- FITUR 1: PELINDUNG SAAT ARC SEDANG DIGESER ---
    // Hanya perbarui garis secara otomatis jika pengguna tidak sedang menyentuh layar
    if (!lv_obj_has_state(ui_SliderProgress, LV_STATE_PRESSED)) {
        lv_arc_set_range(ui_SliderProgress, 0, durSec);
        lv_arc_set_value(ui_SliderProgress, progSec);
        
        char timeString[16];
        snprintf(timeString, sizeof(timeString), "%02d:%02d / %02d:%02d", 
                 (progSec / 60), (progSec % 60), 
                 (durSec / 60), (durSec % 60));
                 
        lv_label_set_text(ui_SongTime, timeString);
    }
    // ---------------------------------------------------

    if (songName != "null" && songName != currentSongName) {
        currentSongName = songName; 

        lv_obj_set_width(ui_LabelJudul, 310); 
        lv_obj_set_height(ui_LabelJudul, 55); 
        lv_label_set_long_mode(ui_LabelJudul, LV_LABEL_LONG_SCROLL_CIRCULAR);

        lv_obj_set_width(ui_LabelArtis, 310); 
        lv_obj_set_height(ui_LabelArtis, 30); 
        lv_label_set_long_mode(ui_LabelArtis, LV_LABEL_LONG_SCROLL_CIRCULAR);

        lv_label_set_text(ui_LabelJudul, songName.c_str());
        lv_label_set_text(ui_LabelArtis, artistName.c_str());
    }

    String imageUrl = doc["item"]["album"]["images"][0]["url"].as<String>();
    
    if (imageUrl != "null" && imageUrl != currentImageUrl) {
        currentImageUrl = imageUrl;

        HTTPClient httpImg;
        httpImg.begin(client, imageUrl);
        int httpImgCode = httpImg.GET();
        
        if (httpImgCode == HTTP_CODE_OK) {
            int len = httpImg.getSize();
            uint8_t* jpgBuf = (uint8_t*)ps_malloc(len);
            if (jpgBuf) {
                WiFiClient* stream = httpImg.getStreamPtr();
                stream->readBytes(jpgBuf, len);
                
                uint16_t real_w, real_h;
                TJpgDec.getJpgSize(&real_w, &real_h, jpgBuf, len);
                
                currentImgWidth = real_w;
                currentImgHeight = real_h;
                
                TJpgDec.drawJpg(0, 0, jpgBuf, len);
                free(jpgBuf); 
                
                processImageToHD();
                
                lv_img_set_src(ui_ImgCover1, &coverDsc);
            }
        }
        httpImg.end();
    }
  } else if (httpResponseCode == 401) { 
    getSpotifyToken();
  }
  http.end();
}

void controlSpotify(String endpoint, String method) {
    if (access_token == "") return;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    
    http.begin(client, "https://api.spotify.com/v1/me/player/" + endpoint);
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Length", "0"); 
    
    int httpResponseCode;
    if (method == "POST") httpResponseCode = http.POST("");
    else if (method == "PUT") httpResponseCode = http.PUT("");
    
    if (httpResponseCode == 204) {
        getCurrentlyPlaying(); 
    }
    http.end();
}

// --- FITUR 2: FUNGSI UNTUK MENANGKAP GESERAN JARI ---
static void arc_progress_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * arc = lv_event_get_target(e);
    int targetSec = lv_arc_get_value(arc);
    
    // Saat sedang digeser: Perbarui teks waktu agar sesuai letak jari (Live Feedback)
    if (code == LV_EVENT_VALUE_CHANGED) {
        int durSec = lv_arc_get_max_value(arc);
        char timeString[16];
        snprintf(timeString, sizeof(timeString), "%02d:%02d / %02d:%02d", 
                 (targetSec / 60), (targetSec % 60), 
                 (durSec / 60), (durSec % 60));
        lv_label_set_text(ui_SongTime, timeString);
    }
    // Saat jari dilepaskan: Kirim posisi waktu yang dituju ke Spotify
    else if (code == LV_EVENT_RELEASED) {
        int targetMs = targetSec * 1000;
        String endpoint = "seek?position_ms=" + String(targetMs);
        lv_timer_handler(); 
        controlSpotify(endpoint, "PUT");
    }
}
// ----------------------------------------------------

static void btn_prev_cb(lv_event_t * e) { 
    lv_timer_handler();
    controlSpotify("previous", "POST"); 
}
static void btn_next_cb(lv_event_t * e) { 
    lv_timer_handler();
    controlSpotify("next", "POST"); 
}

static void btn_play_cb(lv_event_t * e) { 
    lv_obj_add_flag(ui_BtnPlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_BtnPause, LV_OBJ_FLAG_HIDDEN);
    
    lv_timer_handler(); 
    
    controlSpotify("play", "PUT");
    isPlaying = true; 
}

static void btn_pause_cb(lv_event_t * e) { 
    lv_obj_add_flag(ui_BtnPause, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_BtnPlay, LV_OBJ_FLAG_HIDDEN);
    
    lv_timer_handler(); 
    
    controlSpotify("pause", "PUT");
    isPlaying = false; 
}

void setup() {
    Serial.begin(115200);
    
    lcd.init();
    lcd.setBrightness(255);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH); 
    
    rawBuffer = (uint16_t*)ps_malloc(640 * 640 * sizeof(uint16_t));
    coverBuffer = (lv_color_t*)ps_malloc(350 * 350 * sizeof(lv_color_t));
    
    coverDsc.header.always_zero = 0;
    coverDsc.header.w = 350;
    coverDsc.header.h = 350;
    coverDsc.data_size = 350 * 350 * sizeof(lv_color_t);
    coverDsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    coverDsc.data = (const uint8_t*)coverBuffer;

    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(false);
    TJpgDec.setCallback(tjpg_cb);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    ui_init();
    
    lv_obj_set_style_radius(ui_ImgCover1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(ui_ImgCover1, true, 0);

    lv_obj_add_event_cb(ui_BtnPrev, btn_prev_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_BtnNext, btn_next_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_BtnPlay, btn_play_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_BtnPause, btn_pause_cb, LV_EVENT_CLICKED, NULL); 

    // --- FITUR 3: MENDAFTARKAN EVENT ARC (Geser) ---
    lv_obj_add_event_cb(ui_SliderProgress, arc_progress_cb, LV_EVENT_ALL, NULL);
    // -----------------------------------------------

    Serial.print("Menghubungkan ke Wi-Fi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Terhubung!");
    
    getSpotifyToken(); 
}

void loop() {
    lv_timer_handler(); 
    delay(5);
    lv_tick_inc(5);

    if ((millis() - lastTime) > timerDelay) {
        if(WiFi.status() == WL_CONNECTED){
            getCurrentlyPlaying();
        }
        lastTime = millis();
    }
}