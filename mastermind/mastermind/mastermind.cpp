#include <Arduino.h>
#include <stdlib.h>
#include "mastermind.h"
#include "lcd_wrapper.h"

char* generate_code(bool repeat, int length){
  if(length < 1){
    return NULL;
  }
  
  char *secret = malloc(length);

  if(repeat == false){
    secret[0] = random(0,10) + '0';
    secret[1] = random(0,10) + '0';
    while(secret[1] == secret[0]){
      secret[1] = random(0,10) + '0';
    }
    secret[2] = random(0,10) + '0';
    while(secret[2] == secret[1] || secret[2] == secret[0]){
      secret[2] = random(0,9) + '0';
    }
    secret[3] = random(0,10) + '0';
    while(secret[3] == secret[2] || secret[3] == secret[1] || secret[3] == secret[0]){
      secret[3] = random(0,10) + '0';
    }
  }
  else{   
    int i = 0;
    while (i < length) {
      secret[i] = random(0,10) + '0';
      Serial.println(secret[i]);
      i++;
      delay(1000);
    }
  }
  secret[length] = '\0';
  return secret;
}

void play_game(char* secret){
  char attempts[2] = ":";
  char guess[5] = "0000";
  char history[10][5];
  int num = 0;
  int guessedNum = 0;
  int guessedPos = 0;
  int *peg_a;
  int *peg_b;
  int millistime = 500;

  int lastButtonState1 = LOW;
  int lastButtonState2 = LOW;
  int lastButtonState3 = LOW;
  int lastButtonState4 = LOW;
  int lastButtonState5 = LOW;
  
  lcd_print_at(0,0,guess);
  Serial.println(secret);
  lcd_set_cursor(0,1);
  lcd_print("pokusov: 10");
  while(true){
    *peg_a = 0;
    *peg_b = 0;
  
  if(digitalRead(BTN_1_PIN) != lastButtonState1){
    int timer = millis();
    lastButtonState1 = digitalRead(BTN_1_PIN);
    if(digitalRead(BTN_1_PIN) == HIGH){
//      if(timer > millistime){
//        while(digitalRead(BTN_1_PIN) == HIGH){
//            if(digitalRead(BTN_2_PIN) != lastButtonState2){
//               lastButtonState2 = digitalRead(BTN_2_PIN);
//               if(digitalRead(BTN_2_PIN) == HIGH){
//                 Serial.println("tu som button 1");
//               }
//              }
//            if(digitalRead(BTN_3_PIN) != lastButtonState3){
//              lastButtonState3 = digitalRead(BTN_3_PIN);
//              if(digitalRead(BTN_3_PIN) == HIGH){
//                 Serial.println("tu som button 2");
//              }
//            }
//      }
//    }
//    else{
        guess[0] = guess[0] + 1;
        if(guess[0] == ':'){
          guess[0] = '0'; 
        }
        lcd_set_cursor(0,0);
        lcd_print(guess);
        delay(200);
//        }
    }  
  }
 
  if(digitalRead(BTN_2_PIN) != lastButtonState2){
    lastButtonState2 = digitalRead(BTN_2_PIN);
    if(digitalRead(BTN_2_PIN) == HIGH){
      guess[1] = guess[1] + 1;
    if(guess[1] == ':'){
      guess[1] = '0'; 
    }
    lcd_set_cursor(0,0);
    lcd_print(guess);
    }
    delay(200);
  }
 
  if(digitalRead(BTN_3_PIN) != lastButtonState3){
    lastButtonState3 = digitalRead(BTN_3_PIN);
    if(digitalRead(BTN_3_PIN) == HIGH){
      guess[2] = guess[2] + 1;
    if(guess[2] == ':'){
      guess[2] = '0'; 
    }
    lcd_set_cursor(0,0);
    lcd_print(guess);
    }
    delay(200);
  }
  
  if(digitalRead(BTN_4_PIN) != lastButtonState4){
    lastButtonState4 = digitalRead(BTN_4_PIN);
    if(digitalRead(BTN_4_PIN) == HIGH){
      guess[3] = guess[3] + 1;
    if(guess[3] == ':'){
      guess[3] = '0'; 
    }
    lcd_set_cursor(0,0);
    lcd_print(guess);
    }
    delay(200);
  }
  
//    if(digitalRead(BTN_1_PIN) != lastButtonState1){
//      delay(200);
//      lastButtonState1 = digitalRead(BTN_1_PIN);
//      Serial.println("tu som button 1");
//      if(digitalRead(BTN_2_PIN) == HIGH){
//        Serial.println("tu som button 2");
//      }
////      else{
////        Serial.println("tu som button 1 false");
////      }
////      lastButtonState2 = digitalRead(BTN_2_PIN);
////      
////    }
////    if(digitalRead(BTN_1_PIN) == HIGH && digitalRead(BTN_3_PIN) == HIGH){
////      Serial.println("tu som button 2");
//          
//    }
  
  if(digitalRead(BTN_ENTER_PIN) != lastButtonState5){
    lastButtonState5 = digitalRead(BTN_ENTER_PIN);
    if(digitalRead(BTN_ENTER_PIN) == HIGH){
      attempts[0] = attempts[0] - 1;
      strncpy(history[num++], guess, 5);
      lcd_clear();
      lcd_set_cursor(0,0);
      lcd_print(guess);
      lcd_set_cursor(0,1);
      lcd_print("pokusov: ");
      lcd_set_cursor(9,1);
      lcd_print(attempts);
      if(attempts[0] == '0'){
        lcd_clear();
        lcd_print_at(3,0, "Prehral si!");
        delay(2000);
        lcd_clear();
        lcd_print_at(0,0, "spravne cislo");
        lcd_print_at(2,1, "bolo");
        lcd_set_cursor(7,1);
        lcd_print(secret);
        delay(2000);
        return;
      }
      get_score(secret, guess, peg_a, peg_b);
      if(strcmp(secret, guess) == 0){
        lcd_clear();
        lcd_print_at(3,0,"vyhral si!");
        delay(2000);
        return;
      }
      delay(2000);
      turn_off_leds();
    }
  } 
 }
}

