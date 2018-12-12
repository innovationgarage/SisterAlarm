/* The Princess Ophelias Sister Alarm

 * Do you have a sneaky snooping little sister or brother? We know you
 * always want to keep them out of your room with the most high-tech
 * solution available.

 * The following is a tutorial project for building a motion sensor
 * based alarm that is activated/deactivated with an RFID card / tag.
 * It should be suitably loud and annoying to your snooping little
 * friend.

 * Homepage: https://github.com/innovationgarage/SisterAlarm

 * Copyright: 2018, Innovationgarage AS
 * License: MIT

 */

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <MicroLCD.h>

/* You will have to change this:

 * Princess Ophelias card id. To figure out the id of your card so you
 * can update this, program the Arduino and swipe your card. The card
 * id will be displayed on the screen.
 */
byte ownerCardId[4] = {0x3a, 0xf4, 0x7f, 0xb9};

/* You might have to change this if you're using a different Arduino
 * model:

 * Our Arduino pin mapping
 */
#define SS_PIN 10
#define RST_PIN 9 // Not really used, but required by the rfid module
#define BUZZER_PIN 6
#define PIR_PIN 9

LCD_SH1106 lcd; 
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 


// Current state
bool isEngaged = 0; // Is the alarm engaged and ready to beep if the PIR sensor is activated?
bool pirLastVal = LOW; // Previous state of the PIR sensor, used to detect changes in its state.

void setup() { 
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.begin();
  SPI.begin();
  rfid.PCD_Init();
  // We print everything to the serial too for easier debugging...
  Serial.begin(9600);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  displayState();
}

void displayState() {
  if (isEngaged) {
    Serial.println(F("engaged"));
    lcd.clear();
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.setCursor(0, 0);
    lcd.print("PRIVATE AREA");
    lcd.setCursor(0, 2);
    lcd.print("KEEP OUT!");
  } else {
    Serial.println(F("NOT engaged"));
    lcd.clear();
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.setCursor(0, 0);
    lcd.print("At your");
    lcd.setCursor(0, 2);
    lcd.print("service!");
  }
}
 
void loop() {
  bool pirInVal;
  
  if (isEngaged) {
    pirInVal = digitalRead(PIR_PIN);
    if (pirInVal != pirLastVal) {
      if (pirInVal) {
        digitalWrite(BUZZER_PIN, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.setFontSize(FONT_SIZE_LARGE);
        lcd.print("UNDER");
        lcd.setCursor(0, 4);
        lcd.setFontSize(FONT_SIZE_LARGE);
        lcd.print("ATTACK");
      
        Serial.println("UNDER ATTACK");
      } else {
        digitalWrite(BUZZER_PIN, LOW);
        displayState();
        Serial.println("END OF ATTACK");
      }
      pirLastVal = pirInVal;
    }
  }

  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    // The card is not a Classic MIFARE card, so we just tell the
    // sneaky snooper to try something smarter than their old bus
    // card, right?

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.print("What is this sorcery?!");
      
    Serial.println(F("What is this sorcery?!"));
    return;
  }

  if (rfid.uid.uidByte[0] != ownerCardId[0] || 
    rfid.uid.uidByte[1] != ownerCardId[1] || 
    rfid.uid.uidByte[2] != ownerCardId[2] || 
    rfid.uid.uidByte[3] != ownerCardId[3] ) {

    Serial.println(F("You sneaky snooper!\nYour tag:"));
    printHex(rfid.uid.uidByte, rfid.uid.size);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.setFontSize(FONT_SIZE_SMALL);
    lcd.print("You sneaky snooper!");
    lcd.setCursor(0, 1);
    lcd.print("Your tag:");
    lcd.setCursor(0, 2);
    lcdPrintHex(rfid.uid.uidByte, rfid.uid.size);
  }
  else {
    if (isEngaged) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.print("Welcome,");
      lcd.setCursor(0, 1);
      lcd.print("my princess!");
      Serial.println(F("Welcome my princess!"));
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.print("Clear the");
      lcd.setCursor(0, 1);
      lcd.print("area");
      Serial.println(F("Clear the area"));
    }
    delay(2000);
    isEngaged = !isEngaged;

    displayState();
  }

  // Close/stop the RFID reader and clean up its state
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

/* Helper routines to print a card id as hex to the lcd and the serial
 * port.
 */

void lcdPrintHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    lcd.print(buffer[i] < 0x10 ? " 0" : " ");
    lcd.print(buffer[i], HEX);
  }
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
