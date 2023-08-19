module ERROR_INFORMATION(input [79:0] Codeword_in,
                      input [15:0] Syndrome_in,
                      output [1:0] Decode_result_error_out,
                      output [63:0] Data_error_out
                      );
  // Syndome_in : 16bit syndrome input
  // Decode_result_out : NE(00), CE(01), DUE(10)
  // Data_error_out : 64-bit corrected/detected original data

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

  reg [79:0] Codeword;
  reg [1:0] Decode_result;
  reg [7:0] Error_location_gfexp;
  reg [63:0] Data_error;
  wire [7:0] Syndrome0,Syndrome1;

  //GFDIV gdiv(Syndrome_in[7:0], Syndrome_in[15:8], Error_location_gdiv);
  //GFEXP gfexp(Error_location_gdiv, Error_location_gfexp);

    GFEXP gfexp_00(Syndrome_in[7:0],Syndrome0); // S1
    GFEXP gfexp_01(Syndrome_in[15:8],Syndrome1); // S0
    ERROR_LOCATION error_location(Syndrome1 , Syndrome0, Error_location_gfexp);
    //GFDIV error_value00(Syndrome_in[7:0],Syndrome_in[15:8], Error_location_gfdiv); // a^(50+?, 78+?, ...)/a^(25+?, 39+?, ...) => a^(25, 39, ...)  
    //GFDIV error_value01(Syndrome_in[15:8], Error_location_gfdiv, Error_value); // a^(25+?)/a^(a^25) => a^? (error value)
    
   always_comb begin    
      Codeword = Codeword_in;
     // CE (8-th chip)
     if(Syndrome_in[7:0]==8'b0000_0000 && Syndrome_in[15:8]!=8'b0000_0000)begin // 8th chip errors
        Codeword[15:8]^=Syndrome_in[15:8];
        Data_error=Codeword[79:16];
        Decode_result=2'b01; // CE
     end

     // CE (9-th chip)
     if(Syndrome_in[7:0]!=8'b0000_0000 && Syndrome_in[15:8]==8'b0000_0000)begin // 9th chip errors
        Codeword[7:0]^=Syndrome_in[7:0];
        Data_error=Codeword[79:16];
        Decode_result=2'b01; // CE
     end

     // CE(0~7-th chip)
     if(Syndrome_in[7:0]!=8'b0000_0000 && Syndrome_in[15:8]!=8'b0000_0000 && Error_location_gfexp<=8'd7) begin // 0~7th chip errors
        //$display("error location div : %b",Error_location_gdiv);
         //$display("error information error location : %d",Error_location_gfexp);
         //$display("CE!");
         case(Error_location_gfexp)
            8'd0: Codeword[79:72]^=Syndrome_in[15:8];
            8'd1: Codeword[71:64]^=Syndrome_in[15:8];
            8'd2: Codeword[63:56]^=Syndrome_in[15:8];
            8'd3: Codeword[55:48]^=Syndrome_in[15:8];
            8'd4: Codeword[47:40]^=Syndrome_in[15:8];
            8'd5: Codeword[39:32]^=Syndrome_in[15:8];
            8'd6: Codeword[31:24]^=Syndrome_in[15:8];
            8'd7: Codeword[23:16]^=Syndrome_in[15:8];
            default: Codeword = Codeword;
         endcase

         Data_error=Codeword[79:16];
         Decode_result=2'b01; // CE
     end

     // DUE
     if(Syndrome_in[7:0]!=8'b0000_0000 && Syndrome_in[15:8]!=8'b0000_0000 && Error_location_gfexp>8'd7) begin // 0~7th chip errors
        //$display("error location div : %b",Error_location_gdiv);
         //$display("error information error location : %d",Error_location_gfexp);
         //$display("DUE");
         Data_error=Codeword_in[79:16];
         Decode_result=2'b10;
     end

     // NE
     if(Syndrome_in[15:0]==16'b0000_0000_0000_0000) begin
         Data_error=Codeword_in[79:16];
         Decode_result=2'b00;
     end
   end
  
  // NE : Syndrome_in is all zero
  // CE: Error location is 0~9
  // otherwise : DUE
  assign Decode_result_error_out = Decode_result;
  assign Data_error_out = Data_error;

endmodule
