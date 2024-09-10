
#include "main.h"


//屏幕渲染中的互斥锁
SemaphoreHandle_t screen_rending;



char* WiFi_SSID = "9607a";
char* WiFi_PWD  = "960796079607";


void setup() {
    pinMode(4,OUTPUT);
    Serial.begin(115200);

    WiFi.begin(WiFi_SSID, WiFi_PWD);
    // uint8_t i = 0;
    // while (!WiFi.isConnected()) {
    //     delay(1000); i++;    //等待10s
    //     if(i > 10){break;}
    // }
    // if(WiFi.isConnected()){
    //     Serial.println("Connected to WiFi");
    // }else{
    //     Serial.println("Unable to connect to WiFi");
    // }

    //通过NTP服务器同步时间
    if(WiFi.isConnected()){
        //GMT+8 中国北京
        configTime(3600 * 8,0,"ntp.ntsc.ac.cn");
    }
    

    

    //初始化UI
    sAPP_UI_Init();


    sAPP_Func_RTC_Init();

    //屏幕渲染资源互斥锁
    screen_rending = xSemaphoreCreateMutex();
    
    //label = lv_label_create( lv_screen_active() );

    
    // lv_obj_set_style_text_font(label,&lv_font_montserrat_24,0);
    // lv_label_set_text( label, "Sightseer. inHNIP9607" );
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );


    // lv_obj_center(line1);


    // lv_demo_widgets();
    // lv_demo_widgets_start_slideshow();


    Serial.begin(115200);
    Serial.println("Hello ESP32S3 Enen");

    xTaskCreate(sAPP_Tasks_LVGLTask     ,"LVGL_Task"     ,4096,NULL,2,NULL);
    xTaskCreate(sAPP_Tasks_UpdateScreen ,"UpdateScreen"  ,4096,NULL,5,NULL);

    

}



void loop() {
    // digitalWrite(4,HIGH);
    // delay(1000);
    // digitalWrite(4, LOW);
    // delay(1000);



    
    delay(1000);

}

