# RFID-keylock
The project uses:
1) RFID RC522 is connected via SPI bus
2) LCD 1602 is connected via I2C bus
3) Servo SG92 is connected to digital pin 8
4) Buzzer is connected to digital pin 3

If you want to change the connection, change the variables:
int piezoPin = 3; // The pin to which the piezo speaker is connected.
int ServoPin = 8; // servo pin

To add new labels, fill in the array separated by commas:
unsigned long Arr_uid [] = {1696461098, 2082820387}; // An array of permission labels

The code does not use delay () so the Arduino works without stopping, this makes it possible to complete the code.
