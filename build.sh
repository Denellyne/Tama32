#!/bin/sh

echo "Converting PNG to XBM..."
cd ./main/Tools/PEmbed/src/
FILES=$(find ../../../Sprites -type f | grep '.png')
make build
../bin/main ../../../Sprites/spritesData.h $FILES
echo "Files converted"
echo "Building esp"
cd ../../../../
idf.py build && idf.py flash monitor
