VERILATOR= $(shell which verilator)
.PHONY: simulate
simulate:
	$(VERILATOR) -I. -Wall -Wno-fatal --trace --cc --exe --build template.cpp top.sv
	./obj_dir/Vtop
