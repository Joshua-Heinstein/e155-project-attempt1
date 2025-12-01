module dispFSM(
    input logic         clk, reset,
    input logic [15:0]  key,
    output logic [3:0]  s1, s2
)
    typedef enum logic [4:0] {idle, check, S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, Sa, Sb, Sc, Sd, Se, Sf} statetype;
    logic [3:0] s1next, s2next;
    statetype state, nextstate;

    // State register
    always_ff @(posedge clk, posedge reset) begin
            if (reset) begin 
                state <= S0;
                s1 = 4'h0;
                s2 = 4'h0;
            end
            else begin      
                state <= nextstate;
                s1 = s1next;
                s2 = s2next;
            end
    end
    
    // Next state logic
    always_comb begin
        case (state)
            default: nextstate = idle;
            idle: begin 
                    case(key)
                        16'h0000: nextstate = idle;
                        16'h0001: nextstate = S0;
                        16'h0002: nextstate = S1;
                        16'h0004: nextstate = S2;
                        16'h0008: nextstate = S3;
                        16'h0010: nextstate = S4;
                        16'h0020: nextstate = S5;
                        16'h0040: nextstate = S6;
                        16'h0080: nextstate = S7;
                        16'h0100: nextstate = S8;
                        16'h0200: nextstate = S9;
                        16'h0400: nextstate = Sa;
                        16'h0800: nextstate = Sb;
                        16'h1000: nextstate = Sc;
                        16'h2000: nextstate = Sd;
                        16'h4000: nextstate = Se;
                        16'h8000: nextstate = Sf;
                        default: nextstate = idle;
                    endcase
                end

            check: if (key == 16'h0000) nextstate = idle;
                   else nextstate = check;

            S0: assign s2next = s1;
                assign s1next = 4'h0;
                nextstate = check;
            S1: assign s2 = s1;
                assign s1 = 4'h1;
                nextstate = check;

            S2: assign s2 = s1;
                assign s1 = 4'h2;
                nextstate = check;

            S3: assign s2 = s1;
                assign s1 = 4'h3;
                nextstate = check;

            S4: assign s2 = s1;
                assign s1 = 4'h4;
                nextstate = check;

            S5: assign s2 = s1;
                assign s1 = 4'h5;
                nextstate = check;

            S6: assign s2 = s1;
                assign s1 = 4'h6;
                nextstate = check;

            S7: assign s2 = s1;
                assign s1 = 4'h7;
                nextstate = check;

            S8: assign s2 = s1;
                assign s1 = 4'h8;
                nextstate = check;

            S9: assign s2 = s1;
                assign s1 = 4'h9;
                nextstate = check;

            Sa: assign s2 = s1;
                assign s1 = 4'ha;
                nextstate = check;

            Sb: assign s2 = s1;
                assign s1 = 4'hb;
                nextstate = check;

            Sc: assign s2 = s1;
                assign s1 = 4'hc;
                nextstate = check;

            Sd: assign s2 = s1;
                assign s1 = 4'hd;
                nextstate = check;

            Se: assign s2 = s1;
                assign s1 = 4'he;
                nextstate = check;

            Sf: assign s2 = s1;
                assign s1 = 4'hf;
                nextstate = check;
        endcase
    end

endmodule