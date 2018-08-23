#! /bin/bash
while [ 1 ]
do
	sudo /home/pi/projects/FruitPicker/FruitPicker
	if [ $? -eq 0 ]
	then
		break
	fi
done


