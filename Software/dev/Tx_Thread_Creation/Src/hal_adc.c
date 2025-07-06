/**
 ****************************************************************
 @file    hal_adc.c
 ****************************************************************
 @brief   This module offers a function to initialize the adc
 ****************************************************************
 @author  Christian Weidmann
 @version 0
 @date    05.04.2025
 ****************************************************************
 */

// --- Includes
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "hal_adc.h"


// --- Typedefs

// --- Local Function Prototypes

// --- Variables


/**
 ****************************************************************
 @brief  calibrate the adc and start transfering data into buffer  
 @param  hadc the adc
 @param  timer timer which triggers an adc conversion
 @param  adc_buf buffer 
 @param  
 @return - 
 ****************************************************************
 */
HAL_StatusTypeDef hal_adc_init(ADC_HandleTypeDef* hadc, TIM_HandleTypeDef* timer, uint32_t* adc_buf)
{


    if (HAL_OK != HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED))   // calibrate the adc
    {
      Error_Handler();
    } 
    
    if (HAL_OK != HAL_ADC_Start_DMA(hadc, (uint32_t*) &adc_buf, 2))   // start the ADC conversion over DMA in circular mode
    {
      Error_Handler();
    } 
  
    HAL_TIM_Base_Start(timer); // start the timer that triggers ADC Conversions


    return HAL_OK;

}
