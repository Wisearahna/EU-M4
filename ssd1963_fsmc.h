#ifndef _SSD1963_FSMS_H_
#define _SSD1963_FSMS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_ll_fsmc.h"

#include <stdlib.h>
#include <string.h>

// Register Select control
// FMC / 16bit / bank 1 / NE4 (forth subbank) / A10 use as register select
#define CMD *(uint16_t *) 0x6C000000
#define DAT *(uint16_t *) 0x60000800
// math
#define ABS(x) ((x) > 0 ? (x) : -(x))
// mirroring
#define MIRROR_V 0	//flip vertical
#define MIRROR_H 1	//flip horizontal

// Reset control
#define LCD_RST_SET   HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
#define LCD_RST_RESET HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

// LCD mode
#define LSHIFT	1
#define LCD_HEIGHT	480
#define LCD_WIDTH		800

//colors 16 bit
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
#define LIGHTBLUE	0x051F
#define DARK_BLUE 0x0007
#define PURPLE		0x881F
#define GRAY  		0X8430
#define WHITE     0xFFFF
#define GOLDEN		0XBC40
#define MAGENTA   0xF81F
#define CYAN			0x7FFF


typedef struct { // Data stored PER GLYPH
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor position to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

//inline uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);

//functions
void LCD_Init(void);
void LCD_Bright(uint8_t bright);
void LCD_Pixel(uint16_t x, uint16_t y, uint16_t color16);
void LCD_Rect_Fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color16);
void LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color16);
void LCD_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint16_t color16);
void LCD_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color16);
void LCD_Triangle_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color16);
void LCD_Ellipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint8_t fill, uint8_t size, uint16_t color16);
void LCD_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color16);
void LCD_Rect_Round(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color16);
void LCD_Rect_Round_Fill(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color16);
void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font, uint8_t size, uint16_t color16);

// LCD control commands
#define LCD_RESET			 		  0x01
#define LCD_SLEEP_OUT		  	0x11
#define LCD_GAMMA			    	0x26
#define LCD_DISPLAY_OFF			0x28
#define LCD_DISPLAY_ON			0x29
#define LCD_COLUMN_ADDR			0x2A
#define LCD_PAGE_ADDR			  0x2B
#define LCD_GRAM				    0x2C
#define LCD_SAM			        0x36
#define LCD_SMODE				    0xB0
#define LCD_GMODE				    0xB1
#define LCD_SHP					    0xB4
#define LCD_GHP					    0xB5
#define LCD_SVP				 	    0xB6
#define LCD_GVP				 	    0xB7
#define LCD_SPWMC			 	    0xBE
#define LCD_STPLL						0xE0
#define LCD_SPLL						0xE2
#define LCD_LSHIFT					0xE6
#define LCD_SPDI						0xF0 

#ifdef __cplusplus
}
#endif

#endif /* _SSD1963_FSMS_H_ */
