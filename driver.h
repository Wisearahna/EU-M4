#ifndef __Driver__
#define __Driver__

//mcu
extern void Driver_MCU_Init(void);
//lcd
extern void Driver_LcdBacklight(bool sta);
extern void Driver_LcdReset(bool sta);
extern void Driver_LcdFSMCON(void);
extern void Driver_LcdFSMCWriteReg(u16 Index);
extern void Driver_LcdFSMCWriteData(u16 Data);
#endif



