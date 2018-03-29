
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/mcpwm.h"
xQueueHandle Globle_Queue_handle=0,PWMMOter=0;
void adc1read(void *pvParameter){
  int readraw;
  while (1) {
  readraw=adc1_get_voltage(0);
  if (!xQueueSend(Globle_Queue_handle, &readraw, 1000)) {
    printf("error\n\r");
  }
  vTaskDelay(1000);
  //printf("%i\n\r",readraw);
  //vTaskDelay(1000 / portTICK_RATE_MS);
}}
void ADC1InputManger(void *pvParameter){
  int input;
  while (1) {
    if (xQueueReceive(Globle_Queue_handle, &input, 1000)) {
      printf("%i\n",input);
    }
    else{
      puts("mission failed we'ill get them next time");
    }
  }
}
/*void adc2read(void *pvParameter){
  while (1) {
  int readraw=adc2_get_voltage(0);
  printf("%i\n\r",readraw);
  vTaskDelay(1000 / portTICK_RATE_MS);
}}*/
void app_main()
{
      float duty_cycle;
      Globle_Queue_handle=xQueueCreate(2, sizeof(int));
      adc1_config_width(3);//3 for 12 bit, 2 for 11, 1 for 10, 0 for 9
      adc1_config_channel_atten(0,ADC_ATTEN_0db);
      mcpwm_config_t pwm_config;
      pwm_config.frequency = 1000;    //frequency = 500Hz,
      pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
      pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
      pwm_config.counter_mode = MCPWM_UP_COUNTER;
      pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
      mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
      mcpwm_gpio_init(0,1,16);
      mcpwm_set_frequency(0,0,10000);
      mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
      mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
      //adc2_config_width(3);//3 for 12 bit, 2 for 11, 1 for 10, 0 for 9
      //adc2_config_channel_atten(0,ADC_ATTEN_0db);
      xTaskCreate(adc1read,(const char*)"adc1read", 2048, NULL, 1, NULL);
      xTaskCreate(ADC1InputManger,(const char*)"inmang1", 2048, NULL, 2, NULL);
    //  xTaskCreate(adcread,"adc2read", 2048, NULL, 1, NULL);
    printf("Hello world!\n");
}
