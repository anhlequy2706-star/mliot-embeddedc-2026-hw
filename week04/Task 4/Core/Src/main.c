/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/**
  * @brief  Main program entry point for Task 4
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  uint8_t last_button_state = GPIO_PIN_SET; // Mặc định Pull-up là HIGH (1)

  while (1)
  {
    /* USER CODE BEGIN 3 */
    uint8_t current_button_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    // Phát hiện sườn xuống (khi nút được BẤM: từ HIGH -> LOW)
    if (last_button_state == GPIO_PIN_SET && current_button_state == GPIO_PIN_RESET)
    {
      HAL_Delay(20); // Chống dội phím (Debounce delay 20ms)
      if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
      {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Đảo trạng thái LED PC13
      }
    }

    last_button_state = current_button_state;
    /* USER CODE END 3 */
  }
}

/**
  * @brief Cấu hình GPIO (PC13 - LED, PA0 - Nút bấm)
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Bật Clock cho PORT C và PORT A */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* 1. Cấu hình chân PC13 làm Output cho LED */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Tắt LED mặc định
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* 2. Cấu hình chân PA0 làm Input Pull-up cho Nút bấm */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP; // Kéo lên 3.3V nội bộ
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief Cấu hình Clock hệ thống
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Hàm xử lý lỗi
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}