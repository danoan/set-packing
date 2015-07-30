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

gtest_header_path=../gtest-1.7.0/include

mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(dir $(mkfile_path))

CCFLAGS = -std=c++11 -O3

all: lib

debug: CCFLAGS = -std=c++11 -g
debug: lib

lib: check formulation.o heuristics.o lagrangean_formulation.o dual_lagrangean_method.o util.o subgradient_method.o subgradient_element.o constraint_line.o io.o config.o solution.o
	ar rc $(lib_path)/libpack.a $(object_path)/*.o

test: lib
	g++ -I $(header_path) -I $(gtest_header_path)  $(source_path)/test_io.cpp -o $(bin_path)/test_io.out -D ROOT_PATH="\"$(current_dir)\"" -pthread -L$(lib_path) -lpack -Lext_lib -lgtest $(CCFLAGS)
	g++ -I $(header_path) $(source_path)/test_util.cpp -o $(bin_path)/test_util.out -D ROOT_PATH="\"$(current_dir)\"" -L$(lib_path) -lpack  $(CCFLAGS)

clear:
	rm -r -f $(build)/*

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

formulation.o: check
	g++ -I $(header_path) -o $(object_path)/formulation.o -c $(source_path)/formulation.cpp $(CCFLAGS)

heuristics.o: check
	g++ -I $(header_path) -o $(object_path)/heuristics.o -c $(source_path)/heuristics.cpp $(CCFLAGS)

lagrangean_formulation.o: check
	g++ -I $(header_path) -o $(object_path)/lagrangean_formulation.o -c $(source_path)/lagrangean_formulation.cpp $(CCFLAGS)

dual_lagrangean_method.o: check
	g++ -I $(header_path) -o $(object_path)/dual_lagrangean_method.o -c $(source_path)/dual_lagrangean_method.cpp $(CCFLAGS)

solution.o: check
	g++ -I $(header_path) -o $(object_path)/solution.o -c $(source_path)/solution.cpp $(CCFLAGS)

subgradient_method.o: check
	g++ -I $(header_path) -o $(object_path)/subgradient_method.o -c $(source_path)/subgradient_method.cpp $(CCFLAGS)

subgradient_element.o: check
	g++ -I $(header_path) -o $(object_path)/subgradient_element.o -c $(source_path)/subgradient_element.cpp $(CCFLAGS)

constraint_line.o: check
	g++ -I $(header_path) -o $(object_path)/constraint_line.o -c $(source_path)/constraint_line.cpp $(CCFLAGS)

io.o: check
	g++ -I $(header_path) -o $(object_path)/io.o -c $(source_path)/io.cpp $(CCFLAGS)

util.o: check
	g++ -I $(header_path) -o $(object_path)/util.o -c $(source_path)/util.cpp $(CCFLAGS)

config.o: check
	g++ -I $(header_path) -o $(object_path)/config.o -c $(source_path)/config.cpp $(CCFLAGS)