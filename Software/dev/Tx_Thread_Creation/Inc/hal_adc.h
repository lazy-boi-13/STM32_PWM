/**
 ****************************************************************
 @file    hal_adc.h
 ****************************************************************
 @brief   Initialize the ADC
 ****************************************************************
 @author  Christian Weidmann
 @version 0
 @date    05.04.2025
 ****************************************************************
 */
#ifndef HAL_TIMERPWM_H_
#define HAL_TIMERPWM_H_

// --- Includes
#include "stm32h7xx_hal_def.h"


// --- Defines


// --- Typedefs




// --- Function Prototypes
HAL_StatusTypeDef hal_adc_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* timer, uint32_t* adc_buf);


/*  */
#endif /* HAL_TIMERPWM_H_ */