/**
 ****************************************************************
 @file    tasks.c
 ****************************************************************
 @brief   This module contains the threads that control a
 @brief   robotic arm
 ****************************************************************
 @author  Christian Weidmann
 @version 0
 @date    05.04.2025
 ****************************************************************
 */



// this is a TEST


#include <stdio.h>  // definition of printf

#include "tasks.h"
#include "main.h"
#include "app_threadx.h"
#include "timerpwm.h"

#define DEBUG_ADC


volatile uint16_t PotValues[NUM_OF_POTIS];
volatile uint16_t ADC_BUF[2] __attribute__((section(".nocache"))); // add this array to non cacheable area


static void adc_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* triggertimer);



// all the connected peripherals and their values
pwmSettings_t pwm[TIMERPWM_LAST_PERIPHERIE] = 

{

  {
    .peripherie = TIMERPWM_SERVO_0,
    .countUp = false,
    .maxVal = 500,
    .minVal = 100,
    .stepSize = 10 

  },
  
  {
    .peripherie = TIMERPWM_SERVO_1,
    .countUp = false,
    .maxVal = 500,
    .minVal = 250,
    .stepSize = 10 

  },

  {
    .peripherie = TIMERPWM_SERVO_2,
    .countUp = false,
    .maxVal = 900,
    .minVal = 100,
    .stepSize = 1

  },

  {
    .peripherie = TIMERPWM_SERVO_3,
    .countUp = false,
    .maxVal = 900,
    .minVal = 100,
    .stepSize = 1

  },

  {
    .peripherie = TIMERPWM_SERVO_4,
    .countUp = false,
    .maxVal = 900,
    .minVal = 100,
    .stepSize = 1

  },

  {
    /*

    Auto Reload Register (AAR)
    contains period

    AAR = 100, f = 1000Hz
    AAR = 1000, f = 100Hz (default)
    AAR = 10000, f = 10Hz
    
    careful: the pulse value always stays the same no 
    matter what the period is

    */

    // pulse freq. = (clkSource/prescaler)/CounterPeriod = 192Mhz/192/1000 = 1kHz with pulse @200 equals 20% duty cycle

    .peripherie = STEPPER_1,
    .countUp = false,
    .maxVal = 1000, // 1000Hz
    .minVal = 900, // 1111Hz
    .stepSize = 5

  },

  {
    .peripherie = STEPPER_2,
    .countUp = false,
    .maxVal = 900,
    .minVal = 100,
    .stepSize = 1

  }


};




/**
 ****************************************************************
 @brief  Start all PWM's
 @param  pwmtimer timer which outputs the pwm signals
 @param  huart  uart instance for serial communication with the Encoder (AMT213) 
 @param  timer  unused
 @param  adc  unused 
 @return -
 ****************************************************************
 */
void MainThread(UART_HandleTypeDef* huart, TIM_HandleTypeDef* timer, ADC_HandleTypeDef* adc)
{


  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // indicates the code is running

  _tx_thread_sleep(100);  // restart task every 100 ticks to enable context switch


}


/**
 ****************************************************************
 @brief  Initializes ADC and PWM Generation then controls all servos
 @param  hadc ADC struct which samples the stick (2 inputs, x and y)
 @param  pwmtimer timer struct which outputs the PWM Signals
 @param  triggertimer timer which periodically triggers an ADC Conversion 
 @return -
 ****************************************************************
 */
void ServoControl(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef** timers)
{

  adc_init(hadc, timers[triggertimer]); // start ADC in circular mode

  hal_timerPWM_start(timers[servotimer], timers[steppertimer], pwm);  // start PWM Generation on all channels


  /* Infinite loop */
  while(1)
  {

    hal_sweep(timers[servotimer], &pwm[TIMERPWM_SERVO_0]);  // sweep servo 0
    hal_sweep(timers[servotimer], &pwm[TIMERPWM_SERVO_1]);  // sweep servo 1
    hal_sweep(timers[steppertimer], &pwm[STEPPER_1]); // sweep stepper 1 on PC7

    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    _tx_thread_sleep(10);

  }
  
}


/**
 ****************************************************************
 @brief  reserve thread for extending functionallity in the future
 @param  - 
 @return -
 ****************************************************************
 */
void ExtraThread(void)
{

  

  while (1) 
  {

    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    _tx_thread_sleep(100);  // restart task every 100 ticks to enable context switch

  }

  
}


/**
 ****************************************************************
 @brief  Conversion complete callback function
 @brief  this fucntion is called everytime the adc has done a full
 @brief  conversion
 @param  - 
 @return -
 ****************************************************************
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)  
{
    
    uint16_t value0 = ADC_BUF[0];
    uint16_t value1 = ADC_BUF[1];

    // 16 bit adc means the result is between 0 and 65536

    // map the raw adc values accordingly

    // ...

    PotValues[0] = value0;
    PotValues[1] = value1;

#ifdef DEBUG_ADC

    printf("X Value: %u\n", ADC_BUF[0]);  // DEBUG
    printf("Y Value: %u\n", ADC_BUF[1]);  // DEBUG 

#endif

}


/**
 ****************************************************************
 @brief  Initializes the ADC, starts the triggering timer 
 @param  hadc ADC Instance
 @param  triggertimer timer which triggers an adc conversion
 @return -
 ****************************************************************
 */
void adc_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* triggertimer)
{


  if (HAL_OK != HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED))   // calibrate the adc
  {
    Error_Handler();
  } 
  
  if (HAL_OK != HAL_ADC_Start_DMA(hadc, (uint32_t*) &ADC_BUF, NUM_OF_POTIS))   // start the ADC conversion over DMA in circular mode
  {
    Error_Handler();
  } 

  HAL_TIM_Base_Start(triggertimer); // start the timer that triggers ADC Conversions

}
