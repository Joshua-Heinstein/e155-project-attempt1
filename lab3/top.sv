// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/13/2025
// top.sv
// Top module for keypad and dual 7-segment display

module top (
    input  logic reset,
    output logic [3:0] rows,
    input  logic [3:0] cols,
    output logic [6:0] seg,
    output logic en1, en2
);

    logic clk;
    logic [15:0] keyval;
    logic [3:0] sw1, sw2;

    logic int_osc;

    HSOSC #(.CLKHF_DIV(2'b11))
        hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));

    keypad k1 (.Clk(Clk), .Reset(Reset), .Rows(Rows), .Cols(Cols), .KeyVal(KeyVal));
    dispFSM d1 (.Clk(Clk), .Reset(Reset), .KeyVal(KeyVal), .Sw1(Sw1), .Sw2(Sw2));
    DualSevSeg display (.Clk(Clk), .Reset(Reset), .Sw1(Sw1), .Sw2(Sw2), .Seg(Seg), .En1(En1), .En2(En2));

endmodule