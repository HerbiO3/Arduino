#!/bin/bash


######################   INSTALLING LIBS ###################
workspace="/home/herbio/Repos/arduino"
arduino_cli="$workspace/arduino-cli"
chmod 0777 "$arduino_cli"
SKETCH_DIR="$workspace/herbio"
library_file="$SKETCH_DIR/library.json"  # Replace with the actual path of library.json
modified_file="$HOME/.arduino_libs_modified"

# Function to install dependencies and save modification date
install_deps() {
  name=$(jq -r '.name' "$library_file")
  dependencies=$(jq -r '.dependencies | to_entries[] | "\(.key)@\(.value)"' "$library_file")

  # Loop through each dependency and install it
  for dependency in $dependencies
  do
    $arduino_cli lib install $dependency
  done

  # Save the current last edit date into the modified file
  last_edit_current=$(stat -c "%y" "$library_file")
  echo "$last_edit_current" > "$modified_file"
}

# Read the modification date from the modified file
if [ -f "$modified_file" ]; then
  mod_file=$(cat "$modified_file")
else
  mod_file=""
fi

# Get the current last edit date of the library.json file
last_edit_current=$(stat -c "%y" "$library_file")

# Compare last edit dates
if [ "$mod_file" = "$last_edit_current" ]; then
  echo "Library.json has not been modified. Skipping installation."
else
  echo "Library.json has been modified."
  install_deps
fi


######################   COMPILING       ###################
 # Compile the sketch
 $arduino_cli compile --build-path $SKETCH_DIR/build --fqbn arduino:avr:leonardo $SKETCH_DIR

 # Check for compilation errors
 if [ $? -ne 0 ]; then
     echo "Compilation failed"
     exit 1
 fi
######################   ULPOADING       ###################
 # Upload the sketch to the Arduino on /dev/ttyACM0
 $arduino_cli upload -p /dev/ttyACM0 --fqbn arduino:avr:leonardo --input-dir $SKETCH_DIR/build -t