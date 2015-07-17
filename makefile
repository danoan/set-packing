all:
	$(MAKE) -C pisinger_lib
	$(MAKE) -C dual_lagrangean_lib
	$(MAKE) -C clique_inequality
	$(MAKE) -C problem_gen
	$(MAKE) -C dual_solvers

debug:
	$(MAKE) debug -C pisinger_lib
	$(MAKE) debug -C dual_lagrangean_lib
	$(MAKE) debug -C clique_inequality
	$(MAKE) -C problem_gen
	$(MAKE) debug -C dual_solvers

clear:	
	$(MAKE) clear -C pisinger_lib
	$(MAKE) clear -C dual_lagrangean_lib
	$(MAKE) clear -C clique_inequality
	$(MAKE) clear -C dual_solvers
	$(MAKE) clear -C problem_gen