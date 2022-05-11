#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "N20";
const char* password = "zxcvbnmmmm";

WiFiServer server(1111);
TaskStatus_t taskDetails;

/* ------ FREERTOS TASKS ------*/
void serverTask(void *pvParameters)
{
    configASSERT(((uint32_t) pvParameters) == 1)
    bool report = false;
    for ( ;; )
    {
        /* code */
        if(WiFi.status() == WL_CONNECTED)
        {
            WiFiClient client = server.available();
            while (client) {
                if(!report){
                    Serial.println("we've got ourselves a client!!");
                    report = true;
                }
                client.println("HTTP/1.1 200 OK");
                vTaskDelay(100);    
            }
        }
        else if(WiFi.status() != WL_CONNECTED && report == false) 
        {
            Serial.println ("Wifi not connected");
            report =true;
        } 
    }    
}

void emptyTask(void *pvParameters)
{
    configASSERT(((uint32_t) pvParameters) == 1)
    for ( ;; )
    {
        /* code */
        Serial.println("Empty task.");
        vTaskDelay(10); 
    }    
}

/* ------ TASKS CREATION FUNCTIONS ------*/
void runServerTask (void)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
        /* TaskHandle */
        xReturned = xTaskCreate(
            serverTask,
            "serverTask",
            10000,
            (void *)1,
            3,
            &xHandle
        );
        if(xReturned == pdPASS)
        {
            Serial.println("Server Task started");
        }    
}

void runEmptyTask (void)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
        /* TaskHandle */
        xReturned = xTaskCreate(
            emptyTask,
            "emptyTask",
            1000,
            (void *)1,
            4,
            &xHandle
        );
        if(xReturned == pdPASS)
        {
            Serial.println("Empty Task started");
        }    
}


#if !CONFIG_AUTOSTART_ARDUINO
void arduinoTask(void *pvParameter) {

    while(1) {
       
    }
}

void app_main()
{
    // initialize arduino library before we start the tasks
    initArduino();

    xTaskCreate(&arduinoTask, "arduino_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
#else
void setup() {
     WiFi.begin(ssid, password);
     vTaskDelay(100);
     server.begin();
     Serial.begin(115200);
     runServerTask();
     runEmptyTask();
    
}

void loop() {
    
}
#endif 
