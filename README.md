# Arduino_HMAC
Use the Arduino to validate incoming commands with pre-shared key.

# How to use
Set a unique pre-shared key between the Arduino and the scripts. The scripts and/or using OpenSSL will generate a string of text in the format: <DataToHMAC>~<HMACofData> Send that whole string to the Arduino using the Serial monitor, or script, and it will respond that it has validated the data that was sent to it. As long as the keys match, and you send the data with a Newline character it should respond correctly.
