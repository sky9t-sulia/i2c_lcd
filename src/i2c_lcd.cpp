#include "i2c_lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

I2C_LCD::I2C_LCD(uint8_t address, uint8_t cols, uint8_t rows) {

	this->lcd_function = LCD_4BITMODE | LCD_5x8DOTS;
	this->address = (address << 1); // shift address by one bit on stm32
	this->cols = cols;
	this->rows = rows;
	this->handler = &hi2c1;
	this->bkl_config = LCD_BACKLIGHT;
	this->lcd_control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

	HAL_Delay(100);

	this->backlight(false);
	HAL_Delay(1000);

	// we start in 8bit mode, try to set 4 bit mode
	this->send(0x03 << 4, 0);
	HAL_Delay(6); // wait min 4.1ms

	// second try
	this->send(0x03 << 4, 0);
	HAL_Delay(6); // wait min 4.1ms

	// third go!
	this->send(0x03 << 4, 0);
	HAL_Delay(3);

	// finally, set to 4-bit interface
	this->send(0x02 << 4, 0);

	this->send(LCD_FUNCTIONSET | this->lcd_function, 0);

	this->lcd_control |= LCD_DISPLAYON;
	this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);

	this->clear();

	this->lcd_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	this->send(LCD_ENTRYMODESET | this->lcd_mode, 0);

	this->home();
}

void I2C_LCD::home() {
	this->send(LCD_RETURNHOME, 0);
	HAL_Delay(LCD_DELAY_MS);
}

void I2C_LCD::clear() {
	this->send(LCD_CLEARDISPLAY, 0);
	HAL_Delay(LCD_DELAY_MS);
}

void I2C_LCD::set_cursor(uint8_t x, uint8_t y) {
	switch (y) {
	case 0:
		this->send(x | LCD_SETDDRAMADDR, 0);
		HAL_Delay(1);
		break;
	case 1:
		this->send((0x40 + x) | LCD_SETDDRAMADDR, 0);
		HAL_Delay(1);
		break;
	case 2:
		this->send((0x14 + x) | LCD_SETDDRAMADDR, 0);
		HAL_Delay(1);
		break;
	case 3:
		this->send((0x54 + x) | LCD_SETDDRAMADDR, 0);
		HAL_Delay(1);
		break;
	default:
		break;
	}
}

void I2C_LCD::backlight(bool enable) {
	if (enable) {
		this->bkl_config = LCD_BACKLIGHT;
	} else {
		this->bkl_config = LCD_NOBACKLIGHT;
	}

	this->send(0, 0);
}

void I2C_LCD::on_off(bool on) {
	if (on) {
		this->lcd_control |= LCD_DISPLAYON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);
	} else {
		this->lcd_control &= ~LCD_DISPLAYON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);

	}
}

void I2C_LCD::blink(bool on) {
	if (on) {
		this->lcd_control |= LCD_BLINKON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);
	} else {
		this->lcd_control &= ~LCD_BLINKON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);
	}
}

void I2C_LCD::cursor(bool on) {
	if (on) {
		this->lcd_control |= LCD_CURSORON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);
	} else {
		this->lcd_control &= ~LCD_CURSORON;
		this->send(LCD_DISPLAYCONTROL | this->lcd_control, 0);
	}
}

// This is for text that flows Left to Right
void I2C_LCD::entry_mode(bool ltr) {
	if (ltr) {
		this->lcd_mode |= LCD_ENTRYLEFT;
		this->send(LCD_ENTRYMODESET | this->lcd_mode, 0);
	} else {
		this->lcd_mode &= ~LCD_ENTRYLEFT;
		this->send(LCD_ENTRYMODESET | this->lcd_mode, 0);
	}
}

void I2C_LCD::send(uint8_t value, uint8_t mode) {
	HAL_StatusTypeDef res;

	// wait for device be ready
	for (;;) {
		res = HAL_I2C_IsDeviceReady(this->handler, this->address, 1,
		HAL_MAX_DELAY);

		if (res == HAL_OK) {
			break;
		}
	}

	uint8_t up = value & 0xf0;
	uint8_t lo = (value << 4) & 0xf0;

	uint8_t data_arr[4];
	data_arr[0] = up | mode | this->bkl_config | PIN_EN;
	data_arr[1] = up | mode | this->bkl_config;
	data_arr[2] = lo | mode | this->bkl_config | PIN_EN;
	data_arr[3] = lo | mode | this->bkl_config;

	HAL_I2C_Master_Transmit(this->handler, this->address, data_arr,
			sizeof(data_arr),
			HAL_MAX_DELAY);

	HAL_Delay(LCD_DELAY_MS);
}

void I2C_LCD::write_str(const char *str) {
	while (*str) {
		this->send((uint8_t) (*str), 1);
		str++;
	}
}

#ifdef __cplusplus
}
#endif

