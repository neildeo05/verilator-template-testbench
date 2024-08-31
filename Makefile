VERILATOR= $(shell which verilator)
.PHONY: simulate
simulate:
	$(VERILATOR) -I. -Wall -Wno-fatal --trace --cc --exe --build _.cpp _.sv
	./obj_dir/_
