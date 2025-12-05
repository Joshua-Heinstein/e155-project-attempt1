// Joshua Heinstein
// joshuaheinstein@gmail.com
// 10/13/2025
// keypad.sv
// Keypad scanning module

module keypad(
    input logic         clk, reset,
    input logic [3:0]   rows,
    output logic [3:0]  cols,
    output logic [15:0] press
);

    logic [3:0]     db_rows;
    logic [1:0]     col_index;
    logic [15:0]    snapshot;

    debounce db0 (clk, reset, rows[0], db_rows[0]);
    debounce db1 (clk, reset, rows[1], db_rows[1]);
    debounce db2 (clk, reset, rows[2], db_rows[2]);
    debounce db3 (clk, reset, rows[3], db_rows[3]);

    // Scan columns and take snapshot of debounced rows
    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            col_index  <= 2'd0;
            snapshot <= 16'd0;
        end else begin
            col_index <= col_index + 1;
            snapshot[col_index*4 +: 4] <= ~db_rows; // store debounced rows
        end
    end

    assign cols = ~(4'b0001 << col_index); // drive one column low at a time

    // Priority encoder to deal with multiple key presses by only reporting the first one found
    always_comb begin
        press = 16'd0;
        for (int i = 0; i < 16; i++) begin
            if (snapshot[i]) begin
                press = (16'h1 << i);
                break;
            end
        end
    end

endmodule