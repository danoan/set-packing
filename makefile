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


gtest_header_path=gtest-1.7.0/include

mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(dir $(mkfile_path))

main: check lib
	g++ -I $(header_path) main.cpp -o $(bin_path)/main.out -lm -L$(lib_path) -lpack -O3 -w

problem_gen: check lib
	g++ -I $(header_path) $(source_path)/problem_gen.cpp -o $(bin_path)/problem_gen.out -O3 -w

test: lib
	g++ -I $(header_path) -I $(gtest_header_path)  $(source_path)/test_util.cpp -o $(bin_path)/test_util.out -D ROOT_PATH="\"$(current_dir)\"" -pthread -L$(lib_path) -lpack -lgtest -O3 -w

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(source_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

lib: formulation.o heuristics.o lagrangean_formulation.o pure_lagrangean_solver.o subgradient_solver.o util.o 
	ar rc $(lib_path)/libpack.a $(object_path)/*.o

formulation.o: check
	g++ -I $(header_path) -o $(object_path)/formulation.o -c $(source_path)/formulation.cpp -O3 -w

heuristics.o: check
	g++ -I $(header_path) -o $(object_path)/heuristics.o -c $(source_path)/heuristics.cpp -O3 -w

lagrangean_formulation.o: check
	g++ -I $(header_path) -o $(object_path)/lagrangean_formulation.o -c $(source_path)/lagrangean_formulation.cpp -O3 -w

problem_gen.o: check
	g++ -I $(header_path) -o $(object_path)/problem_gen.o -c $(source_path)/problem_gen.cpp -O3 -w

pure_lagrangean_solver.o: check
	g++ -I $(header_path) -o $(object_path)/pure_lagrangean_solver.o -c $(source_path)/pure_lagrangean_solver.cpp -O3 -w	

subgradient_solver.o: check
	g++ -I $(header_path) -o $(object_path)/subgradient_solver.o -c $(source_path)/subgradient_solver.cpp -O3 -w	

util.o: check
	g++ -I $(header_path) -o $(object_path)/util.o -c $(source_path)/util.cpp -O3 -w