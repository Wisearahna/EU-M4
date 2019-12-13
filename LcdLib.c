//////////////////////
//ILI9486 320RGBx480//
//////////////////////

#include "prohead.h"
#include "driver.h"
#include "lcdlib.h"
#include "ASC8x16.h"
#include "GB2312.h"
#include "ILI9486L_CMD_TAB.h"
#include "stdint.h"
//#include "math.h"

//lcd bottom funtions
void LCD_WriteReg(u16 Index);
void LCD_WriteData(u16 Data);
void LCD_Delayms(u8 Num);
void LCD_TimerCountHandle(void);
void LCD_PortInit(void);
void LCD_Reset(void);
void LCD_Init(void);

//lcd display functions
void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1);
void LCD_ClrScr(u16 BackColor);
void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);
void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 lenth, u16 color);
void LCD_DisBLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);
void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);   
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
void LCD_DisASCString16x8(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData);

/********************************************************************************************************
*  Function: LCD_WriteReg				                                                           
*  Object: lcd write reg
*  Input: index
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_WriteReg(u16 Index)
{
	//use FSMC
	Driver_LcdFSMCWriteReg(Index);
}

/********************************************************************************************************
*  Function: LCD_WriteData				                                                           
*  Object: lcd write data
*  Input: index and data
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_WriteData(u16 Data)
{
	//use FSMC
	Driver_LcdFSMCWriteData(Data);
}

/********************************************************************************************************
*  Function: LCD_Delayms				                                                           
*  Object: lcd init wait..
*  Input: Num
*  Output: none                                  
*  brief: time = Num * 1ms (168MHz)
********************************************************************************************************/
void LCD_Delayms(u8 Num)
{
	u16 Timer;
	while(Num--)
		{
		 	Timer = 11500;
			while(Timer--); 
		}
}

/********************************************************************************************************
*  Function: LCD_PortInit				                                                           
*  Object: lcd port init
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_PortInit(void)
{
	//use FSMC
	Driver_LcdFSMCON();
}

/********************************************************************************************************
*  Function: LCD_Reset				                                                           
*  Object: lcd reset control
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_Reset(void)
{
	LCD_RESET_L;
	LCD_Delayms(150);
	LCD_RESET_H;
	LCD_Delayms(50);
}

/********************************************************************************************************
*  Function: Driver_LcdReset				                                                           
*  Object: lcd reset control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_Init(void)
{ 
	//-- LCD PORT INIT --
	LCD_PortInit();

	//-- LCD RESET--
	LCD_Reset();

	//-- Initial sequence send --
	LCD_WriteReg(ILI_CMD_IF_MODE_CTRL);
	LCD_WriteData(0x00);

	LCD_WriteReg(ILI_CMD_SLEEP_OUT);
	LCD_Delayms(120);

	LCD_WriteReg(ILI_CMD_FRAME_RATE_IN_PARTIAL);
	LCD_WriteData(0x02); //DIVC
	LCD_WriteData(0x00); //RTN
	LCD_WriteData(0x00); //
	LCD_WriteData(0x00); //

	LCD_WriteReg(ILI_CMD_PWR_CTRL1);
	LCD_WriteData(0x10);//13	VRH1
	LCD_WriteData(0x3B);//480	VRH2
	LCD_WriteData(0x00);
	LCD_WriteData(0x02);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);//NW
	LCD_WriteData(0x43);

	LCD_WriteReg(ILI_CMD_PWR_CTRL2);
	LCD_WriteData(0x10);//
	LCD_WriteData(0x08);
	LCD_WriteData(0x10);//
	LCD_WriteData(0x16);//CLOCK
	LCD_WriteData(0x08);
	LCD_WriteData(0x08);

	LCD_WriteReg(ILI_CMD_PWR_CTRL5);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);

	LCD_WriteReg(ILI_CMD_CABC_CTRL1);
	LCD_WriteData(0x00);

	LCD_WriteReg(ILI_CMD_CABC_CTRL2);//GAMMA
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);
	LCD_WriteData(0x13);
	LCD_WriteData(0x5C);
	LCD_WriteData(0x03);
	LCD_WriteData(0x07);
	LCD_WriteData(0x14);
	LCD_WriteData(0x08);
	LCD_WriteData(0x00);
	LCD_WriteData(0x21);
	LCD_WriteData(0x08);
	LCD_WriteData(0x14);
	LCD_WriteData(0x07);
	LCD_WriteData(0x53);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x13);
	LCD_WriteData(0x03);
	LCD_WriteData(0x03);
	LCD_WriteData(0x21);
	LCD_WriteData(0x00);

	LCD_WriteReg(ILI_CMD_TEL_ON);
	LCD_WriteData(0x00);

	//Memory Access Control
	LCD_WriteReg(ILI_CMD_MEM_ACCESS_CTRL);
	//MY=0, MX=0, MV=0, ML=0, BGR=1, MH=0
	//LCD_WriteData(0x08);
	//MY=0, MX=1, MV=1, ML=0, BGR=1, MH=0
	LCD_WriteData(0x68);

	LCD_WriteReg(ILI_CMD_IF_PIXEL_FORMAT);
	LCD_WriteData(0x55);

	LCD_WriteReg(ILI_CMD_WRITE_TEAR_SCAN_LINE);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);

	LCD_WriteReg(ILI_CMD_DISP_FUNC_CTRL);
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);//0 GS SS SM ISC[3:0]; GS SS, R36
	LCD_WriteData(0x3B);

	LCD_WriteReg(ILI_CMD_WRITE_NV_MEM);
	LCD_WriteData(0x07);
	LCD_WriteData(0x07);//VCI1
	LCD_WriteData(0x1D);//VRH

	LCD_WriteReg(ILI_CMD_WRITE_NV_MEM_KEY);
	LCD_WriteData(0x00);
	LCD_WriteData(0x03);//VCM
	LCD_WriteData(0x00);//VDV

	LCD_WriteReg(ILI_CMD_READ_NV_MEM_STATUS);
	LCD_WriteData(0x03);
	LCD_WriteData(0x14);
	LCD_WriteData(0x04);

	LCD_WriteReg(ILI_CMD_DISP_ON);
	LCD_Delayms(120);

	LCD_WriteReg(ILI_CMD_DISP_INV_CTRL);
	LCD_WriteData(0x00);
	LCD_Delayms(120);

	LCD_WriteReg(ILI_CMD_W_TO_MEM);
}

/**************************************
*  Function: LCD_OpenWin				                                                           
*  Object: lcd open window for display
*  Input: x0,y0, x1, y1
*  Output: none                                  
*  brief: none
***************************************/
void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1)
{
	LCD_WriteReg(0x2A);	//Column Address Set
	LCD_WriteData(x0>>8);
	LCD_WriteData(0x00FF&x0);		
	LCD_WriteData(x1>>8);
	LCD_WriteData(0x00FF&x1);

	LCD_WriteReg(0x2B);	//Page Address Set
	LCD_WriteData(y0>>8);
	LCD_WriteData(0x00FF&y0);		
	LCD_WriteData(y1>>8);
	LCD_WriteData(0x00FF&y1);

	LCD_WriteReg(0x2C);
}

