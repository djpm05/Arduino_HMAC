/*
 * Copyright (C) 2015 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

 /*
  *PrintHex routines for Arduino: to print byte or word data in hex with
  *leading zeroes.
  *Copyright (C) 2010 Kairama Inc
  *
  *This program is free software: you can redistribute it and/or modify
  *it under the terms of the GNU General Public License as published by
  *the Free Software Foundation, either version 3 of the License, or
  *(at your option) any later version.
  *
  *This program is distributed in the hope that it will be useful,
  *but WITHOUT ANY WARRANTY; without even the implied warranty of
  *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *GNU General Public License for more details.
  *
  *You should have received a copy of the GNU General Public License
  *along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */


/*
 * For the String to char conversion, credit to: http://stackoverflow.com/users/15813/alex-king for the post: http://stackoverflow.com/a/23880116
*/

/*
 * For the getValue function, credit to: http://arduino.stackexchange.com/users/1115/oharkins for the post: http://arduino.stackexchange.com/a/1237
 */

/*
 * This is a super simple script that takes input from the Serial Monitor in the format <data>~<HMAC of data>, if it validates it will tell you, if it fails, it will also tell you.
 */

#include <Crypto.h>
#include <SHA256.h>
#include <string.h>

#define HASH_SIZE 32
#define BLOCK_SIZE 64

const char *psk = "MYUNIQUEPSK";
char *Received;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

String combined = "";
struct ToCrypt
{
    const char *key;
    const char *data;
};

SHA256 sha256;

byte buffer[128];

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String printHex(int num, int precision) {
     char tmp[16];
     char format[128];

     sprintf(format, "%%.%dX", precision);

     sprintf(tmp, format, num);
     //Serial.print(tmp);
     combined += tmp;
     return combined;
}




String calcHMAC(Hash *hash, const struct ToCrypt *calc) {
    uint8_t result[HASH_SIZE];

    // Make sure we've cleared out from the last time.
    combined = "";
    combined += calc->data;
    combined += "~";

    hash->resetHMAC(calc->key, strlen(calc->key));
    hash->update(calc->data, strlen(calc->data));
    hash->finalizeHMAC(calc->key, strlen(calc->key), result, sizeof(result));

        for (int i = 0; i < sizeof(result); i++) {
          combined = printHex(result[i], 2);
        }
        return combined;
}

void serialEvent() {
  while (Serial.available()) {
    String Check;
    String SerialReturn;
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      inputString.trim();
      char sep = '~';
      Check = getValue(inputString, sep, 0);
      int check_len = inputString.length() + 1; 
      char mycheck[check_len];
      Check.toCharArray(mycheck, check_len);
      ToCrypt DataToCheck = {
        psk,
        mycheck
      };
      SerialReturn = calcHMAC(&sha256, &DataToCheck);
      if (SerialReturn.equals(inputString)) {
        // They have provided us a valid command, go ahead and do something with it.
        Serial.println("Command is valid: " + inputString);
      } else {
        // They have send us crap, don't do anything with it.
        Serial.println("Bad command: " + inputString);
      }
    }
  }
}


void setup() {
    Serial.begin(9600);
    while (!Serial) {
    }
    inputString.reserve(200);

}
void loop() {
    if (stringComplete) {
      stringComplete = false;
      // clear the string:
      inputString = "";
    }    
}
