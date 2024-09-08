module top (
    clk
);

  input logic clk;
  logic [7:0] shr;
  // nice showcase to show how verilog works
  always @(posedge clk) begin
    shr <= shr << 1;
    shr[0] <= clk;
  end


endmodule
