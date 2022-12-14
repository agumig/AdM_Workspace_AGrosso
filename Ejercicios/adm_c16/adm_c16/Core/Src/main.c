/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "string.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
    /* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}
static void PrivilegiosSVC (void)
{
	// Obtiene valor del registro de 32 bits del procesador llamado "control".
	// El registro guarda los siguientes estados:
	// bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
	// bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
	// bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
	//        Recordar que este valor solo se usa en modo Thread. Las
	//        interrupciones siempre se ejecutan en modo Handler con total
	//        privilegio.
	uint32_t x = __get_CONTROL ();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	x |= 1;
	// bit 0 a modo No privilegiado.
	__set_CONTROL (x);

	// En este punto se estaria ejecutando en modo No privilegiado.
	// Lectura del registro "control" para confirmar.
	x = __get_CONTROL ();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	x &= ~1u;
	// Se intenta volver a modo Privilegiado (bit 0, valor 0).
	__set_CONTROL (x);

	// Confirma que esta operacion es ignorada por estar ejecutandose en modo
	// Thread no privilegiado.
	x = __get_CONTROL ();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	// En este punto, ejecutando en modo Thread no privilegiado, la unica forma
	// de volver a modo privilegiado o de realizar cualquier cambio que requiera
	// modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
	// opertivo de tiempo real.
	// Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
	// utilizando la instruccion "svc".
	// No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
	// es necesario implementar una funcion en assembler. Ver el archivo
	// asm_func.S.
	asm_svc ();

	// El sistema operativo (el handler de SVC) deberia haber devuelto el modo
	// de ejecucion de Thread a privilegiado (bit 0 en valor 0).
	x = __get_CONTROL ();

	// Fin del ejemplo de SVC
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{


	/* USER CODE BEGIN 1 */
	/*  Creo vectores de prueba */

	uint32_t vectorIn_uint32[LEN] = {1,2,3,4,5,6,7,8,23,10,11,12,13,14,15,16,17,18,19};
	uint32_t vectorOut_uint32[LEN]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

	int32_t vectorIn_int32[LEN] = {131021,1310210,3,4,5,5096,7,8,9,10,11,12,13,14,15,16,17,18,19};
	int32_t vectorOut_int32[LEN]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

	uint16_t vectorIn_uint16[LEN] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	uint16_t vectorOut_uint16[LEN]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

	int16_t vectorIn_int16[LEN_MUESTRAS];
	int16_t vectorOut_int16[LEN_MUESTRAS];
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	// Se activa el contado de ciclos
	DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ETH_Init();
	MX_USART3_UART_Init();
	MX_USB_OTG_FS_PCD_Init();
	/* USER CODE BEGIN 2 */
	PrivilegiosSVC ();

	/* Secci??n de pruebas mediante debugging */
	/*
	asm_zeros(vectorOut_uint32, LEN);
	asm_productoEscalar32 (vectorIn_uint32, vectorOut_uint32, LEN, ESCALAR);
	asm_productoEscalar16 (vectorIn_uint16, vectorOut_uint16, LEN, ESCALAR);
	asm_productoEscalar12 (vectorIn_uint16, vectorOut_uint16, LEN, ESCALAR);
	asm_pack32to16 (vectorIn_int32, vectorOut_int16, LEN);
	//int32_t maximo = asm_max ( vectorIn_int32, LEN);
	asm_downsampleM (vectorIn_int32, vectorOut_int32, LEN, DELETE_MAK);
	asm_invertir (vectorIn_uint16, LEN);
	 */
	/*for(uint32_t i = 0; i<LEN_MUESTRAS; i++)
		vectorIn_int16[i]=i;

	asm_eco (vectorIn_int16, vectorOut_int16, LEN_MUESTRAS, LEN_ECO);

	asm_filtroVentana10(vectorIn_uint16, vectorOut_uint16, LEN); */


	/***********************************************************************
			 *		Secci??n de prueba de tiempos de ejecuci??n
			 */

			/*	Eco	*/

			vPrintString("Comienzo de test\n");

			DWT->CYCCNT = 0;
			asm_eco (vectorIn_int16, vectorOut_int16, LEN_MUESTRAS, LEN_ECO);
			const volatile uint32_t ciclos = DWT->CYCCNT;

			vPrintStringAndNumber("\n10 - ECO en ASM: ",ciclos);

			DWT->CYCCNT = 0;
			eco (vectorIn_int16, vectorOut_int16, LEN_MUESTRAS, LEN_ECO);
			const volatile uint32_t ciclos_b = DWT->CYCCNT;

			vPrintStringAndNumber("\n10 - ECO en C: ",ciclos_b);

			//--------------------------------------------------------------------
			/*	Saturaci??n	*/

			DWT->CYCCNT = 0;
			asm_pack32to16(vectorIn_int32, vectorOut_int16, LEN);
			const volatile uint32_t ciclos_c = DWT->CYCCNT;

			vPrintStringAndNumber("\n4 - Saturaci??n ASM : ",ciclos_c);

			DWT->CYCCNT = 0;
			pack32to16(vectorIn_int32, vectorOut_int16, LEN);
			const volatile uint32_t  ciclos_d = DWT->CYCCNT;

			vPrintStringAndNumber("\n4 - Saturaci??n C : ",ciclos_d);

			//--------------------------------------------------------------------
			/*	Filtro de ventana m??vil	*/

			DWT->CYCCNT = 0;
			asm_filtroVentana10(vectorIn_uint16, vectorOut_uint16, LEN);
			const volatile uint32_t ciclos_e = DWT->CYCCNT;

			vPrintStringAndNumber("\n5 - Filtro de ventana m??vil ASM : ",ciclos_e);

			DWT->CYCCNT = 0;
			filtroVentana10(vectorIn_uint16, vectorOut_uint16, LEN);
			const volatile uint32_t ciclos_f = DWT->CYCCNT;

			vPrintStringAndNumber("\n5 - Filtro de ventana m??vil C : ",ciclos_f);




	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ETH Initialization Function
 * @param None
 * @retval None
 */
