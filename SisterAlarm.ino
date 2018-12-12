#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4] = {0x3a, 0xf4, 0x7f, 0xb9};

#define buzzerPin  6

bool is_engaged;

#define pirPin 9
bool pirlastval = LOW;

void setup() { 
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  lcd.begin();
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  is_engaged = 0;

  display_state();
}

void display_state() {
  if (is_engaged) {
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
  bool pirinval;
  
  if (is_engaged) {
    pirinval = digitalRead(pirPin);
    if (pirinval != pirlastval) {
      if (pirinval) {
        digitalWrite(buzzerPin, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.setFontSize(FONT_SIZE_LARGE);
        lcd.print("UNDER");
        lcd.setCursor(0, 4);
        lcd.setFontSize(FONT_SIZE_LARGE);
        lcd.print("ATTACK");
      
        Serial.println("UNDER ATTACK");
      } else {
        digitalWrite(buzzerPin, LOW);
        display_state();
        Serial.println("END OF ATTACK");
      }
      pirlastval = pirinval;
    }
  }

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.print("What is this sorcery?!");
      
    Serial.println(F("What is this sorcery?!"));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {

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
    if (is_engaged) {
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
    is_engaged = !is_engaged;

    display_state();
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void lcdPrintHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    lcd.print(buffer[i] < 0x10 ? " 0" : " ");
    lcd.print(buffer[i], HEX);
  }
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
