.PHONY: top

top:
	verilator -I../src/ -Wall -Wno-fatal --trace --cc --exe --build _.cpp _.sv
	./obj_dir/V_
