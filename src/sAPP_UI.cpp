#include "sAPP_UI.h"

TFT_eSPI tft = TFT_eSPI();

//LVGL双缓冲区
#define DRAW_BUF_SIZE (320 * 30 * 2)
static uint32_t draw_buf[DRAW_BUF_SIZE];
static uint32_t draw_buf2[DRAW_BUF_SIZE];





//上下的分割线
static lv_point_precise_t up_line_points[] = { {0, 25}, {320, 25} };
static lv_point_precise_t dn_line_points[] = { {0, 240 - 30}, {320, 240 - 30} };
static lv_style_t style_2line;
lv_obj_t* up_line,* dn_line;

//状态栏
lv_obj_t* status_wifi_label,* status_battery_label;

//底部滚动字幕
lv_obj_t* rolling_text_label;
extern const char *rolling_quotes;

//主页的时钟部分
lv_obj_t* clock_main_label,* clock_sec_label;

//主页的年月日 星期
lv_obj_t* date_label;


//LVGL回调
static void log_print(lv_log_level_t level, const char* buf);
IRAM_ATTR static void flush_screen(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p);
static uint32_t get_tick();


// 定义一个函数来获取随机字符串
const char* get_random_quote(const char* quotes[], size_t size) {
    if (size == 0) {
        return NULL; // 防止数组为空的情况
    }

    // 使用当前时间初始化随机数生成器
    srand((unsigned int)time(NULL));

    // 获取一个介于 0 和 size-1 之间的随机索引
    size_t random_index = rand() % size;

    // 返回对应的字符串指针
    return quotes[random_index];
}



// 返回星期几的函数
String getDayOfWeek(struct tm* timeinfo){
    // 定义一个数组，用来存储星期几的名称
    static const char* daysOfWeek[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
    // 使用 tm_wday 来获取星期几的索引
    int dayIndex = timeinfo->tm_wday; 
    // 返回对应的星期几名称
    return String(daysOfWeek[dayIndex]);
}

void sAPP_UI_UpdateDataHdr(){
    //更新WiFi信息
    String ssid;
    if(WiFi.isConnected()){
        ssid = WiFi.SSID();
        String wifi_icon = LV_SYMBOL_WIFI;
        String wifi_rssi = String(WiFi.RSSI());
        wifi_icon = wifi_icon + " " + ssid + "  " + wifi_rssi + "dBm";
        lv_label_set_text(status_wifi_label, wifi_icon.c_str());
    }else{
        lv_label_set_text(status_wifi_label, "NO WIFI");
    }

    //更新时间
    static bool is_sync_time;
    //通过NTP服务器同步时间
    if(WiFi.isConnected() && !is_sync_time){
        //GMT+8 中国北京
        is_sync_time = true;
        configTime(3600 * 8,0,"ntp.ntsc.ac.cn");
    }


    struct tm timeinfo;
    getLocalTime(&timeinfo);

    lv_label_set_text_fmt(clock_main_label,"%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min);
    lv_label_set_text_fmt(clock_sec_label,":%02d",timeinfo.tm_sec);


    char date_buf[32];
    sprintf(date_buf,"%04d.%02d.%02d  ", \
    timeinfo.tm_year + 1900,timeinfo.tm_mon + 1, timeinfo.tm_mday);
    lv_label_set_text(date_label,(String(date_buf) + String(getDayOfWeek(&timeinfo))).c_str());

    static int i = 0;

    if(i > 10 * 10){       //s
        i = 0;

        //滚动字幕
        lv_label_set_text_fmt(rolling_text_label,"%s", get_random_quote(&rolling_quotes,120));

    }else{
        i++;
    }

}



void sAPP_UI_InitWidgets(){
    //创建线样式
    lv_style_init(&style_2line);
    lv_style_set_line_width(&style_2line, 3);
    lv_style_set_line_color(&style_2line, lv_color_make(255,255,255));
    lv_style_set_line_rounded(&style_2line, false);

    //绘制上面的线
    up_line = lv_line_create(lv_screen_active());
    lv_line_set_points(up_line, up_line_points, 2);
    lv_obj_add_style(up_line, &style_2line, 0);
    lv_obj_set_align(up_line, LV_ALIGN_TOP_LEFT);

    //状态栏WIFI信息
    status_wifi_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(status_wifi_label,&lv_font_montserrat_24,0);
    lv_label_set_text(status_wifi_label, LV_SYMBOL_WIFI " " "9607a");
    lv_obj_align(status_wifi_label, LV_ALIGN_TOP_LEFT, 0, 0);

    //状态栏WIFI信息
    status_battery_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(status_battery_label,&lv_font_montserrat_24,0);
    lv_label_set_text(status_battery_label, LV_SYMBOL_BATTERY_3 " " " 74%");
    lv_obj_align(status_battery_label, LV_ALIGN_TOP_RIGHT, 0, 0);


    //时钟 15:30
    clock_main_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(clock_main_label,&Font_digitalTubeBig,0);
    lv_label_set_text(clock_main_label,"12:35");
    lv_obj_align(clock_main_label, LV_ALIGN_TOP_LEFT,10,60);

    //时钟 30 秒
    clock_sec_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(clock_sec_label,&Font_digitalTubeMed,0);
    lv_label_set_text(clock_sec_label,":54");
    lv_obj_align(clock_sec_label, LV_ALIGN_TOP_LEFT,235, 78);

    //2024.09.09
    date_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(date_label,&lv_font_montserrat_26,0);
    lv_label_set_text(date_label,"2024.09.09 MON");
    lv_obj_align(date_label, LV_ALIGN_TOP_LEFT,10, 150);


    //绘制下面的线
    dn_line = lv_line_create(lv_screen_active());
    lv_line_set_points(dn_line, dn_line_points, 2);
    lv_obj_add_style(dn_line, &style_2line, 0);
    lv_obj_set_align(dn_line, LV_ALIGN_TOP_LEFT);

    //底部滚动字幕
    rolling_text_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(rolling_text_label,&RollingFont,0);
    lv_obj_set_width(rolling_text_label,320);
    lv_label_set_text(rolling_text_label, "人们总以为自己未从走过的路上开满了鲜花。   " );
    lv_label_set_long_mode(rolling_text_label,LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_align(rolling_text_label, LV_ALIGN_BOTTOM_LEFT, 0, 0 );



}



void sAPP_UI_Init(){
    //初始化显示屏
    tft.init();
    //反转显示颜色
    tft.invertDisplay(true);
    //设置屏幕旋转
    tft.setRotation(1);
    //纯黑色
    tft.fillScreen(TFT_BLACK);

    //初始化LVGL
    lv_init();
    //注册log回调
    lv_log_register_print_cb(log_print);
    //注册获取滴答定时器回调
    lv_tick_set_cb(get_tick);
    //创建屏幕
    lv_display_t * disp;
    disp = lv_display_create(240, 320);
    //设置屏幕旋转90度
    lv_display_set_rotation(disp,LV_DISPLAY_ROTATION_90);
    //注册刷屏函数
    lv_display_set_flush_cb(disp, flush_screen);
    //设置屏幕部分双缓冲区
    lv_display_set_buffers(disp, draw_buf, draw_buf2, DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);


    sAPP_UI_InitWidgets();



}


static void log_print(lv_log_level_t level, const char* buf){
    Serial.printf("LEVEL:%d :%s",level, buf);
    Serial.flush();
}

IRAM_ATTR static void flush_screen(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p){
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

static uint32_t get_tick(){
    return millis();
}

