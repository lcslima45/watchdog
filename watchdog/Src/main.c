/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
	* @Author					: Dhannyell Lukas
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Src/lcd/pcd8544.h"
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//coment to CPU GRAPH
#define CLOCK_GRAPH

#define MIN_CLOCK 400
#define MAX_CLOCK 2900
#define NUM_VAL (21)

uint8_t rx_data[60];

int enable_iterface = 1;

int cpuClock=0;
char cpuclock[4];

int Temp;
char temp[2];

char load[3];
int Load = 0;

int en, st;
int inp = 0;

//UART CALLBACK FUNCTION DMA
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Receive_DMA(&huart1, rx_data, sizeof(rx_data));
}

void data_schemaSPI(){
	gotoXY(0,2);
	LCDString("Ram:");
	gotoXY(0,3);
	LCDString("Cpu:");
	gotoXY(0,4);
	LCDString("Temp:");
	gotoXY(0,5);
	LCDString("Clock:");
}

int v,i,ght=16;
int selector = 0;
int valTab[NUM_VAL];
int tempTab[NUM_VAL];
int loadTab[NUM_VAL];

//draw clock graph
void drawGraphBar()
{
  drawLineH2(0,83,0,2);
  drawLineH2(0,83,15,2);
  drawLineV2(0,0,15,2);
  drawLineV2(83,0,15,2);
  for(i=0;i<NUM_VAL;i++) {
    drawLineH2(i*4,(i+1)*4-1,15-valTab[i],1);
    drawLineV2(i*4+0,15-valTab[i],15,2);
    drawLineV2(i*4+1,15-valTab[i],15,2);
    drawLineV2(i*4+2,15-valTab[i],15,2);
    drawLineV2(i*4+3,15-valTab[i],15,2);
    if(i>0) drawLineV2(i*4,15-valTab[i-1],15-valTab[i],1);
  }
}

//draw temp graph
void drawGraphBar2()
{
  drawLineH2(0,83,6,2);
  drawLineH2(0,83,21,2);
  drawLineV2(0,6,21,2);
  drawLineV2(83,6,21,2);
  for(i=0;i<NUM_VAL;i++) {
    drawLineH2(i*4,(i+1)*4-1,21-tempTab[i],1);
    drawLineV2(i*4+0,21-tempTab[i],21,2);
    drawLineV2(i*4+1,21-tempTab[i],21,2);
    drawLineV2(i*4+2,21-tempTab[i],21,2);
    drawLineV2(i*4+3,21-tempTab[i],21,2);
    if(i>0) drawLineV2(i*4,21-tempTab[i-1],21-tempTab[i],1);
  }
}

//draw load graph
void drawGraphBar3()
{
  drawLineH2(0,83,26,2);
  drawLineH2(0,83,41,2);
  drawLineV2(0,26,41,2);
  drawLineV2(83,26,41,2);
  for(i=0;i<NUM_VAL;i++) {
    drawLineH2(i*4,(i+1)*4-1,41-loadTab[i],1);
    drawLineV2(i*4+0,41-loadTab[i],41,2);
    drawLineV2(i*4+1,41-loadTab[i],41,2);
    drawLineV2(i*4+2,41-loadTab[i],41,2);
    drawLineV2(i*4+3,41-loadTab[i],41,2);
    if(i>0) drawLineV2(i*4,41-loadTab[i-1],41-loadTab[i],1);
  }
}

//function update clock array used to draw graph
void addVal()
{
  for(int i=0;i<NUM_VAL-1;i++) valTab[i]=valTab[i+1];
#ifdef CLOCK_GRAPH
  if(cpuClock<400) cpuClock=400;
  if(cpuClock>2900) cpuClock=2900;
  valTab[NUM_VAL-1] = (long)(cpuClock-MIN_CLOCK)*ght/(MAX_CLOCK-MIN_CLOCK);
#else
  valTab[NUM_VAL-1] = Load*ght/100;
#endif
}

//add values to generate temperature graphic
void addTemp()
{
  for(int i=0;i<NUM_VAL-1;i++) tempTab[i]=tempTab[i+1];
  tempTab[NUM_VAL-1] = Temp*ght/100;
}

//function update load array used to draw graph
void addLoad()
{
  for(int i=0;i<NUM_VAL-1;i++) loadTab[i]=loadTab[i+1];
  loadTab[NUM_VAL-1] = Load*ght/100;
}

