#include "sAPP_Func.h"

void sAPP_Func_RTC_Get12HTime(char* str){
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    if(timeinfo.tm_sec % 2 == 0){
        if(timeinfo.tm_hour > 12){
            sprintf(str,"PM %02d:%02d",timeinfo.tm_hour - 12,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour == 12){
            sprintf(str,"PM %02d:%02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour < 12){
            sprintf(str,"AM %02d:%02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
    }else{
        if(timeinfo.tm_hour > 12){
            sprintf(str,"PM %02d %02d",timeinfo.tm_hour - 12,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour == 12){
            sprintf(str,"PM %02d %02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
        else if(timeinfo.tm_hour < 12){
            sprintf(str,"AM %02d %02d",timeinfo.tm_hour - 0,timeinfo.tm_min);
        }
    }
}


void sAPP_Func_RTC_Init(){
    struct tm timeinfo = {};
    timeinfo.tm_year = 2024 - 1900; // 年份 - 1900
    timeinfo.tm_mon = 9 - 1;        // 月份，从0开始
    timeinfo.tm_mday = 8;          // 日
    timeinfo.tm_hour = 12;          // 时
    timeinfo.tm_min = 30;           // 分
    timeinfo.tm_sec = 0;            // 秒
    //做一个时间戳
    time_t t = mktime(&timeinfo);
    timeval now = { .tv_sec = t };
    //用时间戳设置时间
    settimeofday(&now, NULL);
}


