module GFINDEX(a, b);
  input [7:0] a;
  output [7:0] b;

  /*
    GF(2^8)
    primitive polynomial : x^8 + x6 + x^4 + x^3 + x^2 + x + 1
    a = n
    b = a^n
  */
  
  reg [7:0] binv;
  always_comb
      case (a)
        8'd0: binv = 8'b0000_0001;
        8'd1: binv = 8'b0000_0010; 
        8'd2: binv = 8'b0000_0100;
        8'd3: binv = 8'b0000_1000;
        8'd4: binv = 8'b0001_0000;
        8'd5: binv = 8'b0010_0000;
        8'd6: binv = 8'b0100_0000;
        8'd7: binv = 8'b1000_0000;
        8'd8: binv = 8'b0101_1111;
        8'd9: binv = 8'b1011_1110;
        8'd10: binv = 8'b0010_0011;
        8'd11: binv = 8'b0100_0110;
        8'd12: binv = 8'b1000_1100;
        8'd13: binv = 8'b0100_0111;
        8'd14: binv = 8'b1000_1110;
        8'd15: binv = 8'b0100_0011;
        8'd16: binv = 8'b1000_0110;
        8'd17: binv = 8'b0101_0011;
        8'd18: binv = 8'b1010_0110;
        8'd19: binv = 8'b0001_0011;
        8'd20: binv = 8'b0010_0110;
        8'd21: binv = 8'b0100_1100;
        8'd22: binv = 8'b1001_1000;
        8'd23: binv = 8'b0110_1111;
        8'd24: binv = 8'b1101_1110;
        8'd25: binv = 8'b1110_0011;
        8'd26: binv = 8'b1001_1001;
        8'd27: binv = 8'b0110_1101;
        8'd28: binv = 8'b1101_1010;
        8'd29: binv = 8'b1110_1011;
        8'd30: binv = 8'b1000_1001;
        8'd31: binv = 8'b0100_1101;
        8'd32: binv = 8'b1001_1010;
        8'd33: binv = 8'b0110_1011;
        8'd34: binv = 8'b1101_0110;
        8'd35: binv = 8'b1111_0011;
        8'd36: binv = 8'b1011_1001;
        8'd37: binv = 8'b0010_1101;
        8'd38: binv = 8'b0101_1010;
        8'd39: binv = 8'b1011_0100;
        8'd40: binv = 8'b0011_0111;
        8'd41: binv = 8'b0110_1110;
        8'd42: binv = 8'b1101_1100;
        8'd43: binv = 8'b1110_0111;
        8'd44: binv = 8'b1001_0001;
        8'd45: binv = 8'b0111_1101;
        8'd46: binv = 8'b1111_1010;
        8'd47: binv = 8'b1010_1011;
        8'd48: binv = 8'b0000_1001;
        8'd49: binv = 8'b0001_0010;
        8'd50: binv = 8'b0010_0100;
        8'd51: binv = 8'b0100_1000;
        8'd52: binv = 8'b1001_0000;
        8'd53: binv = 8'b0111_1111;
        8'd54: binv = 8'b1111_1110;
        8'd55: binv = 8'b1010_0011;
        8'd56: binv = 8'b0001_1001;
        8'd57: binv = 8'b0011_0010;
        8'd58: binv = 8'b0110_0100;
        8'd59: binv = 8'b1100_1000;
        8'd60: binv = 8'b1100_1111;
        8'd61: binv = 8'b1100_0001;
        8'd62: binv = 8'b1101_1101;
        8'd63: binv = 8'b1110_0101;
        8'd64: binv = 8'b1001_0101;
        8'd65: binv = 8'b0111_0101;
        8'd66: binv = 8'b1110_1010;
        8'd67: binv = 8'b1000_1011;
        8'd68: binv = 8'b0100_1001;
        8'd69: binv = 8'b1001_0010;
        8'd70: binv = 8'b0111_1011;
        8'd71: binv = 8'b1111_0110;
        8'd72: binv = 8'b1011_0011;
        8'd73: binv = 8'b0011_1001;
        8'd74: binv = 8'b0111_0010;
        8'd75: binv = 8'b1110_0100;
        8'd76: binv = 8'b1001_0111;
        8'd77: binv = 8'b0111_0001;
        8'd78: binv = 8'b1110_0010;
        8'd79: binv = 8'b1001_1011;
        8'd80: binv = 8'b0110_1001;
        8'd81: binv = 8'b1101_0010;
        8'd82: binv = 8'b1111_1011;
        8'd83: binv = 8'b1010_1001;
        8'd84: binv = 8'b0000_1101;
        8'd85: binv = 8'b0001_1010;
        8'd86: binv = 8'b0011_0100;
        8'd87: binv = 8'b0110_1000;
        8'd88: binv = 8'b1101_0000;
        8'd89: binv = 8'b1111_1111;
        8'd90: binv = 8'b1010_0001;
        8'd91: binv = 8'b0001_1101;
        8'd92: binv = 8'b0011_1010;
        8'd93: binv = 8'b0111_0100;
        8'd94: binv = 8'b1110_1000;
        8'd95: binv = 8'b1000_1111;
        8'd96: binv = 8'b0100_0001;
        8'd97: binv = 8'b1000_0010;
        8'd98: binv = 8'b0101_1011;
        8'd99: binv = 8'b1011_0110;
        8'd100: binv = 8'b0011_0011;
        8'd101: binv = 8'b0110_0110;
        8'd102: binv = 8'b1100_1100;
        8'd103: binv = 8'b1100_0111;
        8'd104: binv = 8'b1101_0001;
        8'd105: binv = 8'b1111_1101;
        8'd106: binv = 8'b1010_0101;
        8'd107: binv = 8'b0001_0101;
        8'd108: binv = 8'b0010_1010;
        8'd109: binv = 8'b0101_0100;
        8'd110: binv = 8'b1010_1000;
        8'd111: binv = 8'b0000_1111;
        8'd112: binv = 8'b0001_1110;
        8'd113: binv = 8'b0011_1100;
        8'd114: binv = 8'b0111_1000;
        8'd115: binv = 8'b1111_0000;
        8'd116: binv = 8'b1011_1111;
        8'd117: binv = 8'b0010_0001;
        8'd118: binv = 8'b0100_0010;
        8'd119: binv = 8'b1000_0100;
        8'd120: binv = 8'b0101_0111;
        8'd121: binv = 8'b1010_1110;
        8'd122: binv = 8'b0000_0011;
        8'd123: binv = 8'b0000_0110;
        8'd124: binv = 8'b0000_1100;
        8'd125: binv = 8'b0001_1000;
        8'd126: binv = 8'b0011_0000;
        8'd127: binv = 8'b0110_0000;
        8'd128: binv = 8'b1100_0000;
        8'd129: binv = 8'b1101_1111;
        8'd130: binv = 8'b1110_0001;
        8'd131: binv = 8'b1001_1101;
        8'd132: binv = 8'b0110_0101;
        8'd133: binv = 8'b1100_1010;
        8'd134: binv = 8'b1100_1011;
        8'd135: binv = 8'b1100_1001;
        8'd136: binv = 8'b1100_1101;
        8'd137: binv = 8'b1100_0101;
        8'd138: binv = 8'b1101_0101;
        8'd139: binv = 8'b1111_0101;
        8'd140: binv = 8'b1011_0101;
        8'd141: binv = 8'b0011_0101;
        8'd142: binv = 8'b0110_1010;
        8'd143: binv = 8'b1101_0100;
        8'd144: binv = 8'b1111_0111;
        8'd145: binv = 8'b1011_0001;
        8'd146: binv = 8'b0011_1101;
        8'd147: binv = 8'b0111_1010;
        8'd148: binv = 8'b1111_0100;
        8'd149: binv = 8'b1011_0111;
        8'd150: binv = 8'b0011_0001;
        8'd151: binv = 8'b0110_0010;
        8'd152: binv = 8'b1100_0100;
        8'd153: binv = 8'b1101_0111;
        8'd154: binv = 8'b1111_0001;
        8'd155: binv = 8'b1011_1101;
        8'd156: binv = 8'b0010_0101;
        8'd157: binv = 8'b0100_1010;
        8'd158: binv = 8'b1001_0100;
        8'd159: binv = 8'b0111_0111;
        8'd160: binv = 8'b1110_1110;
        8'd161: binv = 8'b1000_0011;
        8'd162: binv = 8'b0101_1001;
        8'd163: binv = 8'b1011_0010;
        8'd164: binv = 8'b0011_1011;
        8'd165: binv = 8'b0111_0110;
        8'd166: binv = 8'b1110_1100;
        8'd167: binv = 8'b1000_0111;
        8'd168: binv = 8'b0101_0001;
        8'd169: binv = 8'b1010_0010;
        8'd170: binv = 8'b0001_1011;
        8'd171: binv = 8'b0011_0110;
        8'd172: binv = 8'b0110_1100;
        8'd173: binv = 8'b1101_1000;
        8'd174: binv = 8'b1110_1111;
        8'd175: binv = 8'b1000_0001;
        8'd176: binv = 8'b0101_1101;
        8'd177: binv = 8'b1011_1010;
        8'd178: binv = 8'b0010_1011;
        8'd179: binv = 8'b0101_0110;
        8'd180: binv = 8'b1010_1100;
        8'd181: binv = 8'b0000_0111;
        8'd182: binv = 8'b0000_1110;
        8'd183: binv = 8'b0001_1100;
        8'd184: binv = 8'b0011_1000;
        8'd185: binv = 8'b0111_0000;
        8'd186: binv = 8'b1110_0000;
        8'd187: binv = 8'b1001_1111;
        8'd188: binv = 8'b0110_0001;
        8'd189: binv = 8'b1100_0010;
        8'd190: binv = 8'b1101_1011;
        8'd191: binv = 8'b1110_1001;
        8'd192: binv = 8'b1000_1101;
        8'd193: binv = 8'b0100_0101;
        8'd194: binv = 8'b1000_1010;
        8'd195: binv = 8'b0100_1011;
        8'd196: binv = 8'b1001_0110;
        8'd197: binv = 8'b0111_0011;
        8'd198: binv = 8'b1110_0110;
        8'd199: binv = 8'b1001_0011;
        8'd200: binv = 8'b0111_1001;
        8'd201: binv = 8'b1111_0010;
        8'd202: binv = 8'b1011_1011;
        8'd203: binv = 8'b0010_1001;
        8'd204: binv = 8'b0101_0010;
        8'd205: binv = 8'b1010_0100;
        8'd206: binv = 8'b0001_0111;
        8'd207: binv = 8'b0010_1110;
        8'd208: binv = 8'b0101_1100;
        8'd209: binv = 8'b1011_1000;
        8'd210: binv = 8'b0010_1111;
        8'd211: binv = 8'b0101_1110;
        8'd212: binv = 8'b1011_1100;
        8'd213: binv = 8'b0010_0111;
        8'd214: binv = 8'b0100_1110;
        8'd215: binv = 8'b1001_1100;
        8'd216: binv = 8'b0110_0111;
        8'd217: binv = 8'b1100_1110;
        8'd218: binv = 8'b1100_0011;
        8'd219: binv = 8'b1101_1001;
        8'd220: binv = 8'b1110_1101;
        8'd221: binv = 8'b1000_0101;
        8'd222: binv = 8'b0101_0101;
        8'd223: binv = 8'b1010_1010;
        8'd224: binv = 8'b0000_1011;
        8'd225: binv = 8'b0001_0110;
        8'd226: binv = 8'b0010_1100;
        8'd227: binv = 8'b0101_1000;
        8'd228: binv = 8'b1011_0000;
        8'd229: binv = 8'b0011_1111;
        8'd230: binv = 8'b0111_1110;
        8'd231: binv = 8'b1111_1100;
        8'd232: binv = 8'b1010_0111;
        8'd233: binv = 8'b0001_0001;
        8'd234: binv = 8'b0010_0010;
        8'd235: binv = 8'b0100_0100;
        8'd236: binv = 8'b1000_1000;
        8'd237: binv = 8'b0100_1111;
        8'd238: binv = 8'b1001_1110;
        8'd239: binv = 8'b0110_0011;
        8'd240: binv = 8'b1100_0110;
        8'd241: binv = 8'b1101_0011;
        8'd242: binv = 8'b1111_1001;
        8'd243: binv = 8'b1010_1101;
        8'd244: binv = 8'b0000_0101;
        8'd245: binv = 8'b0000_1010;
        8'd246: binv = 8'b0001_0100;
        8'd247: binv = 8'b0010_1000;
        8'd248: binv = 8'b0101_0000;
        8'd249: binv = 8'b1010_0000;
        8'd250: binv = 8'b0001_1111;
        8'd251: binv = 8'b0011_1110;
        8'd252: binv = 8'b0111_1100;
        8'd253: binv = 8'b1111_1000;
        8'd254: binv = 8'b1010_1111;
      endcase
    assign b = binv;

endmodule

