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
#include "bsp_breathlight.h"


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
extern __IO uint32_t rgb_color;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t pwm_index = 1;       /* 用于PWM查表 */
	static uint16_t period_cnt = 0;      /* 用于计算周期数 */
	static uint16_t amplitude_cnt = 0;   /* 用于计算幅值等级 */
	
	amplitude_cnt++;
	
	//每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
		 //每增加一级多输出一次脉冲，即PWM表中的元素多使用一次
		 //使用256次，根据RGB颜色分量设置通道输出
	if(amplitude_cnt > (AMPLITUDE_CLASS-1))
	{
		period_cnt++;
	
		/* 若输出的周期数大于20，输出下一种脉冲宽的PWM波 */
		if(period_cnt >=20)
		{
			/* 根据PWM表修改定时器的比较寄存器值 */
			__HAL_TIM_SET_COMPARE(&TIM_Handle,TIM_CHANNEL_3,indexWave[pwm_index]);
			
			/* 标志PWM表的下一个元素 */
			pwm_index++;												
			/* 若PWM脉冲表已经输出完成一遍，重置PWM查表标志 */
			if( pwm_index >=  POINT_NUM)								
			{
				pwm_index=0;								
			}
			/* 重置周期计数标志 */
			period_cnt=0;	
		}
		amplitude_cnt = 0;    //重置幅值技术标志
	}
	else
	{
		//每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
			//每增加一级多输出一次脉冲，即PWM表中的元素多使用一次

			//根据RGB颜色分量值，设置各个通道是否输出当前的PWM表元素表示的亮度
			//红
			if(((rgb_color&0xFF0000)>>16) >= amplitude_cnt )
				BRE_TIMx->CCR2 = indexWave[pwm_index];   //根据PWM表修改定时器的比较值寄存器
			else
				BRE_TIMx->CCR2 = 0;   //根据PWM表修改定时器的比较值寄存器
			
			//绿
			if(((rgb_color&0x00FF00)>>8) >= amplitude_cnt )
				BRE_TIMx->CCR3 = indexWave[pwm_index];   //根据PWM表修改定时器的比较值寄存器
			else
				BRE_TIMx->CCR3 = 0;   //根据PWM表修改定时器的比较值寄存器
			
			//蓝
			if((rgb_color&0x0000FF) >= amplitude_cnt )
				BRE_TIMx->CCR4 = indexWave[pwm_index];   //根据PWM表修改定时器的比较值寄存器
			else
				BRE_TIMx->CCR4 = 0;   //根据PWM表修改定时器的比较值寄存器
	}
	
	
}

void BRE_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TIM_Handle);
}



/* USER CODE END 1 */
