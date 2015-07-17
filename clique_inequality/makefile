build=build
source=src
header=include

object=obj
lib=lib
bin=bin

source_path=$(source)
source_path_test=test
header_path=$(header)

object_path=$(build)/$(object)
lib_path=$(build)/$(lib)
bin_path=$(build)/$(bin)

dual_lib=../dual_lagrangean_lib/build/lib
dual_header=../dual_lagrangean_lib/include

all: clear check lib

CCFLAGS = -std=c++11 -O3

debug: CCFLAGS = -std=c++11 -g -D __DEBUG__
debug: clear check lib test_sources

test_sources:
	$(MAKE) debug -C test

lib: clique_inequality.o colision_graph.o dyn_bit_cluster.o pool_clique.o
	ar rc $(lib_path)/libclique.a $(object_path)/*.o

clear:
	rm -r -f $(build)/*

clique_inequality.o:
	g++ -I${header_path} -I${dual_header} -o ${object_path}/clique_inequality.o -c ${source_path}/clique_inequality.cpp -L${dual_lib} -lpack -lm ${CCFLAGS}

colision_graph.o:
	g++ -I${header_path} -I${dual_header} -o ${object_path}/colision_graph.o -c ${source_path}/colision_graph.cpp -L${dual_lib} -lpack -lm ${CCFLAGS}

dyn_bit_cluster.o:
	g++ -I${header_path} -I${dual_header} -o ${object_path}/dyn_bit_cluster.o -c ${source_path}/dyn_bit_cluster.cpp -L${dual_lib} -lpack -lm ${CCFLAGS}	

pool_clique.o:
	g++ -I${header_path} -I${dual_header} -o ${object_path}/pool_clique.o -c ${source_path}/pool_clique.cpp -L${dual_lib} -lpack -lm ${CCFLAGS}	

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)
