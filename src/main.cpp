#include <Arduino.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "lv_demo_widgets.h"


#include "lvgl.h"


#define USE_HSPI_PORT
#define USE_DMA_TO_TFT


TFT_eSPI tft = TFT_eSPI();

#define DRAW_BUF_SIZE (240 * 320 / 10 * (LV_COLOR_DEPTH / 8))
static uint32_t draw_buf[DRAW_BUF_SIZE / 1];

void my_print(lv_log_level_t level, const char * buf)
{
    Serial.printf("LEVEL:%d :%s",level, buf);
    Serial.flush();

    
}
IRAM_ATTR void my_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}



uint32_t get_tick(){
    return millis();
}



void setup() {
    pinMode(4,OUTPUT);

    lv_init();
    tft.init();         // 初始化显示器
    tft.setRotation(1);   // 设置屏幕旋转
    
    lv_log_register_print_cb(my_print);
    lv_tick_set_cb(get_tick);
    lv_display_t * disp;
    disp = lv_display_create(240, 320);
    
    lv_display_set_rotation(disp,LV_DISPLAY_ROTATION_90);
    lv_display_set_flush_cb(disp, my_flush_cb);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // lv_obj_t *label = lv_label_create( lv_scr_act() );
    // lv_obj_set_style_text_font(label,&lv_font_montserrat_18,0);
    // lv_label_set_text( label, "Jie LOVE HHR" );
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );


lv_demo_widgets();
    lv_demo_widgets_start_slideshow();


    Serial.begin(115200);
    Serial.println("Hello ESP32S3 Enen");

}

void loop() {
    // digitalWrite(4,HIGH);
    // delay(1000);
    // digitalWrite(4, LOW);
    // delay(1000);

    lv_task_handler(); /* let the GUI do its work */
    delay(5);

}

