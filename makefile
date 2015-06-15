all:
	$(MAKE) -C pisinger_lib
	$(MAKE) -C dual_lagrangean_lib
	$(MAKE) -C problem_gen
	$(MAKE) -C dual_solvers

clear:	
	$(MAKE) clear -C pisinger_lib
	$(MAKE) clear -C dual_lagrangean_lib
	$(MAKE) clear -C dual_solvers
	$(MAKE) clear -C problem_gen