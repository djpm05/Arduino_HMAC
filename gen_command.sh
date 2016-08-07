#!/bin/bash

PSK="MYUNIQUEPSK"

echo -n $1"~"
echo -n $1 | openssl dgst -sha256 -hmac $PSK | tr '[[:lower:]]' '[[:upper:]]'
