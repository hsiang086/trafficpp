#!/bin/bash
SCRIPT_DIR=$(dirname "$(realpath "${BASH_SOURCE[0]}")")
FILE="$SCRIPT_DIR/Trafficpp"

bold() {
    echo -e "\033[1m$1\033[0m"
}

green() {
    echo -e "\033[32m$1\033[0m"
}

red() {
    echo -e "\033[31m$1\033[0m"
}

if [ "$(id -u)" -ne "0" ]; then
    red "This script must be run as root. Please use sudo."
    exit 1
fi

if [ ! -f "$FILE" ]; then
    red "File $FILE does not exist."
    exit 1
fi

echo -n "Changing ownership of $FILE to root... "
if sudo chown root "$FILE"; then
    green "Success"
else
    red "Failed"
    exit 1
fi

echo -n "Setting setuid bit on $FILE... "
if sudo chmod u+s "$FILE"; then
    green "Success"
else
    red "Failed"
    exit 1
fi

bold "Done! The file $FILE is now owned by root and has the setuid bit set."
