#include <LiquidCrystal.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
}


void loop() {
  lcd.setCursor(0, 1);
  lcd.print(" LCD Tutorial :)");
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print(" googooloo");
  delay(100);

}
