#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_74HC595.h>
#include <Servo.h>
int pirPin = 7;
int ledPin = 6;
int pirValue;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

#define DS 3
#define SHCP 4
#define STCP 2
#define RS 1
#define E 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
LiquidCrystal_74HC595 lcd(DS, SHCP, STCP, RS, E, D4, D5, D6, D7);
Servo servo;
int thresholds[16] = {930, 912, 894, 877, 852, 837, 822, 808, 787, 774, 761, 749, 731, 719, 708, 
698};
char keypad[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
char enteredPassword[5]; // Array to store entered password (4 digits + null terminator)
const int buzzerPin = 5;
// Declaration of the getUIDAsString function
String getUIDAsString();
void setup() {
 pinMode(pirPin, INPUT);
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW); // Ensure LED is initially off
 Serial.begin(9600);
 SPI.begin();
 mfrc522.PCD_Init();
 lcd.begin(16, 2);
 servo.attach(8); // Attach servo to pin 8
 servo.write(0); // Initialize the servo in the closed position
 delay(1000); // Add a delay to allow the servo to reach the closed position
 pinMode(buzzerPin, OUTPUT);
 // PIR Sensor setup
 pinMode(pirPin, INPUT);
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);
}
void loop() {
 pirValue = digitalRead(pirPin);
 digitalWrite(ledPin, pirValue);
 lcd.clear();
 lcd.print("1. PASSWORD");
 lcd.setCursor(0, 1);
 lcd.print("2. UID SCAN");
 char key = getKey();
 switch (key) {
 case '1':
 enterPassword();
 break;
 case '2':
 scanRFID();
 break;
 }
}
char getKey() {
 char key = '\0';
 while (key == '\0') {
 int value = analogRead(A0);
 for (int i = 0; i < 16; i++) {
 if (abs(value - thresholds[i]) < 5) {
 key = keypad[i];
 while (analogRead(A0) > 0) {
 delay(100);
 }
 }
 }
 }
 return key;
}
void enterPassword() {
 lcd.clear();
 lcd.print("Enter Password:");
 lcd.setCursor(0, 1);
 int passwordIndex = 0;
 boolean passwordEntered = false;
 while (!passwordEntered) {
 char key = getKey();
 if (key >= '0' && key <= '9') {
 lcd.print('*');
 enteredPassword[passwordIndex] = key;
 enteredPassword[passwordIndex + 1] = '\0'; // Null terminator for string
 passwordIndex++;
 if (passwordIndex == 4) {
 passwordEntered = true;
 }
 }
 }
 lcd.clear();
 lcd.print("Entered Password:");

 // Display the entered password on the LCD
 for (int i = 0; i < 4; i++) {
 lcd.print('*');
 delay(500);
 }
 lcd.clear();
 // Check the entered password
 if (strcmp(enteredPassword, "2222") == 0) {
 lcd.print("Access Granted");
 delay(3000);
 openServo();
 } else {
 lcd.print("Access Denied");
 buzzAlarm();
 delay(3000);
 }
 // Reset the entered password for the next attempt
 strcpy(enteredPassword, "");
}
void scanRFID() {
 lcd.clear();
 lcd.print("Scan RFID Card");
 // Check for RFID card
 while (!mfrc522.PICC_IsNewCardPresent()) {
 delay(100);
 }

 if (mfrc522.PICC_ReadCardSerial()) {
 String uid = getUIDAsString();
 uid.trim(); // Remove leading and trailing spaces
 uid.toUpperCase(); // Convert to uppercase
 lcd.clear();
 lcd.print("RFID UID: ");
 lcd.setCursor(0, 1);
 lcd.print(uid);
 // Debugging: Print UID to Serial Monitor for analysis
 Serial.print("Scanned UID: ");
 Serial.println(uid);
 if (uid == "D399F50E") {
 lcd.clear();
 lcd.print("Access Granted");
 delay(3000);
 openServo();
 } else {
 lcd.clear();
 lcd.print("Access Denied");
 lcd.setCursor(0, 1);
 buzzAlarm();
 delay(3000);
 }
 // Halt until the card is removed
 while (mfrc522.PICC_IsNewCardPresent()) {
 delay(100);

 }
 }
}
String getUIDAsString() {
 String uid = "";
 for (byte i = 0; i < mfrc522.uid.size; i++) {
 uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
 uid.concat(String(mfrc522.uid.uidByte[i], HEX));
 }
 return uid;
}
void openServo() {
 servo.write(90); // Rotate the servo to 90 degrees (open position)
 delay(5000); // Keep the servo open for 5 seconds
 servo.write(0); // Rotate the servo back to 0 degrees (closed position)
 delay(1000); // Delay to allow the servo to return to the closed position
}
void buzzAlarm() {
 tone(buzzerPin, 1000); // Buzz the buzzer at 1000 Hz
 delay(2000); // Buzz for 2 seconds
 noTone(buzzerPin); // Turn off the buzzer
}
