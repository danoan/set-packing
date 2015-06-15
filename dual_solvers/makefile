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

header_dual_lib=../dual_lagrangean_lib/include
lib_dual_lib=../dual_lagrangean_lib/build/lib

header_pisinger_lib=../pisinger_lib/original/include
lib_pisinger_lib=../pisinger_lib/build/lib

header_ext=ext/include
lib_ext=ext/lib

CCFLAGS = -O3 -w

all: main

debug: CCFLAGS = -g -w
debug: main

main: check lib
	g++ -I $(header_pisinger_lib) -I $(header_path) -I $(header_dual_lib) -I $(header_ext) main.cpp -o $(bin_path)/main.out -lm -L$(lib_path) -lsolver -L$(lib_dual_lib) -lpack -L$(lib_pisinger_lib) -lpisinger -L$(lib_ext) $(CCFLAGS)
	g++ -I $(header_pisinger_lib) -I $(header_path) -I $(header_dual_lib) -I $(header_ext) batch_run.cpp -o $(bin_path)/batch_run.out -lm -L$(lib_path) -lsolver -L$(lib_dual_lib) -lpack -L$(lib_pisinger_lib) -lpisinger -L$(lib_ext) $(CCFLAGS)


lib: simple_dual_solver.o minknap_dual_solver.o real_to_int.o
	ar rc $(lib_path)/libsolver.a $(object_path)/*.o

clear:
	rm -r -f $(build)/*

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

real_to_int.o: check
	g++ -I $(header_path) -I $(header_dual_lib) -o $(object_path)/real_to_int.o -c $(source_path)/real_to_int.cpp -lm $(CCFLAGS)	

simple_dual_solver.o: check
	g++ -I $(header_path) -I $(header_dual_lib) -o $(object_path)/simple_dual_solver.o -c $(source_path)/simple_dual_solver.cpp $(CCFLAGS)

minknap_dual_solver.o: check
	g++ -I $(header_path) -I $(header_dual_lib) -I $(header_pisinger_lib) -I $(header_ext) -o $(object_path)/minknap_dual_solver.o -c $(source_path)/minknap_dual_solver.cpp -L$(lib_ext) -L$(lib_pisinger_lib) -lpisinger $(CCFLAGS)