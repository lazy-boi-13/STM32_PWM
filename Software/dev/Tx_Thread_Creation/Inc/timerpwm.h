/**
 ****************************************************************
 @file    timerpwm.h
 ****************************************************************
 @brief   This module offers a set of functions to handle pwm outputs by
 @brief   timer peripherals.
 ****************************************************************
 @author  Christian Weidmann
 @version 01.011
 @date    23.03.2025
 ****************************************************************
 */
#ifndef HAL_TIMERPWM_H_
#define HAL_TIMERPWM_H_

// --- Includes
#include <stdint.h>


// --- Defines
#define DEFAULT_PERIOD_US         500


// --- Typedefs
//! A enumeration which represents all PWM peripherals
typedef enum
{
  TIMERPWM_SERVO_0 = 0, 
  TIMERPWM_SERVO_1,    
  TIMERPWM_SERVO_2,   
  TIMERPWM_SERVO_3,    
  TIMERPWM_SERVO_4,    
  STEPPER_1,        
  STEPPER_2,
  TIMERPWM_LAST_PERIPHERIE
} timerPWMPeripheral_t;


typedef struct
{
    uint16_t periodTimeUs;
    uint32_t stepSize;
    uint32_t duty;
    bool countUp;
    uint16_t maxVal;    // max Duty Cycle in parts per thousand
    uint16_t minVal;    // min. Duty Cycle in parts per thousand
    uint16_t center;    // unmapped ADC Value for center position
    const timerPWMPeripheral_t peripherie;

} pwmSettings_t;

// --- Function Prototypes
HAL_StatusTypeDef hal_timerPWM_start(TIM_HandleTypeDef* pwmtimer);
void hal_timerPWM_stop(TIM_HandleTypeDef* pwmtimer);
void hal_sweep(TIM_HandleTypeDef* pwmtimer, uint16_t maxVal, uint16_t minVal, uint16_t stepSize);



// in progress...
void hal_timerPWM_periodSet(timerPWMPeripheral_t pwm, uint32_t usPeriod);
void hal_timerPWM_dutySet(TIM_HandleTypeDef* pwmtimer, timerPWMPeripheral_t pwm, uint32_t CCRVal);
void hal_timerPWM_hold(timerPWMPeripheral_t pwm, bool enable);

#endif /* HAL_TIMERPWM_H_ */