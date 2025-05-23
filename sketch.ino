#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String currentChocolate = "";
String quantityInput = "";
int currentQuantity = 0;
int chocolateAngle = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Select Choco:");
  myServo.attach(10);
  myServo.write(0);
  delay(1000);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (key == 'A') {
      currentChocolate = "KitKat";
      chocolateAngle = 45;
      lcd.print("Selected: KitKat");
      quantityInput = "";
      myServo.write(chocolateAngle);
    }
    else if (key == 'B') {
      currentChocolate = "Snickers";
      chocolateAngle = 90;
      lcd.print("Selected: Snickers");
      quantityInput = "";
      myServo.write(chocolateAngle);
    }
    else if (key == 'C') {
      currentChocolate = "Dairy Milk";
      chocolateAngle = 135;
      lcd.print("Selected: Dairy Milk");
      quantityInput = "";
      myServo.write(chocolateAngle);
    }
    else if (key >= '0' && key <= '9') {
      if (currentChocolate != "") {
        if (quantityInput.length() < 2) {
          quantityInput += key;
          lcd.print("Qty: " + quantityInput);
        } else {
          lcd.print("Max 2 digits!");
        }
      } else {
        lcd.print("Select Choco first");
      }
    }
    else if (key == '#') {
      if (quantityInput != "") {
        currentQuantity = quantityInput.toInt();
        if (currentQuantity > 0 && currentQuantity <= 50) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Dispensing...");
          lcd.setCursor(0, 1);
          lcd.print(String(currentQuantity) + " " + currentChocolate);

          // Move servo from choco angle → 180 → 0
          myServo.write(chocolateAngle);
          delay(500);
          myServo.write(180);
          delay(800);
          myServo.write(0);
          delay(800);

          // Reset all
          currentChocolate = "";
          quantityInput = "";
          currentQuantity = 0;
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Select Choco:");
        } else {
          lcd.print("Limit: 1 to 50");
        }
      } else {
        lcd.print("No qty entered");
      }
    }
    else if (key == '*') {
      currentChocolate = "";
      quantityInput = "";
      currentQuantity = 0;
      chocolateAngle = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select Choco:");
    }
    else {
      lcd.print("Invalid input");
    }

    delay(1500);
  }
}