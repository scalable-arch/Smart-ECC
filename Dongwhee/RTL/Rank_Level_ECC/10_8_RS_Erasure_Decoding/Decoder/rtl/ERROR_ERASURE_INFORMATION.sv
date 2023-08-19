module ERROR_ERASURE_INFORMATION(input [9:0] DUE_information_in,
                      output [1:0] error_erasure_correction_flag_out);
  // DUE_information_in: 10-bit DUE_information of 10 chips (8 Data chips + 2 ECC chips) input
  // error_erasure_correction_flag_out : 2-bit error/erasure correction flag output (choose error/erasure correction 'or' DUE)
  /*
    G-Matrix

    1 0 0 0 0 0 0 0 1 1
    0 1 0 0 0 0 0 0 1 a^1
    0 0 1 0 0 0 0 0 1 a^2
    0 0 0 1 0 0 0 0 1 a^3
    0 0 0 0 1 0 0 0 1 a^4
    0 0 0 0 0 1 0 0 1 a^5
    0 0 0 0 0 0 1 0 1 a^6
    0 0 0 0 0 0 0 1 1 a^7

    H-Matrix

    1 1   1   1   1   1   1   1   1 0 
    1 a^1 a^2 a^3 a^4 a^5 a^6 a^7 0 1 

  */

  reg [1:0] error_erasure_correction_flag;

    always @(*) begin
        int sum=0;
        for (int index=0; index<10; index=index+1)
            sum=sum+DUE_information_in[index];
        
        // choose error correction
        if (sum<2)
            error_erasure_correction_flag=2'b00;
        // choose erasure correction
        else if (sum==2)
            error_erasure_correction_flag=2'b01;
        // choose DUE
        else // sum=3~10
            error_erasure_correction_flag=2'b10;

    end

    assign error_erasure_correction_flag_out = error_erasure_correction_flag;

endmodule

