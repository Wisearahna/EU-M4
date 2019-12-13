//************************
// ILI9486 TFT 320RGBx480
// wisearahna
// generate random process
// and try filtering by
// Kalman filter
//************************
#include "prohead.h"
#include "driver.h" 
#include "lcdlib.h"
#include "stdint.h"
#include "stm32f4xx_rng.h"

#define step	4

void DIS_Delayms(u16 Num);
float Kfilter(float val);
//Kalman filter vars
float varS = 160; //sigma
float varProcess = 1; //velocity
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;
//

//random number
uint16_t	randx=0, rx=0;
uint16_t	randy=0, ry=0, rk=0, rkn=0;
uint16_t	randc=0;

int main(void)
{
	Driver_MCU_Init(); //init uController
	LCD_Init(); //init TFT
	Driver_LcdBacklight(True);
	LCD_ClrScr(BLACK); //clear screen
	//enable random gen
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
	
	
	while(1)
	{
		
		for(uint16_t i=0; i<LCD_COL_NUM; i=i+step)
		{
			randy = RNG_GetRandomNumber()>>23;
				if(randy > LCD_ROW_NUM)
					randy=ry;
				LCD_DisBLine(i, LCD_ROW_NUM-ry, i+step, LCD_ROW_NUM-randy, GREEN);
				rk=Kfilter(randy);
				LCD_DisBLine(i, LCD_ROW_NUM-rkn, i+step, LCD_ROW_NUM-rk, BLUE);
				//fat line
				//LCD_DisBLine(i, LCD_ROW_NUM-rkn+2, i+step, LCD_ROW_NUM-rk+2, RED);
				//LCD_DisBLine(i, LCD_ROW_NUM-rkn+1, i+step, LCD_ROW_NUM-rk+1, RED);
				//LCD_DisBLine(i, LCD_ROW_NUM-rkn-1, i+step, LCD_ROW_NUM-rk-1, RED);
				//LCD_DisBLine(i, LCD_ROW_NUM-rkn-2, i+step, LCD_ROW_NUM-rk-2, RED);
				//
				rkn=rk;
				ry=randy;
		}
		
		//show time
		LCD_DisASCString8x16(0, 20, (u8 *) "Kalman filter", BLUE, BLACK);
		LCD_DisASCString8x16(0, 0, (u8 *)"Random process", GREEN, BLACK);
		
		DIS_Delayms(3000);
		LCD_ClrScr(BLACK);
	}
}

//
void DIS_Delayms(u16 Num)
{
   	u16 Timer;
		while(Num--)
		{
		 	Timer = 25000;
			while(Timer--); 
		}
}
//Kalman filter
float Kfilter(float val) {
  Pc = P + varProcess;
  G = Pc/(Pc + varS);
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(val-Zp)+Xp;
  return(Xe);
}