void render_leds(const int peg_a, const int peg_b){
//  if(peg_a == 0 && peg_b == 0){
//   digitalWrite(LED_BLUE_1, HIGH);
//   digitalWrite(LED_RED_1, HIGH);
//  }
  if(peg_a == 1 && peg_b == 0){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
  }  
  if(peg_a == 0 && peg_b == 1){
   digitalWrite(LED_BLUE_1, LOW);
   digitalWrite(LED_RED_1, HIGH);
  }
  if(peg_a == 2 && peg_b == 0){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
  }
  if(peg_a == 0 && peg_b == 2){
   digitalWrite(LED_BLUE_1, LOW);
   digitalWrite(LED_RED_1, HIGH);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
  }
  if(peg_a == 3 && peg_b == 0){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
   digitalWrite(LED_BLUE_3, HIGH);
   digitalWrite(LED_RED_3, LOW);
  }
  if(peg_a == 0 && peg_b == 3){
   digitalWrite(LED_BLUE_1, LOW);
   digitalWrite(LED_RED_1, HIGH);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
  }
  if(peg_a == 4 && peg_b == 0){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
   digitalWrite(LED_BLUE_3, HIGH);
   digitalWrite(LED_RED_3, LOW);
   digitalWrite(LED_BLUE_4, HIGH);
   digitalWrite(LED_RED_4, LOW);
  }
  if(peg_a == 0 && peg_b == 4){
   digitalWrite(LED_BLUE_1, LOW);
   digitalWrite(LED_RED_1, HIGH);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
   digitalWrite(LED_BLUE_4, LOW);
   digitalWrite(LED_RED_4, HIGH);
  }
  if(peg_a == 1 && peg_b == 1){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
  }
  if(peg_a == 1 && peg_b == 2){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
  }
  if(peg_a == 1 && peg_b == 3){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, LOW);
   digitalWrite(LED_RED_2, HIGH);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
   digitalWrite(LED_BLUE_4, LOW);
   digitalWrite(LED_RED_4, HIGH);
  }
  if(peg_a == 2 && peg_b == 1){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
  }
  if(peg_a == 2 && peg_b == 2){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
   digitalWrite(LED_BLUE_3, LOW);
   digitalWrite(LED_RED_3, HIGH);
   digitalWrite(LED_BLUE_4, LOW);
   digitalWrite(LED_RED_4, HIGH);
  }
  if(peg_a == 3 && peg_b == 1){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, LOW);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, LOW);
   digitalWrite(LED_BLUE_3, HIGH);
   digitalWrite(LED_RED_3, LOW);
   digitalWrite(LED_BLUE_4, LOW);
   digitalWrite(LED_RED_4, HIGH);
  }
}

void turn_off_leds(){
   digitalWrite(LED_BLUE_1, HIGH);
   digitalWrite(LED_RED_1, HIGH);
   digitalWrite(LED_BLUE_2, HIGH);
   digitalWrite(LED_RED_2, HIGH);
   digitalWrite(LED_BLUE_3, HIGH);
   digitalWrite(LED_RED_3, HIGH);
   digitalWrite(LED_BLUE_4, HIGH);
   digitalWrite(LED_RED_4, HIGH);
}

void render_history(char* secret, char** history, const int entry_nr){
  
}

void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b){
  int a = 0, b = 0;
    if(guess[0] == secret[0]){
      a++;
    } else {
      if(secret[0] == guess[1] || secret[0] == guess[2] || secret[0] == guess[3]){
        b++;
      }
    }
    if(guess[1] == secret[1]){
      a++;
    } else{
      if(secret[1] == guess[0] || secret[1] == guess[2] || secret[1] == guess[3]){
        b++;
        }
      }
    if(guess[2] == secret[2]){
      a++;
    }
    else{
      if(secret[2] == guess[0] || secret[2] == guess[1] || secret[2] == guess[3]){
        b++;
    }
    }
    if(guess[3] == secret[3]){
      a++;
    }
    else{
      if(secret[3] == guess[0] || secret[3] == guess[1] || secret[3] == guess[2]){
        b++;
        }
    }
    render_leds(a, b);
    *peg_a = a;
    *peg_b = b;
}
