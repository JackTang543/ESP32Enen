#include "sAPP_Tasks.h"





//更新屏幕任务
void sAPP_Tasks_LVGLTask(void* pvPara){
    for(;;){
        //屏幕渲染加锁
        if(xSemaphoreTake(screen_rending, portMAX_DELAY) == pdTRUE){
            lv_task_handler();
            //渲染完释放
            xSemaphoreGive(screen_rending);
        }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

//更新屏幕数据
void sAPP_Tasks_UpdateScreen(void* pvPara){
    for(;;){
        //获取锁
        if(xSemaphoreTake(screen_rending, portMAX_DELAY) == pdTRUE){
            //调用UI处理函数
            sAPP_UI_UpdateDataHdr();
            //释放
            xSemaphoreGive(screen_rending);
        }
        //10Hz刷新一次
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}





