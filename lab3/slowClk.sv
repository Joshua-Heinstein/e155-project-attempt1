// Joshua Heinstein
// 9/10/2025
// joshuaheinstein@gmail.com
// slowClk.sv

// Module slowClk is a clock divider that takes in the 24MHz 
// internal oscillator and outputs a much slower clock to drive
// the switching of 7 segment displays and inputs.

module slowClk (
    input  logic int_osc,   // 24 MHz system clock
    output logic tick       // one-cycle enable pulse
);

    logic [24:0] counter;

    always_ff @(posedge int_osc) begin
        if (counter == 23999) begin   // count 0..23999
            counter <= 0;
            tick    <= 1'b1;          // raise enable once
        end else begin
            counter <= counter + 1;
            tick    <= 1'b0;
        end
    end

endmodule