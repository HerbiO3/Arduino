cd herbio
# Install the dependencies
arduino-cli lib install --dependencies-only .

# Compile the sketch
arduino-cli compile --fqbn arduino:avr:leonardo .

# Check for compilation errors
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

# Upload the sketch to the Arduino on /dev/tty0
arduino-cli upload -p /dev/tty0 --fqbn arduino:avr:leonardo .
