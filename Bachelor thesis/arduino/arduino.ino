#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4);
#include <DHT.h>
#include <SoftwareSerial.h>

SoftwareSerial sw(2, 3);
#define DHTPIN A0 
#define DHTTYPE DHT22
#define yellowled 11
#define blueled 12
#define redled 13

const int buttonPin1 = 2;
const int buttonPin2 = 8;
const int buttonPin3 = 9;
const int buttonPin4 = 10;

const int trigPin = 7;
const int echoPin = 6;

int red_light_pin= 5;
int green_light_pin = 4;
int blue_light_pin = 3;

float duration, cm;
int reading = Serial.read();

int deleteOnce = 0;
int buttonPushCounter = 0;

int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonState3 = 0;
int lastButtonState3 = 0;
int buttonState4 = 0;
int lastButtonState4 = 0;

DHT dht(DHTPIN, DHTTYPE);
float hum;
float temp;

int led_list[3];
int button_list[3];
int check_list[3];

int flash_rate = 750;

int button_count = 0;
int cycle_count = 0;
int flash_count = 0;
int correct_count = 0;

const size_t size_of_led = sizeof(led_list) / sizeof(led_list[0]);
const size_t size_of_button = sizeof(button_list) / sizeof(button_list[0]);
const int led_button[4][2] {{11, 8}, {12, 9}, {13, 10}};
int button_state_map[4][3] {{8, 0, 0}, {9, 0, 0}, {10, 0, 0}};

