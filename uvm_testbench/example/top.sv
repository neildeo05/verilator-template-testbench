// FIFO to test out system
module top (
    input logic clk,
    input logic reset,
    input logic r_w,
    input logic en,
    input logic [31:0] in,
    output logic [31:0] out,
    output logic [31:0] fifo[8]
);

  logic full;
  logic empty;
  logic [2:0] wptr;
  logic [2:0] rdptr;

  assign full  = (rdptr - 1) == wptr;
  assign empty = rdptr == wptr;

  always @(posedge clk) begin
    if (reset) begin
      rdptr <= 3'd4;
      wptr  <= 3'd4;
    end else begin
      if (en) begin
        if (!r_w & !empty) begin
          rdptr <= rdptr - 3'd1;
          out   <= fifo[rdptr];
        end else if (r_w & !full) begin
          wptr <= wptr + 3'd1;
          fifo[wptr] <= in;
          out <= 3'b0;
        end
      end else begin
        out <= 3'b0;
      end
    end
  end
endmodule
