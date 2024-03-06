#!/bin/bash
a=1
while [ $a -le 100 ]
do
	if [ $a -gt 70 ]	   #if loop variable is greater than 70
	then
		rm -rf file$a
	elif [ $a -gt 40 ]         # else if loop variable is great than 40
	then
	# 重命名为newfile
		mv file$a newfile$a
		
	fi
	           #don't forget change the loop variable
	((a++))
done
