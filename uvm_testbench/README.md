# UVM-style C++ testbenches for Verilator


# Using this testbench

Copy over example/Makefile, example/template.cpp example/tb.hpp, and modify it for your purposes

These testbenches are made to test the functionalities of toplevel modules by providing random data, and verifying the tests using a scoreboard rather than asserts. This helps with understanding where functionality is lacking


## Cover
Instead of just having the scoreboard to validate functionality, you would also want to validate the test generation to see if you are generating enough tests. This can be done using functional cover.



### Connecting to a fuzzer

You can change the function `create_input_tx` to whatever input generation fucntion you want. If you have a fuzzer that already generates data, you can use the function to read from the fuzzed program.


## Credit
Most of the code is adapted from <https://itsembedded.com/dhd/verilator_4/> with changes and additions.
