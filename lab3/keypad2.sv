// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/13/2025
// keypad.sv
// Keypad scanning module

module keypad(
    input logic         tick, reset,
    input logic [3:0]   rows,
    output logic [3:0]  cols,
    output logic [15:0] keyval
);

logic [24:0] counter;
logic [3:0]  pressed;
typedef enum logic [4:0] {idle, check, S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, Sa, Sb, Sc, Sd, Se, Sf} statetype;
    logic [3:0] s1next, s2next;

always_ff @(posedge tick) begin
    if (counter[24]) begin   
        counter <= 0;
    end else begin
        counter <= counter + 1;
    end
end



always_ff@(posedge counter[13], posedge reset) begin
    if (reset) begin
        pressed <= 4'b0000;
    end else begin
        case(counter[15:14])
            2'b00: begin row = 4'b0001; pressed
        endcase
    end
end


