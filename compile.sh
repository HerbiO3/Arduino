#!/bin/bash

# Read the name and dependencies from libs.json
name=$(jq -r '.name' library.json)
dependencies=$(jq -r '.dependencies | to_entries[] | "\(.key)@\(.value)"' library.json)

# Loop through each dependency and install it
for dependency in $dependencies
do
  arduino-cli lib install $dependency
done

cd herbio

# Compile the sketch
arduino-cli compile --fqbn arduino:avr:leonardo .

# Check for compilation errors
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

# Upload the sketch to the Arduino on /dev/ttyACM0 
arduino-cli upload -p /dev/ttyACM0  --fqbn arduino:avr:leonardo .
