#VERILATOR= $(shell which verilator)
.PHONY: simulate
simulate:
	verilator -I. -Wall -Wno-fatal --trace --cc --exe --build Example.cpp top.sv
	./obj_dir/Vtop
