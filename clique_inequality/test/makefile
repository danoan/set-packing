build=build
source=src
header=include

object=obj
lib=lib
bin=bin

source_path=$(source)
build=build
source=src
header=include

object=obj
lib=lib
bin=bin

source_path=$(source)
header_path=$(header)

object_path=$(build)/$(object)
lib_path=$(build)/$(lib)
bin_path=$(build)/$(bin)

dual_lib=../../dual_lagrangean_lib/build/lib
dual_header=../../dual_lagrangean_lib/include

clique_lib=../build/lib
clique_header=../include

all: clear check main

CCFLAGS = -std=c++11 -O3

debug: CCFLAGS = -std=c++11 -g -D __DEBUG__
debug: clear check main

main: 
	g++ -I${header_path} -I${dual_header} -I${clique_header} main.cpp ${source_path}/*.cpp -o ${bin_path}/main.out -L${lib_path} -L${dual_lib} -L${clique_lib} -lpack -lclique ${CCFLAGS}

clear:
	rm -r -f $(build)/*

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)
