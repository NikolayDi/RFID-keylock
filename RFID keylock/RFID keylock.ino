/*
The project was developed by Nikolay Dmitrenko in 2021
*/
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
int piezoPin = 3; //The pin to which the piezo speaker is connected.
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long uidDec, uidDecTemp;  // to store the label number in decimal format
unsigned long timer_servo, timer_cycle; // Variable to store the reference point
unsigned long Arr_uid[] = {1696461098, 2082820387};// An array of permission labels
bool flag = true;
bool found = false;
Servo servo;

void setup() {
  Serial.begin(9600);
  lcd.init();    // initialize the lcd 
  lcd.backlight();
  SPI.begin();  // Init SPI bus.
  mfrc522.PCD_Init();  // Init MFRC522 card.
  servo.attach(8); // servo pin
  servo.write(0);  // set the servo to a closed state
}

void loop() {
  //check if 3sk has passed since the last mark was found
  if(millis() - timer_cycle >= 3000){
    if(flag){
      lcd.clear();
      lcd.print("Waiting for card");
      flag = false;
    }
    if (millis() - timer_servo >= 3000){ 
    servo.write(0);  // set the servo to a closed state
    }
    // Finding a new tag
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Selecting a label
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    uidDec = 0;
    // Issuing the serial number of the tag.
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      uidDecTemp = mfrc522.uid.uidByte[i];
      uidDec = uidDec * 256 + uidDecTemp;
    }
    found = false;
    //Searching the array of our label
    for(int i = 0; i < sizeof(Arr_uid)/sizeof(unsigned long); i++){
      if(uidDec == Arr_uid[i]){found = true;}
    }
    if (found) // if found, the servo opens.
    {
      tone(piezoPin, 50, 100); // We make a sound signal, Found
      lcd.clear(); //clear lcd
      lcd.print("Found"); // output to lcd
      servo.write(180); // Rotate the servo 180 degrees
      timer_servo = millis();
      timer_cycle = millis();
      flag = true;
    }
    else{
      tone(piezoPin, 2000, 100); // Beep, Not Found
      lcd.clear();//clear lcd
      lcd.print("Not found");// output to lcd
      timer_cycle = millis();
      flag = true;
    }  
  } 
}
