/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Mảng chứa danh sách chân Hàng và Cột cho bàn phím 2x2 (Port A) */
uint16_t ROW_PINS[2] = {GPIO_PIN_0, GPIO_PIN_1};
uint16_t COL_PINS[2] = {GPIO_PIN_2, GPIO_PIN_3};

/* Mảng lưu danh sách chân điều khiển 4 LED (Port B) */
uint16_t LED_PINS[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* Prototype hàm kiểm tra trạng thái nút nhấn ma trận */
uint8_t KEYPAD_2x2_Scan(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Hàm kiểm tra nút nhấn ma trận phím 2x2
  * @retval Giá trị phím được bấm (1 đến 4), trả về 0 nếu thả phím
  */
uint8_t KEYPAD_2x2_Scan(void) {
    for (uint8_t r = 0; r < 2; r++) {
        // Kích hoạt từng hàng bằng cách đưa về mức LOW (0)
        for (uint8_t i = 0; i < 2; i++) {
            HAL_GPIO_WritePin(GPIOA, ROW_PINS[i], (i == r) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
        HAL_Delay(1); // Trễ ngắn để đường truyền ổn định
        
        // Quét tín hiệu phản hồi trên các cột
        for (uint8_t c = 0; c < 2; c++) {
            if (HAL_GPIO_ReadPin(GPIOA, COL_PINS[c]) == GPIO_PIN_RESET) {
                HAL_Delay(20); // Chờ hết dội phím (Anti-bounce)
                if (HAL_GPIO_ReadPin(GPIOA, COL_PINS[c]) == GPIO_PIN_RESET) {
                    return (r * 2 + c) + 1; // Trả về số thứ tự nút bấm (1..4)
                }
            }
        }
    }
    return 0; // Trả về 0 nếu không phát hiện thao tác bấm
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Kiểm tra xem phím nào đang được giữ/bấm
    uint8_t key = KEYPAD_2x2_Scan();
    
    // Đưa tất cả 4 LED về trạng thái tắt
    HAL_GPIO_WritePin(GPIOB, LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3], GPIO_PIN_RESET);
    
    // Bật LED tương ứng với mã nút bấm
    if (key >= 1 && key <= 4) {
        HAL_GPIO_WritePin(GPIOB, LED_PINS[key - 1], GPIO_PIN_SET);
    }
  /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  /* Kích hoạt Clock cho các Cổng A, B, C và ngoại vi AFIO */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE(); // Cấp Clock cho khối AFIO hỗ trợ Remap chân

  /* Disable tính năng JTAG để sử dụng PB3 và PB4 như các chân I/O chuẩn (Vẫn giữ SWD để nạp code) */
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  // Thiết lập 2 chân Hàng (PA0, PA1) làm Output Push-Pull
  GPIO_InitStruct.Pin = ROW_PINS[0] | ROW_PINS[1];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Thiết lập 2 chân Cột (PA2, PA3) làm Input kéo lên (Pull-up)
  GPIO_InitStruct.Pin = COL_PINS[0] | COL_PINS[1];
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Thiết lập 4 chân xuất tín hiệu LED (PB0, PB1, PB3, PB4) làm Output Push-Pull
  HAL_GPIO_WritePin(GPIOB, LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3], GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */