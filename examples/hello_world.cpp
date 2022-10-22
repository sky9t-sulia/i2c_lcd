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

I2C_LCD lcd(0x27, _COLS, _ROWS);

void app_hello_world() {
	lcd.clear();
	lcd.backlight(true);

	lcd.set_cursor(5, 0);
	lcd.write_str("Hello, world");
}
