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

_test() {
	build
	echo
	for testname in tests/*
	do
		filename="TEST_$(basename -- "$testname")"
		filename="${filename%.*}"
		cp .cmake/$filename build/
		echo -e -n "\033[1;4m$testname\033[0m: "
		./build/$filename
		if [ $? -eq 0 ]; then
			echo -e "\033[32mTest passed.\033[0m"
		else
			echo -e "\033[31mTest failed.\033[0m"
		fi
		rm -f build/$filename
	done
}
		

while getopts ":bt-:" option;
do
	case $option in
		b|build)
			build
			;;
		t|test)
			_test
			;;
		-)
			case "${OPTARG}" in
				build)
					build
					;;
				test)
					_test
					;;
			esac
			;;
	esac
done
