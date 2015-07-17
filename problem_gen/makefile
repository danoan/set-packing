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

main: check
	g++ -I $(header_path) -I $(header_dual_lib) problem_gen.cpp -o $(bin_path)/problem_gen.out -L$(lib_dual_lib) -lpack -std=c++11 -O3

check:
	test -d $(build) || mkdir $(build)

	test -d $(bin_path) || mkdir $(bin_path)
	test -d $(object_path) || mkdir $(object_path)
	test -d $(lib_path) || mkdir $(lib_path)

clear:
	rm -r -f $(build)/*