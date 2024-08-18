#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include "verilated.h"
#include "V_.h"
#include "tb.hpp"

#define MAX_TIME 20

using namespace std;

int main() { 
  TB<_>* tb = new TB<_>(MAX_TIME);

  tb->start_trace("waveform.vcd");
  while (tb->should_tick()) {
    tb->tick();
  }

  tb->close_trace();
  exit(0);
}
