The Smart Home Security System is an innovative project designed to enhance home security by integrating various technologies. This system utilizes a combination of a Passive Infrared (PIR) motion sensor, RFID card reader, keypad input, and visual display through an LCD. The system allows users to secure their homes effectively by granting access via either an authorized RFID card or a password. It aims to provide peace of mind by ensuring that only authorized individuals can enter the premises while also alerting users in case of unauthorized access attempts.

Working Method:

1. Initialization:
   - The system begins by setting up the required components, including the PIR sensor, RFID module, LCD display, servo motor, and buzzer. All pins are initialized, and the LCD is cleared for a fresh start.

2. Main Loop:
   - The system continuously monitors the PIR sensor for motion. When motion is detected, it displays a menu on the LCD with two options: entering a password or scanning an RFID card.

3. Password Entry:
   - If the user selects the password option, they are prompted to enter a 4-digit password using the keypad. The entered digits are masked for privacy. If the correct password is entered, access is granted, and the servo motor activates to unlock the entry.

4. RFID Scanning:
   - If the user opts to scan an RFID card, the system waits for a card to be presented. Upon scanning, the UID of the card is compared with the authorized UID. If they match, access is granted; otherwise, an alarm is triggered.

5. Access Control:
   - The servo motor controls physical access, opening the entry when authorized. If access is denied, the buzzer sounds to alert that an unauthorized attempt has occurred.

This project effectively combines hardware and software components to create a user-friendly and secure home environment.
