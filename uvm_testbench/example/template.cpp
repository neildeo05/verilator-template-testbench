#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include "verilated.h"
#include "Vtop.h"
#include "tb.hpp"

#define MAX_TIME 50

using namespace std;

int main() {
  TB<Vtop>* tb = new TB<Vtop>(MAX_TIME);

  tb->start_trace("waveform.vcd");
  for(int i = 0; i < 10; i++) {
    tb->dut->reset = 1;
    tb->tick();
  }
  tb->dut->reset = 0;
  while (tb->should_tick()) {
    tb->tick();
  }

 tb->close_trace();
  exit(0);
}