static void MX_ETH_Init(void)
{

	/* USER CODE BEGIN ETH_Init 0 */

	/* USER CODE END ETH_Init 0 */

	static uint8_t MACAddr[6];

	/* USER CODE BEGIN ETH_Init 1 */

	/* USER CODE END ETH_Init 1 */
	heth.Instance = ETH;
	MACAddr[0] = 0x00;
	MACAddr[1] = 0x80;
	MACAddr[2] = 0xE1;
	MACAddr[3] = 0x00;
	MACAddr[4] = 0x00;
	MACAddr[5] = 0x00;
	heth.Init.MACAddr = &MACAddr[0];
	heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
	heth.Init.TxDesc = DMATxDscrTab;
	heth.Init.RxDesc = DMARxDscrTab;
	heth.Init.RxBuffLen = 1524;

	/* USER CODE BEGIN MACADDRESS */

	/* USER CODE END MACADDRESS */

	if (HAL_ETH_Init(&heth) != HAL_OK)
	{
		Error_Handler();
	}

	memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
	TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
	/* USER CODE BEGIN ETH_Init 2 */

	/* USER CODE END ETH_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void)
{

	/* USER CODE BEGIN USB_OTG_FS_Init 0 */

	/* USER CODE END USB_OTG_FS_Init 0 */

	/* USER CODE BEGIN USB_OTG_FS_Init 1 */

	/* USER CODE END USB_OTG_FS_Init 1 */
	hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
	hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
	hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USB_OTG_FS_Init 2 */

	/* USER CODE END USB_OTG_FS_Init 2 */

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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : USER_Btn_Pin */
	GPIO_InitStruct.Pin = USER_Btn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
	GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_PowerSwitchOn_Pin */
	GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_OverCurrent_Pin */
	GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
 * @brief   Initialize a vector in zeros
 * @param   *vector 	Pointer to the vector
 * @param	longitud 	Vector size
 * @return  None
 */

void zeros (uint32_t * vector, uint32_t longitud)
{
	uint32_t i = 0;

	if (NULL != vector && longitud > 0)
	{
		for(i = 0; i < longitud; i++)
			*(vector+i) = 0;
	}
	else
	{
		Error_Handler();	//exception
	}
}

/**
 * @brief   Multiplies a vector and a scalar in 32 bits
 * @param   *vectorIn 	Pointer to the vector
 * @param   *vectorOut 	Pointer to the vector multiplied
 * @param	longitud 	Vector size
 * @param	escalar 	Scalar value
 * @return  None
 */
void productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
{
	uint32_t i = 0;

	if (NULL != vectorIn && NULL != vectorOut && longitud > 0)
	{
		for(i = 0; i < longitud; i++)
			*(vectorOut+i) = *(vectorIn+i) * escalar;
	}
	else
	{
		Error_Handler();	//exception
	}
}

/**
 * @brief   Multiplies a vector and a scalar in 16 bits
 * @param   *vectorIn 	Pointer to the vector
 * @param   *vectorOut 	Pointer to the vector multiplied
 * @param	longitud 	Vector size
 * @param	escalar 	Scalar value
 * @return  None
 */
void productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
	uint32_t i = 0;

	if (NULL != vectorIn && NULL != vectorOut && longitud > 0)
	{
		for(i = 0; i < longitud; i++)
			*(vectorOut+i) = *(vectorIn+i) * escalar;
	}
	else
	{
		Error_Handler();	//exception
	}
}

