import subprocess
import argparse
import pathlib

# pass in name of SystemVerilog top file and the name of the C++ test file

parser = argparse.ArgumentParser(prog='create-tb', description="Create Verilator UVM-style testbench")

parser.add_argument('-s', '--system-verilog-source', type=pathlib.Path)
parser.add_argument('-c', '--cpp-file-name')
parser.add_argument('destination', type=pathlib.Path)

args = parser.parse_args()
sv = args.system_verilog_source.resolve()
cpp = args.cpp_file_name
dest = args.destination.resolve()
assert(sv != None and cpp != None)

print(sv, cpp, dest)

# Change all the '%__%' to name/filename

# this is the copy command to move the template to the actual system
exec_list = ['cp', f'{pathlib.Path(__file__)}/template.cpp', f'{dest}/{cpp}']
print(exec_list)
