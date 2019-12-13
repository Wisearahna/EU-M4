// Driver.c

typedef unsigned char       bool;
typedef unsigned char       u8;
typedef unsigned short      u16;
#define  True  1
#define  False 0

//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

/* INCLUDES */
//MCU
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_fsmc.h"
//Driver
#include "Driver.h"

//LCD
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

#define	 LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define  LCD             ((LCD_TypeDef *) LCD_BASE)

//mcu
void Driver_MCU_Init(void);
//lcd
void Driver_LcdBacklight(bool sta);
void Driver_LcdReset(bool sta);
void Driver_LcdFSMCON(void);
void Driver_LcdFSMCWriteData(u16 Data);
void Driver_LcdFSMCWriteReg(u16 Index);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++  MCU ++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU
********************************************************************************************************/
void Driver_MCU_Init(void)
{

		#define PLL_M      8
		#define PLL_N      336	
		#define PLL_P      2	
		#define PLL_Q      7	
  	//--------------------------- CLK INIT, HSE PLL ----------------------------
		ErrorStatus HSEStartUpStatus;
		//RCC reset
		RCC_DeInit();
		//Enable HSE
		RCC_HSEConfig(RCC_HSE_ON); 
		//Wait HSE is ready
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		//If HSE start fail, wail and wail.
		while(HSEStartUpStatus == ERROR);
		//Set bus clock
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//(HCLK=SYSCLK)=168MHz
		RCC_PCLK1Config(RCC_HCLK_Div4);		//(PCLK1=SYSCLK/4)=42MHz
		RCC_PCLK2Config(RCC_HCLK_Div2);		//(PCLK2=SYSCLK/2)=84MHz
		//HSE 8M PLL-> 168M	
		//PLL=8MHz * N/ (M*P)=8MHz* 336 /(8*2) = 168MHz
		RCC_PLLConfig(RCC_PLLSource_HSE,PLL_M ,PLL_N ,PLL_P,PLL_Q);
		RCC_PLLCmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		//Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);
		//----------------------------- CLOSE HSI ---------------------------
		RCC_HSICmd(DISABLE);

		//--------------------------- OPEN GPIO CLK -------------------------
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

		//init RNG
		RCC_AHB3PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

}	

//-------------------------------------------------------------------------------------------------------
//-------------------------------------- LCD DRIVER -----------------------------------------------------
//-------------------------------------------------------------------------------------------------------

/********************************************************************************************************
*  Function: Driver_LcdFSMCON				                                                           
*  Object: FSMC init
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCON(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
		FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
		FSMC_NORSRAMTimingInitTypeDef  writeTiming;
		//FSMC
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,   ENABLE);	
		RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
		//PORTD
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;			
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
		//PORTE
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOE, &GPIO_InitStructure); 
   	//PORTG
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOG, &GPIO_InitStructure); 
		
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12 
   
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12 
   
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_FSMC);//PG0,AF0 
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC); //PG12,AF12 	
		
		//FSMC
		readWriteTiming.FSMC_AddressSetupTime = 0x0f;	
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 
    readWriteTiming.FSMC_DataSetupTime = 0x3c;	
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	
	  writeTiming.FSMC_AddressSetupTime = 0x09;
    writeTiming.FSMC_AddressHoldTime = 0x00;		
    writeTiming.FSMC_DataSetupTime = 0x08;	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 
		//LCD
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; 
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming; 
		//FSMC BANK
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteReg				                                                           
*  Object: FSMC REG
*  Input: Index
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteReg(u16 Index)
{
		LCD->LCD_REG = Index;
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteData				                                                           
*  Object: FSMC Data
*  Input: Data
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteData(u16 Data)
{
		LCD->LCD_RAM = Data;
}

/********************************************************************************************************
*  Function: Driver_LcdBacklight				                                                           
*  Object: lcd backlight control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/

void Driver_LcdBacklight(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PB0
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
			GPIO_Init(GPIOB, &GPIO_InitStructure); 
			StartFlag = False;
		}	
		if(sta)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}


/********************************************************************************************************
*  Function: Driver_LcdReset				                                                           
*  Object: lcd reset control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdReset(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PC5
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		else
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
}

