build=build
bin=bin
object=obj
lib=lib

object_path=$(build)/object
lib_path=$(build)/lib
bin_path=$(build)/bin

source_path=src
header_path=include

all: lib main.out

lib: clear check minknap.o pisinger.o
	ar rc $(lib_path)/libpisinger.a  $(object_path)/minknap.o $(object_path)/pisinger.o

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

minknap.o:
	gcc -I$(header_path) -o $(object_path)/minknap.o -c $(source_path)/minknap.c

pisinger.o:
	g++ -I$(header_path) -o $(object_path)/pisinger.o -c $(source_path)/pisinger.cpp

main.out:
	g++ -I$(header_path) -o $(bin_path)/main.out $(source_path)/main.cpp -L$(lib_path) -lpisinger

clear:
	rm -f -r $(build)/*