#include "mastermind.h"
#include "lcd_wrapper.h"
//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

//int cislo1 = 0;
//int cislo2 = 0;
//int cislo3 = 0;
//int cislo4 = 0;

void setup() {
  // put your setup code here, to run once:
  lcd_init();
  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);
  pinMode(BTN_3_PIN, INPUT);
  pinMode(BTN_4_PIN, INPUT);
  pinMode(BTN_ENTER_PIN, INPUT);

  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_BLUE_2, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  pinMode(LED_BLUE_3, OUTPUT);
  pinMode(LED_RED_3, OUTPUT);
  pinMode(LED_BLUE_4, OUTPUT);
  pinMode(LED_RED_4, OUTPUT);
  turn_off_leds();
}

void loop() {
  Serial.begin(9600);
  turn_off_leds();
  lcd_print_at(1,0,"Vitaj v Logik");
  lcd_print_at(2,1,"Uhadnes kod?");
  delay(2000);
  char* code = NULL;
  code = generate_code(false, 4);
  lcd_clear();
  lcd_set_cursor(0,0);
  play_game(code);
  lcd_clear();

  free(code);
}
