module SEC_decoder_I(input [135:0] codeword, output [127:0] message, output due);

	wire[7:0] syndrome;
	reg[135:0] decoded;
	reg due_information;

	assign syndrome[7] = ^(codeword&136'b10101010011110000110010100100110001101110110110001010101011110100111001011010111110111111111100010110101011001010000101101100110_10000000);
	assign syndrome[6] = ^(codeword&136'b01001101100111000100011011100010101110001100101110110111011001101110001100001100011110011100001001111001001101111001010010001110_01000000);
	assign syndrome[5] = ^(codeword&136'b10010000100100101010111111101101001111111111001101100101110000111001010010111001011110001011010010101001100100011010011100100011_00100000);
	assign syndrome[4] = ^(codeword&136'b00010111111101011111111001100000000000100000110011000011010000011011010100011011101000010100110110111010001010110111110101110111_00010000);
	assign syndrome[3] = ^(codeword&136'b11010101011000000010101010011011110100101111001110000011111010110110110010010000101001101010000100011111011001010100110111011110_00001000);
	assign syndrome[2] = ^(codeword&136'b00011000010000010100010011010101100000111110000010111110111111010001100111111010100111011000111110101100101011011011111000111100_00000100);
	assign syndrome[1] = ^(codeword&136'b11101100001010110011110110100100010010110011111000101110101111111110100100001111111100111000010011001001101000110100000000011011_00000010);
	assign syndrome[0] = ^(codeword&136'b10111110011111101101111001011000010100111000101010101000110010101101001111100011010111001001011001011010011111000000000110011010_00000001);



	always @(syndrome) begin
	case(syndrome)
		// NO ERROR
		8'b00000000:begin decoded = codeword; 		  due_information = 1'b0; end
		// CE (Error Correction)
		8'b10101011:begin decoded = codeword^(1'b1<<135); due_information = 1'b0; end
		8'b01001010:begin decoded = codeword^(1'b1<<134); due_information = 1'b0; end
                8'b10000011:begin decoded = codeword^(1'b1<<133); due_information = 1'b0; end
                8'b00111101:begin decoded = codeword^(1'b1<<132); due_information = 1'b0; end
                8'b11000111:begin decoded = codeword^(1'b1<<131); due_information = 1'b0; end
                8'b01011011:begin decoded = codeword^(1'b1<<130); due_information = 1'b0; end
                8'b10010001:begin decoded = codeword^(1'b1<<129); due_information = 1'b0; end
                8'b01011000:begin decoded = codeword^(1'b1<<128); due_information = 1'b0; end
                8'b01110000:begin decoded = codeword^(1'b1<<127); due_information = 1'b0; end
                8'b10011101:begin decoded = codeword^(1'b1<<126); due_information = 1'b0; end
                8'b10011011:begin decoded = codeword^(1'b1<<125); due_information = 1'b0; end
                8'b11110001:begin decoded = codeword^(1'b1<<124); due_information = 1'b0; end
                8'b11000011:begin decoded = codeword^(1'b1<<123); due_information = 1'b0; end
                8'b01010001:begin decoded = codeword^(1'b1<<122); due_information = 1'b0; end
                8'b00100011:begin decoded = codeword^(1'b1<<121); due_information = 1'b0; end
                8'b00010110:begin decoded = codeword^(1'b1<<120); due_information = 1'b0; end
                8'b00110001:begin decoded = codeword^(1'b1<<119); due_information = 1'b0; end
                8'b11010101:begin decoded = codeword^(1'b1<<118); due_information = 1'b0; end
                8'b10111010:begin decoded = codeword^(1'b1<<117); due_information = 1'b0; end
                8'b00010011:begin decoded = codeword^(1'b1<<116); due_information = 1'b0; end
                8'b00111011:begin decoded = codeword^(1'b1<<115); due_information = 1'b0; end
                8'b11110111:begin decoded = codeword^(1'b1<<114); due_information = 1'b0; end
                8'b01111001:begin decoded = codeword^(1'b1<<113); due_information = 1'b0; end
                8'b10100010:begin decoded = codeword^(1'b1<<112); due_information = 1'b0; end
                8'b01101110:begin decoded = codeword^(1'b1<<111); due_information = 1'b0; end
                8'b01110101:begin decoded = codeword^(1'b1<<110); due_information = 1'b0; end
                8'b11110010:begin decoded = codeword^(1'b1<<109); due_information = 1'b0; end
                8'b00001101:begin decoded = codeword^(1'b1<<108); due_information = 1'b0; end
                8'b00101001:begin decoded = codeword^(1'b1<<107); due_information = 1'b0; end
                8'b10100110:begin decoded = codeword^(1'b1<<106); due_information = 1'b0; end
                8'b11001000:begin decoded = codeword^(1'b1<<105); due_information = 1'b0; end
                8'b00101100:begin decoded = codeword^(1'b1<<104); due_information = 1'b0; end
                8'b01001100:begin decoded = codeword^(1'b1<<103); due_information = 1'b0; end
                8'b00001011:begin decoded = codeword^(1'b1<<102); due_information = 1'b0; end
                8'b11100000:begin decoded = codeword^(1'b1<<101); due_information = 1'b0; end
                8'b11101001:begin decoded = codeword^(1'b1<<100); due_information = 1'b0; end
                8'b01100010:begin decoded = codeword^(1'b1<<99); due_information = 1'b0; end
                8'b10100000:begin decoded = codeword^(1'b1<<98); due_information = 1'b0; end
                8'b10111111:begin decoded = codeword^(1'b1<<97); due_information = 1'b0; end
                8'b10100111:begin decoded = codeword^(1'b1<<96); due_information = 1'b0; end
                8'b01101101:begin decoded = codeword^(1'b1<<95); due_information = 1'b0; end
                8'b11101100:begin decoded = codeword^(1'b1<<94); due_information = 1'b0; end
                8'b10101110:begin decoded = codeword^(1'b1<<93); due_information = 1'b0; end
                8'b00101010:begin decoded = codeword^(1'b1<<92); due_information = 1'b0; end
                8'b11010011:begin decoded = codeword^(1'b1<<91); due_information = 1'b0; end
                8'b10010010:begin decoded = codeword^(1'b1<<90); due_information = 1'b0; end
                8'b01101011:begin decoded = codeword^(1'b1<<89); due_information = 1'b0; end
                8'b01101000:begin decoded = codeword^(1'b1<<88); due_information = 1'b0; end
                8'b01011101:begin decoded = codeword^(1'b1<<87); due_information = 1'b0; end
                8'b10110000:begin decoded = codeword^(1'b1<<86); due_information = 1'b0; end
                8'b01100111:begin decoded = codeword^(1'b1<<85); due_information = 1'b0; end
                8'b11000100:begin decoded = codeword^(1'b1<<84); due_information = 1'b0; end
                8'b00000111:begin decoded = codeword^(1'b1<<83); due_information = 1'b0; end
                8'b11100110:begin decoded = codeword^(1'b1<<82); due_information = 1'b0; end
                8'b01011110:begin decoded = codeword^(1'b1<<81); due_information = 1'b0; end
                8'b11111000:begin decoded = codeword^(1'b1<<80); due_information = 1'b0; end
                8'b00101111:begin decoded = codeword^(1'b1<<79); due_information = 1'b0; end
                8'b11111101:begin decoded = codeword^(1'b1<<78); due_information = 1'b0; end
                8'b11001110:begin decoded = codeword^(1'b1<<77); due_information = 1'b0; end
                8'b10000110:begin decoded = codeword^(1'b1<<76); due_information = 1'b0; end
                8'b10001111:begin decoded = codeword^(1'b1<<75); due_information = 1'b0; end
                8'b01000110:begin decoded = codeword^(1'b1<<74); due_information = 1'b0; end
                8'b11101011:begin decoded = codeword^(1'b1<<73); due_information = 1'b0; end
                8'b00111110:begin decoded = codeword^(1'b1<<72); due_information = 1'b0; end
                8'b01110011:begin decoded = codeword^(1'b1<<71); due_information = 1'b0; end
                8'b11001011:begin decoded = codeword^(1'b1<<70); due_information = 1'b0; end
                8'b11011010:begin decoded = codeword^(1'b1<<69); due_information = 1'b0; end
                8'b10110101:begin decoded = codeword^(1'b1<<68); due_information = 1'b0; end
                8'b00001110:begin decoded = codeword^(1'b1<<67); due_information = 1'b0; end
                8'b00111000:begin decoded = codeword^(1'b1<<66); due_information = 1'b0; end
                8'b11000001:begin decoded = codeword^(1'b1<<65); due_information = 1'b0; end
                8'b01010111:begin decoded = codeword^(1'b1<<64); due_information = 1'b0; end
                8'b10101101:begin decoded = codeword^(1'b1<<63); due_information = 1'b0; end
                8'b10000101:begin decoded = codeword^(1'b1<<62); due_information = 1'b0; end
                8'b00100101:begin decoded = codeword^(1'b1<<61); due_information = 1'b0; end
                8'b10111100:begin decoded = codeword^(1'b1<<60); due_information = 1'b0; end
                8'b01110110:begin decoded = codeword^(1'b1<<59); due_information = 1'b0; end
                8'b11000010:begin decoded = codeword^(1'b1<<58); due_information = 1'b0; end
                8'b10010111:begin decoded = codeword^(1'b1<<57); due_information = 1'b0; end
                8'b10110011:begin decoded = codeword^(1'b1<<56); due_information = 1'b0; end
                8'b10011110:begin decoded = codeword^(1'b1<<55); due_information = 1'b0; end
                8'b11100011:begin decoded = codeword^(1'b1<<54); due_information = 1'b0; end
                8'b01111010:begin decoded = codeword^(1'b1<<53); due_information = 1'b0; end
                8'b11100111:begin decoded = codeword^(1'b1<<52); due_information = 1'b0; end
                8'b11100101:begin decoded = codeword^(1'b1<<51); due_information = 1'b0; end
                8'b10001101:begin decoded = codeword^(1'b1<<50); due_information = 1'b0; end
                8'b10001010:begin decoded = codeword^(1'b1<<49); due_information = 1'b0; end
                8'b11010110:begin decoded = codeword^(1'b1<<48); due_information = 1'b0; end
                8'b11101111:begin decoded = codeword^(1'b1<<47); due_information = 1'b0; end
                8'b11010000:begin decoded = codeword^(1'b1<<46); due_information = 1'b0; end
                8'b10101000:begin decoded = codeword^(1'b1<<45); due_information = 1'b0; end
                8'b10100001:begin decoded = codeword^(1'b1<<44); due_information = 1'b0; end
                8'b10010100:begin decoded = codeword^(1'b1<<43); due_information = 1'b0; end
                8'b00110111:begin decoded = codeword^(1'b1<<42); due_information = 1'b0; end
                8'b01000101:begin decoded = codeword^(1'b1<<41); due_information = 1'b0; end
                8'b00011100:begin decoded = codeword^(1'b1<<40); due_information = 1'b0; end
                8'b10110110:begin decoded = codeword^(1'b1<<39); due_information = 1'b0; end
                8'b01000011:begin decoded = codeword^(1'b1<<38); due_information = 1'b0; end
                8'b11110100:begin decoded = codeword^(1'b1<<37); due_information = 1'b0; end
                8'b11011001:begin decoded = codeword^(1'b1<<36); due_information = 1'b0; end
                8'b01111111:begin decoded = codeword^(1'b1<<35); due_information = 1'b0; end
                8'b10001100:begin decoded = codeword^(1'b1<<34); due_information = 1'b0; end
                8'b00011001:begin decoded = codeword^(1'b1<<33); due_information = 1'b0; end
                8'b11101010:begin decoded = codeword^(1'b1<<32); due_information = 1'b0; end
                8'b00100110:begin decoded = codeword^(1'b1<<31); due_information = 1'b0; end
                8'b10001001:begin decoded = codeword^(1'b1<<30); due_information = 1'b0; end
                8'b11011111:begin decoded = codeword^(1'b1<<29); due_information = 1'b0; end
                8'b01100001:begin decoded = codeword^(1'b1<<28); due_information = 1'b0; end
                8'b00010101:begin decoded = codeword^(1'b1<<27); due_information = 1'b0; end
                8'b11001101:begin decoded = codeword^(1'b1<<26); due_information = 1'b0; end
                8'b01010010:begin decoded = codeword^(1'b1<<25); due_information = 1'b0; end
                8'b11111110:begin decoded = codeword^(1'b1<<24); due_information = 1'b0; end
                8'b01100100:begin decoded = codeword^(1'b1<<23); due_information = 1'b0; end
                8'b00011010:begin decoded = codeword^(1'b1<<22); due_information = 1'b0; end
                8'b00110100:begin decoded = codeword^(1'b1<<21); due_information = 1'b0; end
                8'b01010100:begin decoded = codeword^(1'b1<<20); due_information = 1'b0; end
                8'b10011100:begin decoded = codeword^(1'b1<<19); due_information = 1'b0; end
                8'b01111100:begin decoded = codeword^(1'b1<<18); due_information = 1'b0; end
                8'b10100100:begin decoded = codeword^(1'b1<<17); due_information = 1'b0; end
                8'b10111001:begin decoded = codeword^(1'b1<<16); due_information = 1'b0; end
                8'b01001001:begin decoded = codeword^(1'b1<<15); due_information = 1'b0; end
                8'b10011000:begin decoded = codeword^(1'b1<<14); due_information = 1'b0; end
                8'b10110100:begin decoded = codeword^(1'b1<<13); due_information = 1'b0; end
                8'b00011111:begin decoded = codeword^(1'b1<<12); due_information = 1'b0; end
                8'b01001111:begin decoded = codeword^(1'b1<<11); due_information = 1'b0; end
                8'b11011100:begin decoded = codeword^(1'b1<<10); due_information = 1'b0; end
                8'b11111011:begin decoded = codeword^(1'b1<<9); due_information = 1'b0; end
                8'b00110010:begin decoded = codeword^(1'b1<<8); due_information = 1'b0; end
                8'b10000000:begin decoded = codeword^(1'b1<<7); due_information = 1'b0; end
                8'b01000000:begin decoded = codeword^(1'b1<<6); due_information = 1'b0; end
                8'b00100000:begin decoded = codeword^(1'b1<<5); due_information = 1'b0; end
                8'b00010000:begin decoded = codeword^(1'b1<<4); due_information = 1'b0; end
                8'b00001000:begin decoded = codeword^(1'b1<<3); due_information = 1'b0; end
                8'b00000100:begin decoded = codeword^(1'b1<<2); due_information = 1'b0; end
                8'b00000010:begin decoded = codeword^(1'b1<<1); due_information = 1'b0; end
                8'b00000001:begin decoded = codeword^(1'b1<<0); due_information = 1'b0; end

		// DUE (Detectable Error but Uncorrectable) 
		default: begin decoded = codeword; due_information = 1'b1; end
	endcase
	end
	assign message = decoded[135:8];
	assign due     = due_information


endmodule



