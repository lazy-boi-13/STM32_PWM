/*
  ******************************************************************************
  * @file    tasks.h
  * @brief   This file contains the headers of the callback functions.
  ******************************************************************************
*/

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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASKS_H
#define __TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdbool.h"




void MainThread(UART_HandleTypeDef* huart, TIM_HandleTypeDef* timer, ADC_HandleTypeDef* adc);


/**
  * @brief Thread to control the servo motors 
  * 
  * initialize pwm config
  * calculate/modify duty cycle according to configuration
  * @param  None    
  * @retval None
  */
 void ServoControl(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef** timers);


/**
  * @brief Thread d
  * @param  hadcs
  * @retval None
  */
void ExtraThread(void);



/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __TASKS_H */
