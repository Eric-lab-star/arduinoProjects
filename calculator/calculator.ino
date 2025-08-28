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
  BUZZ = 11,
  CLEAR =  12,
  SUM = 13,
  NUMBERS_COUNT,
};

enum OP {
  NONE = 0,
  ADD = 14, //A0
  SUB = 15, //A1
  MULT = 16, //A2
  DIV = 17, //A3
};
unsigned long LH = ZERO;
uint8_t LH_DIGIT = 0;
uint8_t OP_state = NONE;
unsigned long RH = ZERO;
uint8_t RH_DIGIT = 0;

/* 
 *  printOP prints arithmetic
 *  operators to LCD screen
*/
void printOP(uint8_t op){
  switch (op) {
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
}


void setup() {
  // configure number buttons
  for (int i = ZERO; i < NUMBERS_COUNT; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  // configure operators
  for (int i = 14; i < 18; i++) {
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
  for (int i = ZERO; i < 10; i++) {// Number Button Listener
    if (digitalRead(i) == 0) {// Button is Pressed
      if (OP_state == NONE) {
        if (LH * 10 + i >= 4000000000){
          break;
        }
        LH = LH * 10 + i;
        LH_DIGIT++;
        lcd.clear();
        lcd.setCursor(16 - LH_DIGIT, 0);
        lcd.print(LH);
      } else {
        if (RH * 10 + i >= 4000000000){
          break;
        }
        RH = RH * 10 + i;
        RH_DIGIT++;
        lcd.clear();
        lcd.setCursor(16 - LH_DIGIT, 0);
        lcd.print(LH);
        
        lcd.setCursor(15 - RH_DIGIT, 1);
        printOP(OP_state);
        lcd.print(RH);
      }
      delay(200);
    }

  }

  for (int i = 14; i < 18; i++) {// arithmetic button listener
    if (digitalRead(i) == 0 && RH == 0) {
      OP_state = i;
      lcd.clear();
      lcd.setCursor(16 - LH_DIGIT, 0);
      lcd.print(LH);
      lcd.setCursor(15, 1);
      printOP(i);
      delay(200);
    }
  }

  if(digitalRead(CLEAR)== 0){
    LH = ZERO;
    LH_DIGIT = 0;
    OP_state = NONE;
    RH = ZERO;
    RH_DIGIT = 0;
    lcd.clear();
    lcd.home();
    lcd.print("SIMPLE");
    lcd.setCursor(3,1);
    lcd.print("CALCULATOR");
  }

  if (digitalRead(SUM) == 0 ) {
    lcd.clear();
    lcd.home();
    lcd.print("RESULT");
    lcd.setCursor(0, 1);
  switch (OP_state) {
    case ADD:
      lcd.print(LH + RH);
      break;
    case SUB:
      lcd.print(LH - RH);
      break;
    case MULT:
      lcd.print(LH * RH);
      break;
    case DIV:
      lcd.print(LH / RH);
      break;
    default:
      break;
  }
    delay(200);
  }
}
