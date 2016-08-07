#!/bin/bash

PSK="MYUNIQUEPSK"
ARDUINODEV=/dev/cu.Repleo-CH341-00002014
echo "Press Ctrl+c once you get a response. This could definitely be handled better."

echo -n $1"~" > $ARDUINODEV
echo -n $1 | openssl dgst -sha256 -hmac $PSK | tr '[[:lower:]]' '[[:upper:]]' > $ARDUINODEV
echo > $ARDUINODEV

tail -f $ARDUINODEV
