#include "Arduino.h"
#include "camera.h"
#include "ESPAsyncWebServer.h"
#include "async_jpeg.h"

#define STA_SSID "your-ssid"
#define STA_PASS "your-pass"
#define AP_SSID  "esp32-cam"


// #define CAM_PINS_M5STACK_PSRAM

#ifdef CAM_PINS_M5STACK_PSRAM

#define CAM_PIN_RESET    15
#define CAM_PIN_XCLK     27
#define CAM_PIN_SIOD     25
#define CAM_PIN_SIOC     23

#define CAM_PIN_D7       19
#define CAM_PIN_D6       36
#define CAM_PIN_D5       18
#define CAM_PIN_D4       39
#define CAM_PIN_D3        5
#define CAM_PIN_D2       34
#define CAM_PIN_D1       35
#define CAM_PIN_D0       32

#define CAM_PIN_VSYNC    22
#define CAM_PIN_HREF     26
#define CAM_PIN_PCLK     21

#else //WROVER_KIT

#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    21
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      19
#define CAM_PIN_D2      18
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4

#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22
#endif

AsyncWebServer server(80);

static camera_model_t camera_model = CAMERA_NONE;

static camera_config_t camera_config = {
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,

    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000,

    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = CAMERA_PF_JPEG,//YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = CAMERA_FS_SVGA,//QVGA,VGA,SVGA,SXGA,UXGA

    .jpeg_quality = 10,//lower value gives higher quality
    .fb_count = 1 //if buffer equals 1, then frame is acquired on request, else camera is in continuous mode
};

bool cameraInit(){
    esp_err_t err = camera_probe(&camera_config, &camera_model);
    if (err != ESP_OK) {
        log_e("Camera probe failed with error 0x%x", err);
        return false;
    }
    if (camera_model == CAMERA_OV7725) {
        log_i("Detected OV7725 camera");
    } else if (camera_model == CAMERA_OV2640) {
        log_i("Detected OV2640 camera");
    } else {
        log_e("Camera not supported");
        return false;
    }
    err = camera_init(&camera_config);
    if (err != ESP_OK) {
        log_e("Camera init failed with error 0x%x", err);
        return false;
    }

    sensor_t * s = sensor();
    if (camera_config.pixel_format == CAMERA_PF_JPEG) {
    	s->set_quality(s, camera_config.jpeg_quality);//0 => 255
    }

    //s->set_colorbar(s, true);

    //s->set_pixformat(s, PIXFORMAT_JPEG);//PIXFORMAT_JPEG, PIXFORMAT_YUV422, PIXFORMAT_GRAYSCALE, PIXFORMAT_RGB565
    //s->set_framesize(s, FRAMESIZE_SVGA);//FRAMESIZE_40x30 => FRAMESIZE_UXGA
    //s->set_contrast(s, -2);//-3 => 2
    //s->set_brightness(s, 1);//-3 => 2
    //s->set_saturation(s, -2);//-3 => 2
    //s->set_gainceiling(s, GAINCEILING_16X);//GAINCEILING_2X => GAINCEILING_128X

    //s->set_whitebal(s, false);
    //s->set_gain_ctrl(s, false);
    //s->set_exposure_ctrl(s, false);
    //s->set_hmirror(s, true);
    //s->set_vflip(s, true);
    //skip_frame();

    return true;
}
/*
//frame capture routine example
void printImage(){
    esp_err_t err = camera_start();
    if (err != ESP_OK) {
        log_e("Camera capture failed with error = %d", err);
        return;
    }

    err = camera_wait();
    if (err != ESP_OK) {
        log_e("Camera capture failed with error = %d", err);
        return;
    }

    camera_fb_t * fb = camera_fb_get();
    if (fb == NULL) {
        log_e("Camera frame failed");
        return;
    }

    log_e("Camera done: len: %uB", fb->len);
    //write(fb->buf, fb->len);

    camera_fb_return(fb);
}
*/

void sendJpg(AsyncWebServerRequest *request){
    // esp_err_t err = camera_start();
    // if (err != ESP_OK) {
    //     log_e("Camera capture failed with error = %d", err);
    //     request->send(501);
    //     return;
    // }

    // err = camera_wait();
    // if (err != ESP_OK) {
    //     log_e("Camera capture failed with error = %d", err);
    //     request->send(501);
    //     return;
    // }

    // camera_fb_t * fb = camera_fb_get();
    // if (fb == NULL) {
    //     log_e("Camera frame failed");
    //     request->send(501);
    //     return;
    // }

    // uint32_t sig = *((uint32_t *)fb->buf) & 0xFFFFFF;
    // if(sig != 0xffd8ff){
    //     log_e("Bad JPEG signature[0x%x]: 0x%x", (uint32_t)fb->buf, sig);
    //     camera_fb_return(fb);
    //     request->send(501);
    //     return;
    // }

    // AsyncJpegResponse * response = new AsyncJpegResponse(fb);
    // if (response == NULL) {
    //     log_e("Response alloc failed");
    //     request->send(501);
    //     return;
    // }

    // request->send(response);
}

void streamJpg(AsyncWebServerRequest *request){
    AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
    if(!response){
        request->send(501);
        return;
    }
    request->send(response);
}


void setup(){
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    cameraInit();
    WiFi.softAP(AP_SSID);
    // WiFi.begin(STA_SSID, STA_PASS);
    server.on("/capture.jpg", HTTP_GET, sendJpg);
    server.on("/stream.jpg", HTTP_GET, streamJpg);
    server.onNotFound([](AsyncWebServerRequest *request){ request->send(404); });
    server.begin();
}

void loop(){
    delay(10);
}
