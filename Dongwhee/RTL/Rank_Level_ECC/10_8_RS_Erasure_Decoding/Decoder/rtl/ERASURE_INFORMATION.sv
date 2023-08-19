module ERASURE_INFORMATION(input [79:0] Codeword_in,
                      input [9:0] DUE_information_in,
                      input [15:0] Syndrome_in,
                      output [1:0] Decode_result_erasure_out,
                      output [63:0] Data_erasure_out
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
  reg [63:0] Data_erasure;
  reg [7:0] Error_value0, Error_value1;

  wire [3:0] First_error_location; // 0~8
  wire [3:0] Second_error_location; // 1~9
  wire [3:0] First_error_location_index; // a^0~a^8
  wire [3:0] Second_error_location_index; // a^1~a^9
  wire [7:0] Syndrome0,Syndrome1;
  wire [7:0] Temp_value0, Temp_value1, Temp_value2, Temp_value3, Temp_value4, Temp_value5, Temp_value6;

  //wire [7:0] OUTPUT1, OUTPUT2, OUTPUT3;

  //GFDIV gdiv(Syndrome_in[7:0], Syndrome_in[15:8], Error_location_gdiv);
  //GFEXP gfexp(Error_location_gdiv, Error_location_gfexp);

    GFEXP gfexp_00(Syndrome_in[7:0],Syndrome0); // S1
    GFEXP gfexp_01(Syndrome_in[15:8],Syndrome1); // S0 
    ERASURE_LOCATION erasure_location(DUE_information_in, First_error_location, Second_error_location);
    GFINDEX gfindex_00(First_error_location, First_error_location_index); // a^i
    GFINDEX gfindex_01(Second_error_location, Second_error_location_index); // a^j

    GFMULT gfmult_00(Syndrome_in[15:8], Second_error_location_index, Temp_value0); // Temp0 = S0 x a^j
    GFADD gfadd_00(Temp_value0, Syndrome_in[7:0], Temp_value1); // Temp1 = S0 x a^j + S1
    GFADD gfadd_01(First_error_location_index, Second_error_location_index, Temp_value2); // Temp2 = a^i + a^j
    GFMULT gfmult_01(Syndome_in[15:8], First_error_location_index, Temp_value6); // Temp6 = S0 x a^i

    GFDIV gfdiv_00(Temp_value1, Temp_value2, Temp_value3); // Temp3 = (S0 x a^j + S1)/(a^i + a^j) 
    GFDIV gfdiv_01(Syndrome_in[7:0], Temp_value2, Temp_value4); // Temp4 = S1/(a^i + a^j)
    GFDIV gfdiv_02(Syndrome_in[7:0], First_error_location_index, Temp_value5); // Temp5 = S1/a^i

    //GFEXP gfexp02(Temp_value1, OUTPUT1);
    //GFEXP gfexp03(Temp_value2, OUTPUT2);
    //GFEXP gfexp04(Temp_value3, OUTPUT3);

    //GFDIV error_value00(Syndrome_in[7:0],Syndrome_in[15:8], Error_location_gfdiv); // a^(50+?, 78+?, ...)/a^(25+?, 39+?, ...) => a^(25, 39, ...)  
    //GFDIV error_value01(Syndrome_in[15:8], Error_location_gfdiv, Error_value); // a^(25+?)/a^(a^25) => a^? (error value)
    
   always_comb begin    
      Codeword = Codeword_in;
      // $display("First_error_location : %d",First_error_location);
      // $display("Second_error_location : %d",Second_error_location);
      
      // $display("First_error_location_index : %b",First_error_location_index);
      // $display("Second_error_location_index : %b",Second_error_location_index);

      // $display("Temp value1 : %b", Temp_value1); // Temp1 = S0 x a^j + S1
      // $display("Temp value2 : %b", Temp_value2); // Temp2 = a^i + a^j (0000_1001)
      // $display("Temp value3 : %b", Temp_value3); // Temp3 = (S0 x a^j + S1)/(a^i + a^j)
 
      // $display("OUTPUT1 : %d", OUTPUT1); // Temp1의 지수값
      // $display("OUTPUT2 : %d", OUTPUT2); // Temp1의 지수값
      // $display("OUTPUT3 : %d", OUTPUT3); // Temp1의 지수값

      // CE (0~7-th chips)
      if(First_error_location<=4'b0110 && Second_error_location<=4'b0111)begin
         // CE
         Decode_result=2'b01;
         // n != m
         if(Syndrome_in[15:8]!=8'b0000_0000)begin
            // DSC and SSC
            if(Temp_value1!=0) begin
               Error_value0 = Temp_value3; // a^n = (S0 x a^j + S1)/(a^i + a^j) 
               Error_value1 = Syndrome_in[15:8] ^ Error_value0; // a^m = S0 + a^n

               //$display("Error value0 : %b",Error_value0);
               //$display("Error value1 : %b",Error_value1);

               case(First_error_location)
                  4'd0: Codeword[79:72]^=Error_value0;
                  4'd1: Codeword[71:64]^=Error_value0;
                  4'd2: Codeword[63:56]^=Error_value0;
                  4'd3: Codeword[55:48]^=Error_value0;
                  4'd4: Codeword[47:40]^=Error_value0;
                  4'd5: Codeword[39:32]^=Error_value0;
                  4'd6: Codeword[31:24]^=Error_value0;
                  default: Codeword = Codeword;
               endcase

               case(Second_error_location)
                  4'd1: Codeword[71:64]^=Error_value1;
                  4'd2: Codeword[63:56]^=Error_value1;
                  4'd3: Codeword[55:48]^=Error_value1;
                  4'd4: Codeword[47:40]^=Error_value1;
                  4'd5: Codeword[39:32]^=Error_value1;
                  4'd6: Codeword[31:24]^=Error_value1;
                  4'd7: Codeword[23:16]^=Error_value1;
                  default: Codeword = Codeword;
               endcase
            end
         end
         // n == m
         else begin
            Error_value0 = Temp_value4; // a^n = S1/(a^i + a^j)
            Error_value1 = Error_value0; // a^m = a^n

            case(First_error_location)
               4'd0: Codeword[79:72]^=Error_value0;
               4'd1: Codeword[71:64]^=Error_value0;
               4'd2: Codeword[63:56]^=Error_value0;
               4'd3: Codeword[55:48]^=Error_value0;
               4'd4: Codeword[47:40]^=Error_value0;
               4'd5: Codeword[39:32]^=Error_value0;
               4'd6: Codeword[31:24]^=Error_value0;
               default: Codeword = Codeword;
            endcase

            case(Second_error_location)
               4'd1: Codeword[71:64]^=Error_value1;
               4'd2: Codeword[63:56]^=Error_value1;
               4'd3: Codeword[55:48]^=Error_value1;
               4'd4: Codeword[47:40]^=Error_value1;
               4'd5: Codeword[39:32]^=Error_value1;
               4'd6: Codeword[31:24]^=Error_value1;
               4'd7: Codeword[23:16]^=Error_value1;
               default: Codeword = Codeword;
            endcase
         end
      end 

      // CE (0~7-th chip, 8-th chip) (i: 0~7, j=8)
      else if(First_error_location<=4'b0111 && Second_error_location==4'b1000)begin
         // CE
         Decode_result=2'b01;
         // n!=m
         if(Syndrome_in[15:8]!=8'b0000_0000) begin
            Error_value0 = Temp_value5; // a^n = S1/a^i
            Error_value1 = Error_value0 ^ Syndrome_in[15:8]; // a^m = a^n + S0

            case(First_error_location)
               4'd0: Codeword[79:72]^=Error_value0;
               4'd1: Codeword[71:64]^=Error_value0;
               4'd2: Codeword[63:56]^=Error_value0;
               4'd3: Codeword[55:48]^=Error_value0;
               4'd4: Codeword[47:40]^=Error_value0;
               4'd5: Codeword[39:32]^=Error_value0;
               4'd6: Codeword[31:24]^=Error_value0;
               4'd7: Codeword[23:16]^=Error_value0;
               default: Codeword = Codeword;
            endcase

            // Second error location (fixed)
            Codeword[15:8]^=Error_value1;
         end

         // n == m
         else begin
            Error_value0 = Temp_value5; // a^n = S1/a^i
            Error_value1 = Error_value0; // a^m = a^n
         
            case(First_error_location)
               4'd0: Codeword[79:72]^=Error_value0;
               4'd1: Codeword[71:64]^=Error_value0;
               4'd2: Codeword[63:56]^=Error_value0;
               4'd3: Codeword[55:48]^=Error_value0;
               4'd4: Codeword[47:40]^=Error_value0;
               4'd5: Codeword[39:32]^=Error_value0;
               4'd6: Codeword[31:24]^=Error_value0;
               4'd7: Codeword[23:16]^=Error_value0;
               default: Codeword = Codeword;
            endcase

            // Second error location (fixed)
            Codeword[15:8]^=Error_value1;
         end
      end

      // CE (0~7-th chip, 9-th chip) (i: 0~7, j=9)
      else if(First_error_location<=4'b0111 && Second_error_location==4'b1001)begin
         // CE
         Decode_result=2'b01;
         // n!=m
         if(Syndrome_in[15:8]!=8'b0000_0000) begin
            Error_value0 = Syndrome_in[15:8]; // a^n = S0
            Error_value1 = Temp_value6 ^ Syndrome_in[7:0]; // a^m = S0 x a^i + S1
           
            case(First_error_location)
               4'd0: Codeword[79:72]^=Error_value0;
               4'd1: Codeword[71:64]^=Error_value0;
               4'd2: Codeword[63:56]^=Error_value0;
               4'd3: Codeword[55:48]^=Error_value0;
               4'd4: Codeword[47:40]^=Error_value0;
               4'd5: Codeword[39:32]^=Error_value0;
               4'd6: Codeword[31:24]^=Error_value0;
               4'd7: Codeword[23:16]^=Error_value0;
               default: Codeword = Codeword;
            endcase

            // Second error location (fixed)
            Codeword[7:0]^=Error_value1;
         end

         // n == m
         else begin
            Error_value1 = Temp_value6 ^ Syndrome_in[7:0]; // a^m = S0 x a^i + S1
            Error_value0 = Error_value1; // a^n = a^m
         
            case(First_error_location)
               4'd0: Codeword[79:72]^=Error_value0;
               4'd1: Codeword[71:64]^=Error_value0;
               4'd2: Codeword[63:56]^=Error_value0;
               4'd3: Codeword[55:48]^=Error_value0;
               4'd4: Codeword[47:40]^=Error_value0;
               4'd5: Codeword[39:32]^=Error_value0;
               4'd6: Codeword[31:24]^=Error_value0;
               4'd7: Codeword[23:16]^=Error_value0;
               default: Codeword = Codeword;
            endcase

            // Second error location (fixed)
            Codeword[15:8]^=Error_value1;
         end
      end

      // CE (8-th chip, 9-th chip) (i: 8, j=9)
      else if(First_error_location==4'b1000 && Second_error_location==4'b1001)begin
         // CE
         Decode_result=2'b01;
         Error_value0 = Syndrome_in[15:8]; // a^n = S0
         Error_value1 = Syndrome_in[7:0]; // a^m = S1

         // first error location (fixed)
         Codeword[15:8]^=Error_value0;

         // Second error location (fixed)
         Codeword[7:0]^=Error_value1;
      end

      // NE
      else if(Syndrome_in == 16'b0000_0000_0000_0000) begin
         //$display("NE cases!");
         Decode_result=2'b00;
      end

      // DUE
      else begin
         //$display("DUE report!");
         Decode_result = 2'b10;
      end

      Data_erasure = Codeword[79:16];
   end
  
  // NE : Syndrome_in is all zero
  // CE: Error location is 0~9
  // otherwise : DUE
  assign Decode_result_erasure_out = Decode_result;
  assign Data_erasure_out = Data_erasure;

endmodule