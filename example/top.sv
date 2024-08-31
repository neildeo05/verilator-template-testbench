module top (
    clk
);

  input logic clk;
  logic foo;
  logic bar;
  initial begin
    assign foo = 'b1;
  end
  always @(posedge clk) begin
    bar <= foo;
    foo <= 0;
  end


endmodule