//port to substring function arduino to stm32
int substring(char *source, int from, int n, char *target){
    int length,i;
    //get string length 
    for(length=0;source[length]!='\0';length++);
          
    if((from+n)>length){
        //get substring till end
        n=(length-from);
    }
    //get substring in target
    for(i=0;i<n;i++){
        target[i]=source[from+i];
    }
    target[i]='\0'; //assign null at last
     
    return 0;    
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	//Call the Callback function on DMA
	HAL_UART_Receive_DMA(&huart1, rx_data, sizeof(rx_data));
	
	//LCD SPI INIT CONFIG
	LcdInitialise();
	LcdClear();
		
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		for(int j=0;j<sizeof(rx_data);j++){
			//Check if any data is receive.
			if(rx_data[0] == 0x00){
				gotoXY(0,2);
				LCDString("Conectando..");
				LcdClearBuffer();
				LcdClear();
			//if data receive load main interface
			}else if(enable_iterface == 1){
				data_schemaSPI();
				enable_iterface = 0;
			//if button 1 is pressed load secudary interface
			}else if(enable_iterface == 2){
				gotoXY(11,0);
				LCDString("Temp/Load");
				en = strcspn(rx_data,"Y");
				st = strcspn(rx_data,"K");
				
				if(en >= 0){
				substring(rx_data,en+1,2,temp);
				Temp = atoi(temp);
				inp = 3;
				}
				if(st >=0){
					substring(rx_data, st+2,3,load);
					Load = atoi(load);
				}
				if(inp == 3){
					addTemp();
					addLoad();
					LcdClearBuffer();	
					drawGraphBar2();
					drawGraphBar3();
					PCD8544_UpdateArea(80,40,80,40);
				}
				
				if(inp>=3) { HAL_Delay(1000); inp=0; PCD8544_Refresh();}
				//if button 1 is pressed again it returns to the main interface
				if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)){
					HAL_Delay(20);
					LcdClearBuffer();
					LcdClear();
					enable_iterface = 1;
				}
			//if button 3 is pressed load config menu
			}else if(enable_iterface == 3){
				//switch to control cursor
				switch(selector){
					case 0:
							LcdClear();
							gotoXY(0,0);
							LCDString(">");
							break;
					case 1:
						  LcdClear();
							gotoXY(0,1);
							LCDString(">");
							break;
					case 2:
							LcdClear();
							gotoXY(0,2);
							LCDString(">");
							break;
					case 3:
							LcdClear();
							gotoXY(0,3);
							LCDString(">");
							break;
					default:
						if(selector > 2)selector = 0;
						if(selector < 0) selector = 2;
				}
				gotoXY(9, 0);
				LCDString("Zen Mode");
				gotoXY(9, 1);
				LCDString("Turbo Mode");
				gotoXY(9, 2);
				LCDString("Contrast");
				gotoXY(9, 3);
				LCDString("Exit");
				//if button 3 is pressed update cursor on screen
				if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)){
					HAL_Delay(200);
					selector = selector + 1; 
				//if button 4 pressed update cursor on screen
				}else if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
					HAL_Delay(200);
					selector = selector - 1;
				//if button 2 is pressed take a action based in cursor value
				}else if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)){
					HAL_Delay(200);
					switch(selector){
						case 0:
							HAL_UART_Transmit(&huart1,"Zen Mode\n", 9, 100);
							break;
						case 1:
							HAL_UART_Transmit(&huart1,"Turbo Mode\n", 12, 100);
							break;
						case 2:
								HAL_UART_Transmit(&huart1,"Contrast\n", 9, 100);
								break;
						case 3:
							enable_iterface = 1;
							break;
					}
				}
			//update data to main interface
			}else{
				gotoXY(27,2);
				en = strcspn(rx_data,"R");
				LCDCharacter(rx_data[en+1]);
				LCDCharacter(rx_data[en+2]);
				LCDCharacter(rx_data[en+3]);
				LCDString("GB");
				
				gotoXY(27,3);
				en = strcspn(rx_data,"K");
				substring(rx_data,en+2,3,load);
				LCDCharacter(load[0]);
				LCDCharacter(load[1]);
				if(load[2] == 0x7C)load[2] = 0x20;
				Load = atoi(load);
				LCDCharacter(load[2]);
				
				gotoXY(35,4);
				en = strcspn(rx_data,"Y");
				substring(rx_data,en+1,2,temp);
				LCDCharacter(temp[0]);
				LCDCharacter(temp[1]);
				Temp = atoi(temp);
				LCDString("C");
				
				gotoXY(40,5);
				en = strcspn(rx_data,"J");
				st = strcspn(rx_data,"\\");
				if(en >= 0){
					substring(rx_data,en+1,4,cpuclock);
					LCDCharacter(cpuclock[0]);
					LCDCharacter(cpuclock[1]);
					LCDCharacter(cpuclock[2]);
					if(cpuclock[3] == 0x5C)cpuclock[3] = 0x20;
					LCDCharacter(cpuclock[3]);
					LCDString("Hz");
					inp = 3;
					cpuClock = atoi(cpuclock);
				}
			
				if(inp == 3){
					addVal();
					LcdClearBuffer();	
					drawGraphBar();
					//HAL_Delay(20000);
					PCD8544_UpdateArea(40,10,40,10);
					//PCD8544_Refresh();
				}
				if(inp>=3) { HAL_Delay(1000); inp=0; PCD8544_Refresh();}
			  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)){
					HAL_Delay(10);
					LcdClearBuffer();
					LcdClear();
					enable_iterface = 2;
				}else if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)){
					HAL_Delay(100);
					LcdClearBuffer();
					LcdClear();
					enable_iterface = 3;
				}
			}
				
		}
}
    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 128000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
