/**
 * @file main.c
 * @brief Program to output different kinds of waves to DAC.
 *
 * This program initializes the USER Button using GPIO
 */
#include "main.h"

/* Global variables */
DAC_HandleTypeDef DacHandle; /* DAC handle */
static DAC_ChannelConfTypeDef sConfig; /* DAC channel configuration */
const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF}; /* Escalator 8-bit values */
__IO uint8_t ubSelectedWavesForm = 1; /* User-selected waveform */
__IO uint8_t ubKeyPressed = SET; /* Key pressed status */

/* Function prototypes */
static void DAC_Ch1_TriangleConfig(void); /* Configure DAC channel 1 for triangle wave */
static void DAC_Ch1_EscalatorConfig(void); /* Configure DAC channel 1 for escalator wave */
static void TIM6_Config(void); /* Configure Timer 6 */
static void SystemClock_Config(void); /* Configure the system clock */
static void Error_Handler(void); /* Error Handler */

int main(void)
{
  HAL_Init(); /* Initialize the HAL Library */
  SystemClock_Config(); /* Configure the System Clock */
  BSP_LED_Init(LED3); /* Initialize LED3 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI); /* Initialize User Button */

  DacHandle.Instance = DACx; /* DAC instance */
  TIM6_Config(); /* Configure Timer 6 */

  /* Infinite loop */
  while (1)
  {
    if (ubKeyPressed != RESET)
    {
      HAL_DAC_DeInit(&DacHandle); /* De-initialize the DAC */
      if (ubSelectedWavesForm == 1)
      {
        DAC_Ch1_TriangleConfig(); /* Configure DAC for triangle wave */
      }
      else
      {
        DAC_Ch1_EscalatorConfig(); /* Configure DAC for escalator wave */
      }
      ubKeyPressed = RESET; /* Reset key pressed status */
    }
  }
}


/**
 * @brief  System Clock Configuration
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
  /* Local variables */
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif /* USE_STM32469I_DISCO_REVA */
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}
/**
 * @brief  Error Handler: This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
  BSP_LED_On(LED3); // Turn on the LED3 to indicate error

  while(1) // Infinite loop to halt the system
  {
  }
}

/**
 * @brief  Configures the DAC channel 1 to generate an escalator waveform.
 * @param  None
 * @retval None
 */
static void DAC_Ch1_EscalatorConfig(void)
{
  if (HAL_DAC_Init(&DacHandle) != HAL_OK) // Initialize DAC
  {
    Error_Handler(); // Handle errors
  }

  // Configure DAC settings
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK) // Configure DAC channel
  {
    Error_Handler(); // Handle errors
  }

  // Start DAC with DMA (Direct Memory Access)
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 6, DAC_ALIGN_8B_R) != HAL_OK)
  {
    Error_Handler(); // Handle errors
  }
}

/**
 * @brief  Configures the DAC channel 1 to generate a triangle waveform.
 * @param  None
 * @retval None
 */
static void DAC_Ch1_TriangleConfig(void)
{
  if (HAL_DAC_Init(&DacHandle) != HAL_OK) // Initialize DAC
  {
    Error_Handler(); // Handle errors
  }

  // Configure DAC settings
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK) // Configure DAC channel
  {
    Error_Handler(); // Handle errors
  }

  // Generate triangle waveform
  if (HAL_DACEx_TriangleWaveGenerate(&DacHandle, DACx_CHANNEL, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
  {
    Error_Handler(); // Handle errors
  }
  
  // Start the DAC
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL) != HAL_OK)
  {
    Error_Handler(); // Handle errors
  }

  // Set the DAC value
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL, DAC_ALIGN_12B_R, 0x100) != HAL_OK)
  {
    Error_Handler(); // Handle errors
  }
}

/**
 * @brief  GPIO EXTI Callback: Handles the external interrupt callback.
 * @param  GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  ubKeyPressed = 1; // Update the key pressed status
  ubSelectedWavesForm = !ubSelectedWavesForm; // Toggle the waveform selection
}

/**
 * @brief  Configures Timer 6.
 * @param  None
 * @retval None
 */
void TIM6_Config(void)
{
  static TIM_HandleTypeDef htim; // Timer handle
  TIM_MasterConfigTypeDef sMasterConfig;

  htim.Instance = TIM6; // Timer 6 instance

  // Timer initialization settings
  htim.Init.Period            = 0x7FF;
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = 0;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim); // Initialize Timer 6

  // Timer master configuration settings
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig); // Configure Timer 6 master

  HAL_TIM_Base_Start(&htim); // Start Timer 6
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif


/**
  * @}
  */

/**
  * @}
  */
