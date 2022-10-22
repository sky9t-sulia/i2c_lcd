/**
 * To run this example need to include hello_world.h
 *
 * USER CODE BEGIN Includes
 * #include "hello_world.h"
 * USER CODE END Includes
 */

#include "hello_world.h"

#define _COLS 20
#define _ROWS 4

extern I2C_HandleTypeDef hi2c1; // SPI1

I2C_LCD lcd(&hi2c1, 0x27, _COLS, _ROWS);

void app_hello_world() {
	lcd.clear();
	lcd.backlight(true);

	lcd.set_cursor(5, 0);
	lcd.write_str("Hello, world");
}
