module TB();

wire [79:0] codeword_in;
wire [9:0] DUE_information_in;
wire [1:0] decode_result_out;
wire [63:0] data_out;
//wire [7:0] error_value_out;
//wire [7:0] syndrome0_out;
//wire [7:0] syndrome1_out;
 
reg [79:0] codeword;
reg [9:0] DUE_information;

initial begin
    // Error correction

    /*
    // First error correction case

    DUE_information = 10'b10_0000_0000;

    codeword[79:72]  = 8'b1010_0011; // error value
    codeword[71:0]   = 72'b0;

    */

    /*
    // Second error correction case

    DUE_information = 10'b01_0000_0000;

    codeword[79:72] = 8'b0000_0000;
    codeword[71:64] = 8'b1010_0011;
    codeword[63:0]  = 64'b0;
    
    */
    
    // Erasure correction
  
  
    /*
    // First erasure correction case 
    DUE_information = 10'b10_0000_0010;

    codeword[79:72] = 8'b1010_0011;
    codeword[71:16] = 56'b0;
    codeword[15:8]  = 8'b0111_1001;
    codeword[7:0]   = 8'b0;

    */

    
    // Second erasure correction case

    DUE_information = 10'b10_0100_0000;

    codeword[79:72] = 8'b1010_0011;
    codeword[71:56] = 16'b0;
    codeword[55:48] = 8'b0101_1111;
    codeword[47:0]  = 48'b0;
    

end

assign codeword_in = codeword;
assign DUE_information_in = DUE_information;

  //AMDCHIPKILL_decodeR(codeword_in, error_location_out, decode_result_out, data_out, error_value_out,syndrome0_out,syndrome1_out);
  DECODER decoder(codeword_in, DUE_information_in, decode_result_out, data_out);

  initial begin
    # 60;
    $display("codeword :           %b",codeword_in);
    //$display("syndrome_0 :         %b",syndrome0_out);
    //$display("syndrome_1 :         %b",syndrome1_out);
    $display("due information :    %b",DUE_information_in);
    //$display("error_value :        %b",error_value_out);
    $display("decode_result_out :  %b",decode_result_out);
    $display("data :               %b",data_out);
  end

endmodule