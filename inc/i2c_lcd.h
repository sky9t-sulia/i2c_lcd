#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_RS    (1 << 0)	// 0 command 1 data
#define PIN_EN    (1 << 2)	// read
#define LCD_DELAY_MS 5		// pause for display to draw char

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_4BITMODE 0x00

#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00


#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x0


class I2C_LCD {
private:
	uint8_t cols;
	uint8_t rows;
	uint8_t address;
	uint8_t lcd_mode;
	uint8_t lcd_control;
	uint8_t lcd_function;
	uint8_t bkl_config;
	I2C_HandleTypeDef *handler;
	void send(uint8_t value, uint8_t mode);
public:
	I2C_LCD(uint8_t address, uint8_t cols, uint8_t rows);
	void home();
	void set_cursor(uint8_t col, uint8_t row);
	void clear();
	void backlight(bool enable);
	void on_off(bool on);
	void blink(bool on);
	void cursor(bool on);
	void entry_mode(bool ltr);
	void write_str(const char *str);
};

#ifdef __cplusplus
}
#endif

#endif /* INC_I2C_LCD_H_ */
