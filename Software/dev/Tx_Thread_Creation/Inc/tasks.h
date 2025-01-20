/*
  ******************************************************************************
  * @file    tasks.h
  * @brief   This file contains the headers of the callback functions.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASKS_H
#define __TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdbool.h"


/*  SERVO PULSE WIDTH
 *
 * PWM-Settings minimum and max value is the value of the duty cycle with 200 Hz period
 * e.g 100 equals 10% DC at 200 Hz which is a pulse width of 0.1 * 1/200 = 500 microseconds
 * 300 equals 30% @ 200 Hz which is 0.3 * 1/200 = 1500 microseconds
 * the maximum Duty Cycle for a 2000 microseconds pulse is 50%
 * the dm1500's range is 500 to 2000 microseconds that is a duty in parts of thousand from
 * 100 to 500
 *
 */

/*  MAP POTVALUES TO DUTYCYCLE
 *
 *  in partsPerThousand with 12 bit ADC
 *  example for digital representation of physical stick position
 *  in x direction.
 *
 * - potValue = 3 when all the way up
 * - potValue = [1900 - 2300] center position (varies from stick to stick)
 * - potValue = 4094 when all the way down
 *
 */

/*  STEPPER FREQUENCY
 *
 * The Stepper Motors are controlled through frequency rather than pulse width
 * minVal and maxVal refer to the frequency and not the duty cycle.
 * Each Stepper also has GPIO pin assigned to that when toggled changes direction
 * And an enable pin for error handling
 *
 */

/*
 * STEPPER HOLDING TORQUE
 *
 * The holding torque is created by setting the step pin to high for as long
 * as the holding torque must persist, this is done by setting the Duty cycle to zero
 *
 */




/* 
 * Control the AMT21xE encoder.
 * It uses one UART to control the encoder and the the another UART to report back to the PC
 * via the Serial Monitor.
 * For more information or assistance contact CUI Devices for support.
 *
 *
 * AMT21 Pin Connections
 * Vdd (5V):    Pin  .
 * B:           Pin  .
 * A:           Pin  .
 * GND:         Pin  .
 * NC:          Pin  .
 * NC:          Pin  .
 *
 *
 * Required Equipment Note:
 * The AMT21 requires a high speed RS485 transceiver for operation. For this demo
 * a generic RS485 transceiver was used. This code was
 * made in conjunction with a walkthrough demo project available online at www.cuidevices.com/resources/.
 * 
 */

/* Serial rates for UART */
#define USB_BAUDRATE         115200
#define RS485_BAUDRATE       115200

/* We will use this define macro so we can write code once compatible with 12 or 14 bit encoders */
#define RESOLUTION            12

/* The AMT21 encoder is able to have a range of different values for its node address. This allows there
 * to be multiple encoders on on the RS485 bus. The encoder will listen for its node address, and that node
 * address doubles as the position request command. This simplifies the protocol so that all the host needs
 * to do is transmit the node address and the encoder will immediately respond with the position. The node address
 * can be any 8-bit number, where the bottom 2 bits are both 0. This means that there are 63 available addresses.
 * The bottom two bits are left as zero, because those bit slots are used to indicate extended commands. Taking
 * the node address and adding 0x01 changes the command to reading the turns counter (multi-turn only), and adding
 * 0x02 indicates that a second extended command will follow the first. We will define two encoder addresses below,
 * and then we will define the modifiers, but to reduce code complexity and the number of defines, we will not
 * define every single variation. 0x03 is unused and therefore reserved at this time.
 *
 */
#define RS485_RESET           0x75
#define RS485_ZERO            0x5E
#define RS485_ENC0            0x54
#define RS485_ENC1            0x58

#define RS485_POS             0x00 //this is unnecessary to use but it helps visualize the process for using the other modifiers
#define RS485_TURNS           0x01
#define RS485_EXT             0x02

/* The RS485 transceiver uses 2 pins to control the state of the differential RS485 lines A/B. To control
 * the transevier we will put those pins on our digital IO pins. We will define those pins here now. We will get
 * into more information about this later on, but it is important to understand that because of the high speed of the
 * AMT21 encoder, the toggling of these pins must occur very quickly. More information available in the walkthrough online.
 * Receive enable, drive enable, data in, received data out. Typically the RE and DE pins are connected together and controlled
 * with 1 IO pin but we'll connect them all up for full control.
 */
#define RS485_T_RE            8
#define RS485_T_DE            9
#define RS485_T_DI            18
#define RS485_T_RO            19

/* For ease of reading, we will create a helper function to set the mode of the transceiver. We will send that funciton
 * these arguments corresponding with the mode we want.
 */
#define RS485_T_TX            0 //transmit: receiver off, driver on
#define RS485_T_RX            1 //receiver: driver off, transmit on

bool verifyChecksumRS485(uint16_t message);


/*
 * This function sets the state of the RS485 transceiver. We send it that state we want. Recall above I mentioned how we need to do this as quickly
 * as possible. To be fast, we are not using the digitalWrite functions but instead will access the avr io directly. I have shown the direct access
 * method and left commented the digitalWrite method.
 */
void setStateRS485(uint8_t state);

/**
  * @brief Main thread 
  * @param huart struct(s) for sending and recieving encoder information
  *        timer struct(s) for adjusting the period of the stepper pwm
  *        adc struct (s) for reading the anoalog stick value 
  *        
  * @retval None
  */

void MainThread(UART_HandleTypeDef* huart, TIM_HandleTypeDef* timer, ADC_HandleTypeDef* adc);


/**
  * @brief Thread One 
  * @param  None    
  * @retval None
  */
void ThreadOne_x(void);


/**
  * @brief Thread Two 
  * @param  None    
  * @retval None
  */
void ThreadTwo_x(void);


/** 
  * @brief  Application Delay function for avoiding context switch
  *         otherwise use _tx_thread_sleep()
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay);



/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __TASKS_H */
