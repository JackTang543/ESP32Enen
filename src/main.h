#pragma once


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>



#include "sAPP_UI.h"
#include "sAPP_Func.h"
#include "sAPP_Tasks.h"


/* TFT_eSPI */
//使用HSPI
#define USE_HSPI_PORT
//使用DMA
#define USE_DMA_TO_TFT



extern SemaphoreHandle_t screen_rending;



