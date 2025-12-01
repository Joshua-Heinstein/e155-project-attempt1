// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/13/2025
// debounce.sv
// Debounce/synchronizer module for mechanical button presses

module debounce(
    input  logic clk,
    input  logic reset,
    input  logic button,
    output logic clean_press
);

    logic [19:0] counter;   // Adjust width for desired delay
    logic sync0, sync1;

    always_ff @(posedge clk) begin
        if (reset) begin
            clean_press <= 0;
            counter <= 0;
            sync0 <= 0;
            sync1 <= 0;
        end else begin
            // Synchronizer
            sync0 <= ~button;  // use ~ if button is active-low
            sync1 <= sync0;

            // Debounce logic
            if (clean_press == sync1)
                counter <= 0;
            else begin
                counter <= counter + 1;
                if (&counter)
                    clean_press <= sync1;
            end
        end
    end

endmodule
