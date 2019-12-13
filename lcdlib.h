//////////////////////
//ILI9486 320RGBx480//
//////////////////////

#ifndef __Lcdlib__
#define __Lcdlib__

//colors
//----- RGB565 ------
//xxxxxxxx xxxxxxxx
//rrrrrggg gggbbbbb
//11111000 00000000	0xF800
//00000111 11100000 0x07E0
//00000000 00011111 0x001F
#define BLACK			0x0000
#define RED       0xF800
#define ORANGE 		0xFB00
#define YELLOW    0xFFE0
#define GREEN     0x07E0
#define BLUE      0x001F
#define PURPLE		0x881F
#define GRAY  		0X8430
#define WHITE     0xFFFF	//
#define GOLDEN		0XBC40
#define LIGHTBLUE	0x051F
#define MAGENTA   0xF81F	//
#define CYAN			0x7FFF	//

//lcd drivers
#define	 LCD_RESET_H  		Driver_LcdReset(1)
#define	 LCD_RESET_L  		Driver_LcdReset(0)

//lcd resolution 
#define  LCD_ROW_NUM    320
#define  LCD_COL_NUM    480

//APP USER LIB
extern void LCD_Init(void);
//
extern void LCD_ClrScr(u16 BackColor);
//set: x0, y0, Color
extern void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);
//set: x0, y0, (dir: 1 - vert, 2 - horiz), length, color
extern void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 length, u16 color);
//set: x1, y1, x2, y2, color
extern void LCD_DisBLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
//set: x0, y0, x1, y1, Color
extern void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
//set: x0, y0, r, Color
extern void LCD_DisACircle(u16 x0, u16 y0, u8 r, u16 Color);
//set: x0, y0, wide, Color
extern void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);
//set: x0, y0, x1, y1, Color
extern void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
//set: x0, y0, ASCII, font Color, background Color
extern void LCD_DisASCString8x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//
extern void LCD_DisGB2312String16x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//
//extern void LCD_DisGB2312String32x32(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//set: x0, y0, high, wide, RGB565 Data pointer
extern void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData);
//
#endif
