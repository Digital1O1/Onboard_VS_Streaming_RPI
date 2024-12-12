#!/bin/bash

# Configuration
CMD_FILE="/tmp/laptop_commands.txt"

# Ensure the command file exists
if [ ! -f "$CMD_FILE" ]; then
    touch "$CMD_FILE"
fi

# Monitor the command file for new commands
echo "Monitoring $CMD_FILE for new commands..."
tail -f -n0 "$CMD_FILE" | while read -r command; do
    if [ -n "$command" ]; then
        echo "Executing: $command"
        eval "$command"
    fi
done
