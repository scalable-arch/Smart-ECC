module DECODER(input [79:0] codeword_in,
                      input [9:0] DUE_information_in,
                      output [1:0] Decode_result_out,
                      output [63:0] data_out
                      );
  // codeword_out: 80-bit RS codeword output


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
  wire [1:0] error_erasure_correction_flag;
  wire [15:0] syndrome;

  reg [1:0] decode_result;
  reg [1:0] decode_result_error;
  reg [1:0] decode_result_erasure;

  reg [63:0] data;
  reg [63:0] data_error;
  reg [63:0] data_erasure;


  // Choose error/erasure correction 'or' DUE report
  // Syndrome generation
  ERROR_ERASURE_INFORMATION error_erasure_information(DUE_information_in, error_erasure_correction_flag);
  SYNDROME_GENERATOR syndrome_generator(codeword_in,syndrome);
  
  // NE 'or' DUE: no error correction, extract data => Decode result = NE(00) 'or' DUE(10)
  // CE: implement error correction, forward Error location value => Decode result = 01 (CE)

  // Error correction
  ERROR_INFORMATION error_information(codeword_in, syndrome, decode_result_error, data_error);

  // Erasure correction
  ERASURE_INFORMATION erasure_information(codeword_in, DUE_information_in, syndrome, decode_result_erasure, data_erasure);

   always_comb begin   
      // implement error correction
      if (error_erasure_correction_flag==2'b00)begin
         data = data_error;
         decode_result = decode_result_error;
      end
      // implement erasrue correction
      else if (error_erasure_correction_flag==2'b01)begin
         data = data_erasure;
         decode_result = decode_result_erasure;
      end
      // report DUE (No error correction)
      else begin
         data = codeword_in[79:16];
         decode_result=2'b10;
      end
   end


  assign data_out = data; // error location : 0 > 79~72, error location : 1 > 71~64
  assign Decode_result_out = decode_result; // NE/CE/DUE
  //assign error_value_out = error_value; // for debug
  //assign syndrome0_out = syndrome[15:8]; // for debug
  //assign syndrome1_out = syndrome[7:0]; // for debug

endmodule
