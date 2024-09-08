// Borrowed from <https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html>
// Inspired from <https://itsembedded.com/dhd/verilator_4/>
#include <verilated_vcd_c.h>
#include <deque>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cassert>
#define PCT_GEN 4
#define SEED 1337


struct InputTx {
  // Define Inputs to module
};

struct OutputTx {
  // Define Outputs of module
};


InputTx* create_input_tx() {
  // Create transaction item 25% of the time, instead of 20% of the time, so we can drive it more often
  if(rand() % PCT_GEN == 0) {
    InputTx* tx = new InputTx;
    // Set input tx based on random values
    return tx;
  }
  else return NULL;
}


struct Scoreboard {
  std::deque<InputTx*> inputs;
  void input_write(InputTx* input_value) {
    inputs.push_back(input_value);
  }
  void output_write(OutputTx* output_value) {
    assert(inputs.empty());

    InputTx* in = inputs.front();
    inputs.pop_front();


    // Make sure that the inputs are correct here -> For example look at the inputs opcode, and see what the output is vs the desired output


    free(in);
    free(output_value);
  }

};
template<class Module> class TB {
public:
  unsigned long long cnt;
  unsigned long long max_cnt;
  Module* dut;
  VerilatedVcdC *trace;
  Scoreboard* scb;

  TB(unsigned long long _max_cnt) {
    max_cnt = _max_cnt;
    scb = new Scoreboard;
    dut = new Module;
    cnt = 0;
    Verilated::traceEverOn(true);
  }
  ~TB() {
    delete scb;
    delete dut;
  }
  virtual int should_tick() {
    return cnt != max_cnt;
  }
  virtual void start_trace (const char* fname) {
    trace = new VerilatedVcdC;
    dut->trace(trace, 99);
    trace->open(fname);
  }
  virtual void close_trace() {
    trace->close();
  }
  virtual void drive(InputTx* in_tx) {
    if(in_tx != NULL) {
      // any other conditions to not set test
      // dut->val = in_tx->val
      // make sure to delete the transaction
      free(in_tx);
    }
  }
  virtual void in_monitor() {
    InputTx* tx = new InputTx;
    //move dut values into tx values
    scb->input_write(tx);
  }
  virtual void out_monitor() {
    OutputTx* tx = new OutputTx;
    // move dut values into tx values
    scb->output_write(tx);
  }
  virtual void tick() {
    // evaluate falling edge
    InputTx* tx;
    dut->clk ^= 1;
    printf("%d\n", dut->clk);
    dut->eval();
    if(dut->clk == 1) {
      tx = create_input_tx();
      drive(tx);
      in_monitor();
      out_monitor();
    }
    trace->dump((int)cnt);
    cnt++;
    trace->flush();
    free(tx);
  }
};
