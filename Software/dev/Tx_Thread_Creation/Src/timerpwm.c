/**
 ****************************************************************
 @file    timerPWM.c
 ****************************************************************
 @brief   This module offers a set of functions to handle pwm outputs by
 @brief   timer peripherals.
 ****************************************************************
 @author  Christian Weidmann
 @version 0
 @date    05.04.2025
 ****************************************************************
 */

// --- Includes
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "timerpwm.h"


// --- Typedefs

// --- Local Function Prototypes
void _hal_timerPWM_dutySet(timerPWMPeripheral_t pwm, uint32_t partOfTousend);

// --- Variables
volatile uint32_t RegisterVal; // Capture compare register-value
volatile uint32_t ARRVal; // auto reload register-value 


/**
 ****************************************************************
 @brief  Start all PWM's
 @param  pwmtimer timer which outputs the pwm signals
 @param  arr struct with pwm parameters
 @return -
 ****************************************************************
 */
HAL_StatusTypeDef hal_timerPWM_start(TIM_HandleTypeDef* servotimer, TIM_HandleTypeDef* steppertimer, pwmSettings_t* arr)
{



  for (int i = 0; i < TIMERPWM_LAST_PERIPHERIE; i++)
  {
    switch(arr[i].peripherie)
    {

    case TIMERPWM_SERVO_0:

      if (HAL_OK != HAL_TIM_PWM_Start(servotimer, TIM_CHANNEL_1))
      {
        Error_Handler();
      }

    break;

    case TIMERPWM_SERVO_1:

      if (HAL_OK != HAL_TIM_PWM_Start(servotimer, TIM_CHANNEL_2))
      {
        Error_Handler();  
      } 

      break;

    case TIMERPWM_SERVO_2:

      if (HAL_OK != HAL_TIM_PWM_Start(servotimer, TIM_CHANNEL_3))
      {
        Error_Handler();
      } 

      break;

    case TIMERPWM_SERVO_3:
      //set PWM period (SERVO_3)

      break;
    case TIMERPWM_SERVO_4:
       //servo 4 is started with the steppertimer

        break;
    case STEPPER_1:

      if (HAL_OK != HAL_TIM_PWM_Start(steppertimer, TIM_CHANNEL_2))
      {
        Error_Handler();
      } 

    break;
    
    case STEPPER_2:
       //set PWM period (STEPPER_2)

      break;

    default:
      break;

    }
  }

  return HAL_OK;
  
}


/**
 ****************************************************************
 @brief  Disable all PWM-Outputs.
 @param  pwm   One of the possible timerPWM from enumeration
 @return -
 ****************************************************************
 */
void hal_timerPWM_stop(TIM_HandleTypeDef* pwmtimer)
{
  (void)pwmtimer;
}


/**
 ****************************************************************
 @brief  sweep duty cycle between min and max value
 @param  pwmtimer  Timer which outputs the pwm signal
 @return -
 ****************************************************************
 */
void hal_sweep(TIM_HandleTypeDef* pwmtimer, pwmSettings_t* pwm)
{

  
  // get capture compare value
  switch(pwm->peripherie)
  {

    case TIMERPWM_SERVO_0:
    RegisterVal = pwmtimer->Instance->CCR1;
    break;

    case TIMERPWM_SERVO_1:
    RegisterVal = pwmtimer->Instance->CCR2;
    break;

    case TIMERPWM_SERVO_2:
    RegisterVal = pwmtimer->Instance->CCR3;
    break;

    case TIMERPWM_SERVO_3:
    break;

    case TIMERPWM_SERVO_4:
    break;

    case STEPPER_1:
    //set PWM period (STEPPER_1)
    RegisterVal = pwmtimer->Instance->ARR;
    break;

    case STEPPER_2:
    //set PWM period (STEPPER_2)
    break;

    default:
    break;
  }



  if (pwm->countUp == true)
  {
    if (RegisterVal >= pwm->maxVal - pwm->stepSize)
    {
      pwm->countUp = false;
      RegisterVal = pwm->maxVal;
    }

    else 
    {
      RegisterVal += pwm->stepSize;
    }
  }

  else
  {
    if (RegisterVal <= pwm->minVal + pwm->stepSize)
    {
      pwm->countUp = true;
      RegisterVal = pwm->minVal;
    }
    else 
    {
      RegisterVal -= pwm->stepSize;
    }
  }


  if (pwm->peripherie < (TIMERPWM_LAST_PERIPHERIE - NUM_OF_STEPPERS))
  {
    hal_timerPWM_dutySet(pwmtimer, pwm->peripherie, RegisterVal);
  }

  else
  {
    hal_timerPWM_periodSet(pwmtimer,pwm->peripherie, RegisterVal);
  }



}


/**
 ****************************************************************
 @brief  Set period of a single PWM.
 @param  pwm   One of the possible timerPWM from enumeration
 @param  usPeriod  Set period of PWM Timer.
 @return -
 ****************************************************************
 */
void hal_timerPWM_periodSet(TIM_HandleTypeDef* pwmtimer, timerPWMPeripheral_t pwm, uint16_t period)
{

  // get ARR value (period)
  switch(pwm)
  {

    case STEPPER_1:
    pwmtimer->Instance->ARR = period;
      break; 

    case STEPPER_2:
       //set PWM period (STEPPER_2)

      break;

    default:
      break;
  }

}

/**
 ****************************************************************
 @brief  Set the PWM-duty to the corresponding output [�].
 @param  pwm   One of the possible timerPWM from enumeration
 @param  partOfTousend A number between 0 and 1000
 @return -
 ****************************************************************
 */
void hal_timerPWM_dutySet(TIM_HandleTypeDef* pwmtimer, timerPWMPeripheral_t pwm, uint32_t CCRVal)
{

  switch(pwm)
  {
    case TIMERPWM_SERVO_0:
    pwmtimer->Instance->CCR1 = CCRVal;
    break;

    case TIMERPWM_SERVO_1:
    pwmtimer->Instance->CCR2 = CCRVal;
    break;

    case TIMERPWM_SERVO_2:
    pwmtimer->Instance->CCR3 = CCRVal;
    break;

    case TIMERPWM_SERVO_3:
    break;

    case TIMERPWM_SERVO_4:
    break;

    case STEPPER_1:
    //set PWM period (STEPPER_1)
    break;

    case STEPPER_2:
    //set PWM period (STEPPER_2)
    break;

    default:
    break;
  }



}

/**
 ****************************************************************
 @brief  Set the PWM match register.
 @param  pwm   One of the possible timerPWM from enumeration
 @param  partOfTousend A number between 0 and 1000
 @return -
 ****************************************************************
 */
void _hal_timerPWM_dutySet(timerPWMPeripheral_t pwm, uint32_t partOfTousend)
{
  uint32_t matchValue = 0;
  uint32_t period = 0;

  if(matchValue == period)
  {
    //invert output
    switch(pwm)
    {
      case TIMERPWM_SERVO_0:
        break;
      case TIMERPWM_SERVO_1:

        break;
      case TIMERPWM_SERVO_2:

        break;
      case TIMERPWM_SERVO_3:

        break;
      case TIMERPWM_SERVO_4:

        break;
      case STEPPER_1:

        break;
      case STEPPER_2:

      break;

      default:  
      break;
    }

  }

}

void hal_timerPWM_hold(timerPWMPeripheral_t pwm, bool enable)
{
  switch(pwm)
  {


    case TIMERPWM_SERVO_0:

    break;

    default:
    break;
  }

}
