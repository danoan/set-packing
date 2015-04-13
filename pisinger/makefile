build=build
bin=bin
object=obj
lib=lib

object_path=$(build)/object
lib_path=$(build)/lib
bin_path=$(build)/bin

source_path=src
header_path=include

all: clear check minknap.o main.o main.out

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

minknap.o:
	gcc -I$(header_path) -o $(object_path)/minknap.o -c $(source_path)/minknap.c

main.o:
	g++ -I$(header_path) -o $(object_path)/main.o -c $(source_path)/main.cpp

main.out:
	g++ -I$(header_path) -o $(bin_path)/main.out $(object_path)/main.o $(object_path)/minknap.o

clear:
	rm -f -r $(build)/*