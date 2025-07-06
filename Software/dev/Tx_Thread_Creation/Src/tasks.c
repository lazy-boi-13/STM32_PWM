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


uint8_t addresses[1] = {RS485_ENC0};  //data to send with readposition command
uint8_t DataR[2] = {0,0}; //array to catch encoder response 
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
    AAR = 100, f = 1000Hz
    AAR = 1000, f = 100Hz (default)
    AAR = 10000, f = 10Hz  
    */
    .peripherie = STEPPER_1,
    .countUp = false,
    .maxVal = 1500,  
    .minVal = 500,
    .stepSize = 1

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
 @param  huart  uart instance for serial communication with the Encoder
 @param  timer  unused
 @param  adc  unused 
 @return -
 ****************************************************************
 */
void MainThread(UART_HandleTypeDef* huart, TIM_HandleTypeDef* timer, ADC_HandleTypeDef* adc)
{

  while (1)
  {
    for(int encoder = 0; encoder < sizeof(addresses); ++encoder)
    {

      setStateRS485(RS485_T_TX); //put the transciver into transmit mode

      HAL_UART_Transmit(huart, &addresses[encoder], 1, 100);    //send the command to get position. All we have to do is send the node address


      // after the readposition command has been sent it takes the encoder 11uS to respond with 
      // 20 bits of data, at a rate of 115200 bits/s that takes 185 uS to get position after read
      // command has been sent 
      
      setStateRS485(RS485_T_RX); //set the transceiver back into receive mode for the encoder response

      HAL_UART_Receive(huart, DataR, 2,100);
      

      uint16_t currentPosition = DataR[0]; // low byte comes first

      currentPosition |= DataR[1] << 8; // high byte next, OR it into our 16 bit holder


      
      if (verifyChecksumRS485(currentPosition))
      {
        //we got back a good position, so just mask away the checkbits
        currentPosition &= 0x3FFF;

        //If the resolution is 12-bits, then shift position
        if (RESOLUTION == 12)
        {
          currentPosition = currentPosition >> 2;
        }
          // printf("current Position: %d\n", currentPosition);  // DEBUG
      }
      else
      {
        // printf("%s\n", "error: Invalid checksum."); // DEBUG
      }

     /*
      read the adc value(s) and modify the stepper period accordingly with respect to min and max values
      from the encoder(s)
     */

     HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // indicates the code is running

     _tx_thread_sleep(100);  // restart task every 100 ticks to enable context switch

   } // end of for
 } // end of while

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



  adc_init(hadc, timers[servotimer]); // start ADC in circular mode

  hal_timerPWM_start(timers[servotimer], timers[steppertimer], pwm);  // start PWM Generation


  /* Infinite loop */
  while(1)
  {


    hal_sweep(timers[servotimer], &pwm[TIMERPWM_SERVO_0]);  // sweep servo 0


    hal_sweep(timers[servotimer], &pwm[TIMERPWM_SERVO_1]);  // sweep servo 1

    hal_sweep(timers[servotimer], &pwm[STEPPER_1]); // sweep stepper 1 on PA7


    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    _tx_thread_sleep(10);  // restart task every 200 ticks to allow context switch

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


/* ---Helper functions--- */


bool verifyChecksumRS485(uint16_t message)
{
  //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent.
  //checksum is invert of XOR of bits, so start with 0b11, so things end up inverted
  uint16_t checksum = 0x3;
  for(int i = 0; i < 14; i += 2)
  {
    checksum ^= (message >> i) & 0x3;
  }
  return checksum == (message >> 14);
}





void setStateRS485(uint8_t state)
{
  //switch case to find the mode we want
  switch (state)
  {
    case RS485_T_TX:
      /*     
      Transmitting: 
      set ~RE to HIGH -> sets RXD to High Impedance
      set DE to HIGH -> sets TXD to either High or low and thus Y and or Z to high or low     
      */ 
      HAL_GPIO_WritePin(Recieve_Enable_GPIO_Port, Recieve_Enable_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Drive_Enable_GPIO_Port, Drive_Enable_Pin, GPIO_PIN_SET);
      break;
    case RS485_T_RX:
      /*
      Recieving: 
      set ~RE to LOW -> sets RXD to either HIGH low or Don't care depending on A-B Voltage
      set DE to LOW -> sets TXD to Don't care and Y and Z to high Impedance
      */ 
      HAL_GPIO_WritePin(Recieve_Enable_GPIO_Port, Recieve_Enable_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Drive_Enable_GPIO_Port, Drive_Enable_Pin,GPIO_PIN_RESET);
      break;

  }

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