/**
 * @brief   Multiplies a vector and a scalar in 12 bits saturated arithmetic
 * @param   *vectorIn 	Pointer to the vector
 * @param   *vectorOut 	Pointer to the vector multiplied
 * @param	longitud 	Vector size
 * @param	escalar 	Scalar value
 * @return  None
 */
void productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
	uint32_t i = 0;

	if (NULL != vectorIn && NULL != vectorOut && longitud > 0)
	{
		for(i = 0; i < longitud; i++)
		{
			if((*(vectorIn+i) * escalar) > SATURACION_12_BITS)
				*(vectorOut+i) = SATURACION_12_BITS;
			else
				*(vectorOut+i) = *(vectorIn+i) * escalar;
		}
	}
	else
	{
		Error_Handler();	//exception
	}
}

/**
 * @brief   10-samples window average filter
 * @param   *vectorIn 	Pointer to the vector
 * @param   *vectorOut 	Pointer to the vector multiplied
 * @param	longitudVectorIn 	Vector size
 * @return  None
 */
void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn)
{
	uint32_t i = 0, delta = 0;
	uint8_t samples = 0;
	uint64_t acum = 0;

	for (i = 0; i < longitudVectorIn; i++)
	{
		for(delta = 1; delta <= SAMPLES_WINDOW/2 ; delta++)
		{
			if((i + delta) < longitudVectorIn)
			{
				acum+=vectorIn[i+delta];
				samples++;
			}
			if((i - delta) >= 0)
			{
				acum+=vectorIn[i-delta];
				samples++;
			}
		}
		vectorOut[i] = acum/samples;
		acum = 0;
		samples = 0;
	}
}

/**
 * @brief   Converts (compressing) 32-bits data to 16-bits data
 * @param   *vectorIn 	Pointer to the input vector
 * @param   *vectorOut 	Pointer to the output vector
 * @param	longitud 	Vector size
 * @return  None
 */
void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud)
{
	uint32_t i = 0;

	for(i = 0; i < longitud; i++)
	{
		*(vectorOut+i) = (int16_t)((*(vectorIn+i))>>16);
	}
}
/**
 * @brief   Searches maximum value in an array
 * @param   *vectorIn 	Pointer to the input vector
 * @param	longitud 	Vector size
 * @return  int32_t		Maximum value index
 */
int32_t max (int32_t * vectorIn, uint32_t longitud)
{
	uint32_t i = 0;
	uint32_t index = 0, maxValue = 0;

	for(i = 0; i < longitud; i++)
	{
		if(*(vectorIn+i) > maxValue)
		{
			maxValue = *(vectorIn+i);
			index = i;
		}
	}

	return index;
}

/**
 * @brief   Deletes one digit every N digits
 * @param   *vectorIn 	Pointer to the input vector
 * @param	longitud 	Vector size
 * @param	N			Number of digit to delete
 * @return  none
 */
void downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N)
{
	int i = 0, outIndex = 0;

	for(i = 0; i < longitud ; i++)	// Recorro el vector
	{
		if((i+1)%N != 0)	// Mientras sea falso (cuando i+1 no es m??ltimo de N)
		{
			vectorOut[outIndex] = vectorIn[i];	// Copio
			outIndex++;
		}
	}
}
/**
 * @brief   Inverts the elements of a vector
 * @param   *vector 	Pointer to the input vector
 * @param	longitud 	Vector size
 * @return  none
 */

void invertir (uint16_t * vector, uint32_t longitud)
{
	uint32_t i = 0;
	uint16_t auxSwap = 0;

	longitud--;

	for (i = 0; i < (longitud - i); i++)
	{
		auxSwap = *(vector + i);
		*(vector + i) = *(vector + (longitud - i));
		*(vector + (longitud - i)) = auxSwap;
	}
}


void eco (int16_t * vectorIn, int16_t * vectorOut, uint32_t longitud, uint32_t muestra)
{
	int i = 0;
	for(i = 0; i < longitud; i++)
		vectorOut[i] = vectorIn[i];

	for(i = 0; (i+muestra) < longitud; i++)
		vectorOut[i+muestra] += vectorIn[i]/2;	// Le agrego el eco defasado
}

void vPrintString( const char *pcString )
{
	/* Print the string, using a critical section as a crude method of mutual
	 * exclusion. */
	{
		printf( "%s", pcString );
		fflush( stdout );
	}
}

void vPrintStringAndNumber( const char *pcString, uint32_t ulValue )
{
	/* Print the string, using a critical section as a crude method of mutual
	 * exclusion. */
	{
		printf( "%s %lu\r\n", pcString, ulValue );
		fflush( stdout );
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
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
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
