module GFADD(a, b, c);
  input [7:0] a;
  input [7:0] b;
  output [7:0] c;

  /*
    GF(2^8)
    primitive polynomial : x^8 + x6 + x^4 + x^3 + x^2 + x + 1
    a = a^n
    b = a^m
    c = a^n + a^m
  */
  
  assign c = a ^ b;

endmodule


