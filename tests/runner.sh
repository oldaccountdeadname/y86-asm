#!/bin/sh

for case in *.s; do
	y86-asm "$case" --output "$case.actual"
	diff "$case.expected" "$case.actual" || { echo "$case failed."; exit 1; }
	echo -e "\033[1;32m$case succeeded\033[0m"
	rm $case.actual
done
