#!/bin/bash

mkdir -p .cmake
cmake -B .cmake -S .

build() {
	mkdir -p build
	cd .cmake
	make
	for execname in ../exe/* 
	do
		filename=$(basename -- "$execname")
		cp ${filename%.*} ../build
	done
	echo -e "\033[1;4;32mDone building, executables stored in the build directory\033[0m"
	cd ..
}

while getopts ":b-:" option;
do
	case $option in
		b|build)
			build
			;;
		-)
			case "${OPTARG}" in
				build)
					build
					;;
			esac
			;;
	esac
done
