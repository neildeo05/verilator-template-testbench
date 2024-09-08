// Borrowed from <https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html> and <https://itsembedded.com/dhd/verilator_4/>
#include <verilated_vcd_c.h>
#include <deque>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cinttypes>
#define PCT_GEN 4
#define SEED 1337
#define DEBUG 1
#define COVER 0

struct InputTx {
  // Define Inputs to module
  bool r_w;
  bool en;
  uint32_t in;
};

struct OutputTx {
  // Define Outputs of module
  uint32_t out;
  unsigned int* fifo;
};



InputTx* create_input_tx() {
  // Create transaction item 25% of the time, instead of 20% of the time, so we can drive it more often
  int rando = rand() % PCT_GEN;
  if(rando) {
    InputTx* tx = new InputTx;
    // Set input tx based on random values
    tx->r_w = rand() % 2;
    tx->en = rand() % 2;
    tx->in = rand() % 256;
    return tx;
  }
  else return NULL;
}

#define debug(...) if(DEBUG) printf(__VA_ARGS__);

struct Scoreboard {
  std::deque<InputTx*> inputs;
  // To get Inputs to match up with their corresponding outputs
  bool delay_flag;
  void input_write(InputTx* input_value) {
    inputs.push_back(input_value);
  }
  void output_write(OutputTx* output_value) {
    assert(!inputs.empty());
    InputTx* in = inputs.front();
    // Since the inputs are delivered per rising edge, we expect output to show up on the next rising edge, thats why we need to delay the first input
    // until actual outputs are being received.
    if(!delay_flag) {
      delay_flag = 1;
      delete output_value;
    } else {
      debug("========== PACKET PRINTOUT START ==========\n");
      debug("R/W -> %d\n", in->r_w);
      debug("Enable -> %d\n", in->en);
      debug("Input -> %d\n", in->in);
      debug("Output -> %d\n", output_value->out);
      for(int i = 0; i < 8; i++) {
        debug("fifo[%d]: %d ", i, output_value->fifo[i]);
      }
      debug("\n");
      debug("========== PACKET PRINTOUT  END  ==========\n");
      debug("\n\n\n");
      //debug("========== EVALUATION BEGIN  ==========\n");
      // This is the space to evaluate whether the packets are correct or not
      //debug("========== EVALUATION END  ==========\n");
      inputs.pop_front();
      delete in;
      delete output_value;
    }
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

      dut->r_w = in_tx->r_w;
      dut->en = in_tx->en;
      dut->in = in_tx->in;
      // dut->val = in_tx->val
      // make sure to delete the transaction
      delete in_tx;
    }
  }
  virtual void in_monitor() {
    InputTx* tx = new InputTx;
    //move dut values into tx values
    tx->r_w = dut->r_w;
    tx->en = dut->en;
    tx->in = dut->in;
    scb->input_write(tx);
  }
  virtual void out_monitor() {
    OutputTx* tx = new OutputTx;
    // move dut values into tx values
    tx->out = dut->out;
    tx->fifo = dut->fifo;
    scb->output_write(tx);
  }
  virtual void tick() {
    // evaluate falling edge
    // All that matters is the position of the evaluation. Either after driving it, or not at all.
    dut->clk ^= 1;
    dut->eval();
    if(!dut->reset && dut->clk) {
      InputTx* tx = create_input_tx();
      drive(tx);
      in_monitor();
      out_monitor();
    }
    trace->dump((int)cnt);
    cnt++;
    trace->flush();
  }
};
