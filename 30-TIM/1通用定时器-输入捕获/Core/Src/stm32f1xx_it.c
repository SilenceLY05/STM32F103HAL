/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_usart.h"
#include "bsp_GeneralTim_tim.h"
#include "bsp_led.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
void GENERAL_TIM_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBase);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{
	TIM_ICUserValueStructure.usPeriod++;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef*htim)
{
	TIM_IC_InitTypeDef IC_Config;
	
	if(TIM_ICUserValueStructure.ucStartFlag == 0)
	{
		LED1_TOGGLE;
		
		__HAL_TIM_SET_COUNTER(htim,0);    //清零定时器计数
		TIM_ICUserValueStructure.usPeriod = 0;
		TIM_ICUserValueStructure.usCtr = 0;
		
		//配置输入捕获参数，主要是修改触发电平
		IC_Config.ICPolarity = GENERAL_TIM_END_ICPolarity;
		IC_Config.ICFilter = 0;
		IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
		IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
		
		HAL_TIM_IC_ConfigChannel(&TIM_TimeBase,&IC_Config,GENERAL_TIM_CHANNELx);
		
		//清除中断标志位
		__HAL_TIM_CLEAR_IT(htim,GENERAL_TIM_IT_CCx);
		
		//启动输入捕获并开启中断
		HAL_TIM_IC_Start_IT(&TIM_TimeBase,GENERAL_TIM_CHANNELx);
		
		TIM_ICUserValueStructure.ucStartFlag = 1;
	}
	else
	{
		LED1_TOGGLE;
		
		TIM_ICUserValueStructure.usCtr = HAL_TIM_ReadCapturedValue(&TIM_TimeBase,GENERAL_TIM_CHANNELx);
		
		//配置输入捕获参数，主要是修改触发电平
		IC_Config.ICPolarity = GENERAL_TIM_START_ICPolarity;
		IC_Config.ICFilter = 0;
		IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
		IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
		
		HAL_TIM_IC_ConfigChannel(&TIM_TimeBase,&IC_Config,GENERAL_TIM_CHANNELx);
		
		//清除中断标志位
		__HAL_TIM_CLEAR_IT(htim,GENERAL_TIM_IT_CCx);
		
		//启动输入捕获并开启中断
		HAL_TIM_IC_Start_IT(&TIM_TimeBase,GENERAL_TIM_CHANNELx);
		
		TIM_ICUserValueStructure.ucStartFlag = 0;
		TIM_ICUserValueStructure.ucFinishing = 1;
	}
}

/* USER CODE END 1 */
