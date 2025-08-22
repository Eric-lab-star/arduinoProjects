#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

enum Numbers {
  ZERO = 0,
  ONE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  NUMBERS_COUNT,
};

enum OP {
  NONE = 0,
  ADD = 14,
  SUB = 15,
  MULT = 16,
  DIV = 17,
  CAL = 18,
};
unsigned long LH = ZERO;
uint8_t LH_DIGIT = 0;
uint8_t OP_state = NONE;
unsigned long RH = ZERO;
uint8_t RH_DIGIT = 0;

void setup() {
  for (int i = ZERO; i < NUMBERS_COUNT; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  for (int i = 14; i < 19; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  lcd.init();               
  lcd.backlight();

  lcd.home();
  lcd.print("SIMPLE");
  lcd.setCursor(3,1);
  lcd.print("CALCULATOR");
}

void loop() {
  for (int i = ZERO; i < NUMBERS_COUNT; i++) {
    if (digitalRead(i) == 0) {
      if (OP_state == NONE) {
        if (LH * 10 + i > 4000000000){
          continue;
        }

        LH = LH * 10 + i;
        LH_DIGIT++;
        lcd.clear();
        lcd.setCursor(16 - LH_DIGIT, 0);
        lcd.print(LH);
      } else {
        if (RH * 10 + i > 4000000000){
          continue;
        }
        RH = RH * 10 + i;
        RH_DIGIT++;
        lcd.clear();
        lcd.setCursor(16 - LH_DIGIT, 0);
        lcd.print(LH);
        
        lcd.setCursor(15 - RH_DIGIT, 1);
        switch (OP_state) {
          case ADD:
            lcd.print("+");
          break;
          case SUB:
            lcd.print("-");
          break;
          case MULT:
            lcd.print("x");
          break;
          case DIV:
            lcd.print("÷");
            break;
          default:
            break;
        }
        lcd.print(RH);
      }
      delay(200);
    }

  }

  for (int i = 14; i < 19; i++) {
    if (digitalRead(i) == 0 && RH == 0) {
      OP_state = i;
      lcd.clear();
      lcd.setCursor(16 - LH_DIGIT, 0);
      lcd.print(LH);
      lcd.setCursor(15, 1);
      switch (i) {
        case ADD:
          lcd.print("+");
          break;
        case SUB:
          lcd.print("-");
          break;
        case MULT:
          lcd.print("x");
          break;
        case DIV:
          lcd.print("÷");
          break;
        default:
          continue;
      }
      delay(200);
    }

  }

  if (digitalRead(CAL) == 0 ) {
    lcd.clear();
    lcd.home();
    lcd.print("RESULT");
    lcd.setCursor(0, 1);
    switch (OP_state) {
      case ADD:
        lcd.print("=");
        lcd.print(LH + RH);
        break;
      case SUB:
        lcd.print("=");  
        lcd.print(LH - RH);
        break;
      case MULT:
        lcd.print("=");
        lcd.print(LH * RH);
        break;
      case DIV:
        lcd.print("=");
        lcd.print(LH / RH);
        break;
    }
    delay(200);
  }

}