void setup() {
  for (int i = 0; i < size_of_led; i++) {
    led_list[i] = led_button[i][0];

    button_list[i] = led_button[i][1];
    pinMode(button_list[i], INPUT);
  }
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(yellowled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  dht.begin();
  sw.begin(9600);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  lastButtonState1 = buttonState1;
  
  if(buttonPushCounter == 0){
    RGB_color(0, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print("Welcome!");
    lcd.setCursor(0, 2);
    lcd.print("Press the button to");
    lcd.setCursor(0, 3);
    lcd.print("start the first mode");
  }
  if(buttonPushCounter == 1){
    if(deleteOnce == 0){
      lcd.clear();
      deleteOnce = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print("Mode 1:");
    lcd.setCursor(3, 1);
    lcd.print("Distance sensor");
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    cm = (duration*.0343)/2;

    if (cm < 15){
       RGB_color(255, 0, 0);
    }
    if(cm >= 15 && cm < 110){
       RGB_color(0, 255, 0);
    }
    if(cm >= 110){
       RGB_color(0, 0, 255);
    }
    lcd.setCursor(0, 3);
    lcd.print(cm);
    lcd.setCursor(5, 3);
    lcd.print(" cm");
    delay(500);
  }
  if(buttonPushCounter == 2){
    if(deleteOnce == 1){
      RGB_color(0, 0, 0);
      lcd.clear();
      deleteOnce = 2;
    }
    lcd.setCursor(0, 0);
    lcd.print("Mode 2:");
    lcd.setCursor(3, 1);
    lcd.print("Push buttons");
    buttonState2 = digitalRead(buttonPin2);
    if (buttonState2 != lastButtonState2) {
      // if the state has changed, increment the counter
      if (buttonState2 == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        digitalWrite(yellowled, HIGH);       
      } else {
        // if the current state is LOW then the button went from on to off:
        digitalWrite(yellowled, LOW);
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    lastButtonState2 = buttonState2;
    buttonState3 = digitalRead(buttonPin3);
    if (buttonState3 != lastButtonState3) {
      // if the state has changed, increment the counter
      if (buttonState3 == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        digitalWrite(blueled, HIGH);       
      } else {
        // if the current state is LOW then the button went from on to off:
        digitalWrite(blueled, LOW);
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    lastButtonState3 = buttonState3;
    buttonState4 = digitalRead(buttonPin4);
    if(buttonState4 != lastButtonState4) {
      // if the state has changed, increment the counter
      if(buttonState4 == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        digitalWrite(redled, HIGH);       
      } else {
        // if the current state is LOW then the button went from on to off:
        digitalWrite(redled, LOW);
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    lastButtonState4 = buttonState4;
  }
  
  if(buttonPushCounter == 3){
    if(deleteOnce == 2){
      lcd.clear();
      deleteOnce = 3;
    }
    lcd.setCursor(0, 0);
    lcd.print("Mode 3:");
    lcd.setCursor(3, 1);
    lcd.print("ESP Server LED");
    if(Serial.available()){
      Serial.print("I received: ");
      // read the incoming byte:
      while(Serial.available()) {
         reading = Serial.read();
         if(reading == 1){
           Serial.print(reading);
           digitalWrite(yellowled, HIGH);
         }
         else if(reading == 2){
           Serial.print(reading);
           digitalWrite(blueled, LOW);
         }
         else if(reading == 3){
           Serial.print(reading);
           digitalWrite(blueled, HIGH);
         }
         else if(reading == 0){
           Serial.print(reading);
           digitalWrite(yellowled, LOW);
         }
         else if(reading == 5){
           Serial.print(reading);
           digitalWrite(redled, HIGH);
         }
         else if(reading == 4){
           Serial.print(reading);
           digitalWrite(redled, LOW);
         }
         if(!Serial.available()) delayMicroseconds(1200);
      }
      Serial.println();
    }
  }

  if(buttonPushCounter == 4){
    if(deleteOnce == 3){
      lcd.clear();
      deleteOnce = 4;
    }
    lcd.setCursor(0, 0);
    lcd.print("Mode 4:");
    lcd.setCursor(0, 1);
    lcd.print("Temperature&Humidity");

    hum = dht.readHumidity();
    temp= dht.readTemperature();
    lcd.setCursor(0, 2);
    lcd.print(temp);
    lcd.setCursor(6, 2);
    lcd.print("C");
    lcd.setCursor(0, 3);
    lcd.print(hum);
    lcd.setCursor(6, 3);
    lcd.print("%");
 
    if (temp < 18 || hum < 30){
       RGB_color(0, 0, 255);
    }
    if((temp >= 18 &&  temp < 25) || (hum >= 30 && hum < 60)){
       RGB_color(0, 255, 0);
    }
    if(temp >= 25 || hum >= 60){
       RGB_color(255, 0, 0);
    }
  }
  if(buttonPushCounter == 5){
    if(deleteOnce == 4){
      lcd.clear();
      RGB_color(0, 0, 0);
      deleteOnce = 5;
    }
    lcd.setCursor(0, 0);
    lcd.print("Mode 5:");
    lcd.setCursor(1, 1);
    lcd.print("Button memory game");
    while (flash_count == 0) {
      // CALL THE 'shuffle_led_list' FUNCTION //
      shuffle_led_list();
      // POPULATE 'button_list' WITH PIN NUMBERS, RESPECTIVE TO THE ORDER IN WHICH THE LEDs FLASHED. SET ALL LED PINS AS AN OUTPUT //
      for (int i = 0; i < size_of_button; i++) {
        button_list[i] = led_button[find_index_led(led_button, size_of_led, led_list[i])][1];
        pinMode(led_list[i], OUTPUT);
      }
      // WAIT 1 SECOND //
      delay(1000);
      // FLASH THE LEDs //
      for (int j = 0; j < size_of_led; j++) {
        digitalWrite(led_list[j], HIGH);
        delay(flash_rate);
        digitalWrite(led_list[j], LOW);
      }
      flash_count++;
    }
    process_button();
    while (button_count == size_of_button) {
      // IF THE LIST INDEXES ARE EQUAL FOR BOTH LISTS, INCREMENT 'cycle_count' //
      for (int k = 0; k < size_of_button; k++) {
        if (button_list[k] == check_list[k]) {
          cycle_count++;
        }
      }
      // IF PLAYER INPUTTED WRONG COMBONATION, FLASH LAST LED 3 TIMES AND DISPLAY THE SCORE //
      if (cycle_count < size_of_button) {
        for (int l = 0; l < 3; l++) {
          digitalWrite(led_button[3][0], HIGH);
          delay(200);
          digitalWrite(led_button[3][0], LOW);
          delay(200);
        }    
        lcd.setCursor(1, 2);
        lcd.print("Final score:");
        lcd.setCursor(13, 2);
        lcd.print(correct_count);
        delay(5000);
        lcd.clear();
        correct_count = 0;
        flash_rate = 750;
      }
      correct_count++;
      // IF PLAYER INPUTTED CORRECT COMBONATION, FLASH ALL LEDs //
      for (int m = 0; m < size_of_led; m++) {
        digitalWrite(led_button[m][0], HIGH);
      }
      lcd.setCursor(1, 2);
      lcd.print("Score:");
      lcd.setCursor(8, 2);
      lcd.print(correct_count);   
      for (int n = 0; n < size_of_led; n++) {
        digitalWrite(led_button[n][0], LOW);
      }
      // DECREMENT 'flash_rate' BY 75 EVERY SECOND CYCLE WHILE 'flash_rate' > 150 //
      if (correct_count > 1) {
        if (flash_rate > 100 && (correct_count & 1) != 0) {
          flash_rate -= 75;
        }
      }
      // RESET KEY VARIABLES //
      button_count -= button_count; cycle_count -= cycle_count; flash_count -= flash_count;
    }
  }
  
  if(buttonPushCounter == 6){
    if(deleteOnce == 5){
      lcd.clear();
      deleteOnce = 0;
    }
    buttonPushCounter = 0;
  }
}

int process_button() {
  // READING THE BUTTON STATE FROM THE DESIRED PIN //
  for (int i = 0; i < size_of_button; i++) {
    button_state_map[i][1] = digitalRead(button_state_map[i][0]);
  }
  // DEPENDING ON WHAT BUTTON IS PRESSED, APPEND THAT BUTTON TO THE ARRAY 'check_list' //
  if (button_state_map[0][1] != button_state_map[0][2]) {
    if (button_state_map[0][1] == HIGH) {
      check_list[button_count] = button_state_map[0][0]; button_count++;
    }
    delay(50);
  }
  button_state_map[0][2] = button_state_map[0][1];
  if (button_state_map[1][1] != button_state_map[1][2]) {
    if (button_state_map[1][1] == HIGH) {
      check_list[button_count] = button_state_map[1][0]; button_count++;
    }
    delay(50);
  }
  button_state_map[1][2] = button_state_map[1][1];
  if (button_state_map[2][1] != button_state_map[2][2]) {
    if (button_state_map[2][1] == HIGH) {
      check_list[button_count] = button_state_map[2][0]; button_count++;
    }
    delay(50);
  }
  button_state_map[2][2] = button_state_map[2][1];
  if (button_state_map[3][1] != button_state_map[3][2]) {
    if (button_state_map[3][1] == HIGH) {
      check_list[button_count] = button_state_map[3][0]; button_count++;
    }
    delay(50);
  }
  button_state_map[3][2] = button_state_map[3][1];
}

int find_index_led(const int list[4][2], int size, int value) {
  int index = 0;
  while (index < size && list[index][0] != value) index++;
  return (index == size ? -1 : index);
}
// FUNCTION TO SHUFFLE THE LED LIST - MAKE THE LEDs FLASH IN A RANDOM ORDER //
int shuffle_led_list() {
  for (size_t i = 0; i < size_of_led; i++) {
    size_t j = random(0, size_of_led);
    int k = led_list[i];
    led_list[i] = led_list[j];
    led_list[j] = k;
    randomSeed(analogRead(1));
  }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