/********************************************************************************************************
*  Function: LCD_ClrScr				                                                           
*  Object: lcd clear screen
*  Input: backcolor
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_ClrScr(u16 BackColor)
{
	u16 i,j;
	LCD_OpenWin(0, 0, LCD_COL_NUM-1, LCD_ROW_NUM-1);
	for(i = 0; i < LCD_COL_NUM; i++)
		for(j =0; j < LCD_ROW_NUM; j++)
			LCD_WriteData(BackColor);
}

/********************************************************************************************************
*  Function: LCD_DisAPoint				                                                           
*  Object: Display a point at screen
*  Input: site and color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisAPoint(u16 x0, u16 y0, u16 Color)
{
	//LCD_DisASquare(x0, y0, 1, Color);
	//wisearahna
	LCD_OpenWin(x0, y0, x0, y0);
	LCD_WriteData(Color);
}

/********************************************************************************************************
*  Function: LCD_DisALine				                                                           
*  Object: Display a line
*  Input: site dir(1 - vert, 2 - horiz) length wide color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 length, u16 color)
{
	u16 x1,y1;
	x1 = x0;
	y1 = y0;
	if(dir == 1)
		y1 = y1 + length;
	else
		x1 = x1 + length;
	LCD_DisARectangular(x0, y0, x1, y1, color);
}
//wisearahna
//Bresenham's line algorithm
void LCD_DisBLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{  
	int dx, dy, sx, sy, d, d1, d2;
	u16 i, x, y;

   if( x2 >= x1)
   {  dx = x2 - x1;
      sx = 1;
   } else {
      dx = x1 - x2; 
      sx = -1;
   } 
   if( y2 >= y1)
   {  dy = y2 - y1;
      sy = 1;
   } else {
      dy = y1 - y2; 
      sy = -1;
   } 

   if(dy <= dx) 
   {  d = (dy << 1) - dx;
      d1 = dy << 1;
      d2 = (dy - dx) << 1;

      for(x = x1 + sx, y = y1, i = 1; i <= dx ; i++, x += sx)
      {  if(d > 0)
         {  d += d2;
            y += sy;
         } else {
            d += d1;
         }   
         LCD_DisAPoint(x, y, color);
      } 

   } else {
      d  = (dx << 1) - dy;
      d1 = dx << 1;
      d2 = (dx - dy) << 1;

      for(x=x1,y=y1+sy,i=1;i <= dy ; i++,y += sy)
      {  if(d > 0)
         {  d += d2;
            x += sx;
         } else {
            d += d1;
         }   
         LCD_DisAPoint(x, y, color);
      } 

   } /* endif(dy <=dx) */
}
/********************************************************************************************************
*  Function: LCD_DisABorder				                                                           
*  Object: Display a border
*  Input: x0, y0, x1, y1, color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
	LCD_DisALine(x0, y0, 1, y1-y0, Color);
	LCD_DisALine(x1, y0, 1, y1-y0, Color);
	LCD_DisALine(x0, y0, 2, x1-x0, Color);
	LCD_DisALine(x0, y1, 2, x1-x0, Color);
}

/********************************************************************************************************
*  Function: LCD_DisCircle				                                                           
*  Object: Display a circle
*  Input: site,radius and color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisACircle(u16 x0, u16 y0, u8 r, u16 Color)
{
	s16 a,b,next;
	a	=	0;
	b = r;	  
	next = 3 - (r<<1);            
	while(a <= b)
		{
			LCD_DisAPoint(x0+a, y0-b, Color);             
			LCD_DisAPoint(x0+b, y0-a, Color);                      
			LCD_DisAPoint(x0+b, y0+a, Color);                          
			LCD_DisAPoint(x0+a, y0+b, Color);             
			LCD_DisAPoint(x0-a, y0+b, Color);                  
			LCD_DisAPoint(x0-b, y0+a, Color);             
			LCD_DisAPoint(x0-a, y0-b, Color);                          
			LCD_DisAPoint(x0-b, y0-a, Color);              	         
			a++;
			//use the bresenham    
			if(next<0)
				next += 4*a+6;	  
			else
				{
					next += 10+4*(a-b);   
					b--;
				} 						    
		}
} 

/********************************************************************************************************
*  Function: LCD_DisASquare				                                                           
*  Object: Display a square
*  Input: start point, wide, color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color)
{
	u16 i,j;
	LCD_OpenWin(x0, y0, x0+wide-1, y0+wide-1);
	for(i = 0; i < wide; i++)
		for(j = 0; j < wide; j++)
			 LCD_WriteData(Color);
}

/********************************************************************************************************
*  Function: LCD_DisARectangular				                                                           
*  Object: Display a rectangular
*  Input: start point, end point, color
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
	u16 i,j;
	LCD_OpenWin(x0, y0, x1, y1);
	for(i = 0; i <= x1-x0; i++)
		for(j = 0; j <= y1-y0; j++)
			LCD_WriteData(Color);
}

/********************************************************************************************************
*  Function: LCD_DisASC8x16				                                                           
*  Object: Display a ASCII(8x16)
*  Input: site, char, fColor, bColor
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisASCString8x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor)
{
	u16 cc = 1; //char counter
	while(*s)
		{
			LCD_OpenWin(x0+(8-1)*(cc-1), y0, x0+(8-1)*cc, y0+16-1);//open for one char
			////LCD_OpenWin(x0, y0+(8-1)*(l-1), x0+16-1, y0+(8-1)*l);
			for(u8 i=0; i<16; i++) 
				{
					u8 m = InforCode_Font8x16[(*s)*16+i];
					for(u8 j=0; j<8; j++) 
						{
							if(m&CHSBIT7)
								LCD_WriteData(fColor);
							else 
								LCD_WriteData(bColor);
							m <<= 1;
						}
				}
			s++;
			cc++;
		}
}

/********************************************************************************************************
*  Function: LCD_DisAPhoto				                                                           
*  Object: display a photo
*  Input: Site(x0,y0), high and wide, pData
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData)
{
	u16 i,length;
	length = high * wide * 2;  		//RGB565 bitmap
	LCD_OpenWin(x0, y0, x0+high-1, y0+wide-1);
	for(i = 0; i < length; i+=2)
		LCD_WriteData((*(pData+i))*0x100 + (*(pData+i+1)));
}
