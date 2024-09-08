#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include "verilated.h"
#include "Vtop.h"
#include "tb.hpp"

#define MAX_TIME 5

using namespace std;

int main() {
  TB<Vtop>* tb = new TB<Vtop>(MAX_TIME);

//  tb->start_trace("waveform.vcd");
  while (tb->should_tick()) {
    tb->tick();
  }

//  tb->close_trace();
  exit(0);
}
