#!/bin/bash

run_cmake() {
	mkdir -p .cmake
	cmake -B .cmake -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
}

usage() {
	echo -e "\033[1mEngine CTL Usage\033[0m\n"

	echo -e -n "\t\033[1m-b/--build\033[0m: Builds all executables (files with sources found in the ./exe directory). "
	echo -e "You can find the executable binaries in the ./build directory\n"

	echo -e "\t\033[1m-r/--run\033[0m: Builds the project, then runs the blunder-ai executable\n"

	echo -e "\t\033[1m-t/--test\033[0m: Builds the project, then runs all unit tests\n"

	echo -e "\t\033[1m-h/--help\033[0m: Displays the usage guide\n"

	echo -e "\033[1mExample\033[0m:"
	echo -e "./enginectl.sh --run"
}

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
	rm ./compile_commands.json
	cp .cmake/compile_commands.json .
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

run() {
	build
	echo
	echo -e "\033[1;94mProgram Output:\033[0m"
	echo
	./build/blunder-ai
}
		

while getopts ":btrh-:" option;
do
	case $option in
		b|build)
			run_cmake
			build
			;;
		t|test)
			run_cmake
			_test
			;;
		r|run)
			run_cmake
			run
			;;
		h|help)
			usage
			;;
		-)
			case "${OPTARG}" in
				build)
					run_cmake
					build
					;;
				test)
					run_cmake
					_test
					;;
				run)
					run_cmake	
					run
					;;
				help)
					usage
					;;
			esac
			;;
	esac
done
