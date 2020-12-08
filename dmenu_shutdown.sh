#! /bin/bash

r=$(printf "shutown\nreboot" | dmenu)
if [[ "$r" == "shutdown\n" ]]
then
	exec shutdown 0
fi
if [[ "$r" == "reboot\n" ]]
then
	exec shutdown -r 0
fi
echo "end"
