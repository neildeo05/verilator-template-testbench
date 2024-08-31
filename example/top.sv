module top (
    clk
);

  input logic clk;
  logic foo;
  // nice showcase to show how verilog works
  always @(posedge clk) begin
    if (foo == 1'b0) begin
      foo <= 0;
    end
    foo <= 1;
  end


endmodule
