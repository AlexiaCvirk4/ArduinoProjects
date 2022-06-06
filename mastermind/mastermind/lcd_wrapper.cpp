#include <Arduino.h>
#include <Wire.h> // Library for I2C communication
#include "LiquidCrystal_I2C.h" // Library for LCD

#include "lcd_wrapper.h"
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

void lcd_init(){
  lcd.init();
  lcd.backlight();
}

void lcd_clear(){
  lcd.clear();
}


void lcd_set_cursor(int y, int x){
  lcd.setCursor(y, x);
}


void lcd_print(char* text){
    lcd.print(text);
}


void lcd_print_at(int y, int x, char* text){
    lcd.setCursor(y, x);
    lcd.print(text);
}
