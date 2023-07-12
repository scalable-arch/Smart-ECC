module bch_chien(received, locator0, locator1, locator2, codeword);
  input [14:0] received;
  input [3:0] locator0;
  input [3:0] locator1;
  input [3:0] locator2;
  output [14:0] codeword;

  wire [14:0] error;
  wire [3:0] alpha0_0_x_locator0;
  wire [3:0] alpha0_1_x_locator1;
  wire [3:0] alpha0_2_x_locator2;
  wire [3:0] alpha1_0_x_locator0;
  wire [3:0] alpha1_1_x_locator1;
  wire [3:0] alpha1_2_x_locator2;
  wire [3:0] alpha2_0_x_locator0;
  wire [3:0] alpha2_1_x_locator1;
  wire [3:0] alpha2_2_x_locator2;
  wire [3:0] alpha3_0_x_locator0;
  wire [3:0] alpha3_1_x_locator1;
  wire [3:0] alpha3_2_x_locator2;
  wire [3:0] alpha4_0_x_locator0;
  wire [3:0] alpha4_1_x_locator1;
  wire [3:0] alpha4_2_x_locator2;
  wire [3:0] alpha5_0_x_locator0;
  wire [3:0] alpha5_1_x_locator1;
  wire [3:0] alpha5_2_x_locator2;
  wire [3:0] alpha6_0_x_locator0;
  wire [3:0] alpha6_1_x_locator1;
  wire [3:0] alpha6_2_x_locator2;
  wire [3:0] alpha7_0_x_locator0;
  wire [3:0] alpha7_1_x_locator1;
  wire [3:0] alpha7_2_x_locator2;
  wire [3:0] alpha8_0_x_locator0;
  wire [3:0] alpha8_1_x_locator1;
  wire [3:0] alpha8_2_x_locator2;
  wire [3:0] alpha9_0_x_locator0;
  wire [3:0] alpha9_1_x_locator1;
  wire [3:0] alpha9_2_x_locator2;
  wire [3:0] alpha10_0_x_locator0;
  wire [3:0] alpha10_1_x_locator1;
  wire [3:0] alpha10_2_x_locator2;
  wire [3:0] alpha11_0_x_locator0;
  wire [3:0] alpha11_1_x_locator1;
  wire [3:0] alpha11_2_x_locator2;
  wire [3:0] alpha12_0_x_locator0;
  wire [3:0] alpha12_1_x_locator1;
  wire [3:0] alpha12_2_x_locator2;
  wire [3:0] alpha13_0_x_locator0;
  wire [3:0] alpha13_1_x_locator1;
  wire [3:0] alpha13_2_x_locator2;
  wire [3:0] alpha14_0_x_locator0;
  wire [3:0] alpha14_1_x_locator1;
  wire [3:0] alpha14_2_x_locator2;

  gfmult gfm_alpha0_0_x_locator0(4'b0001, locator0, alpha0_0_x_locator0);
  gfmult gfm_alpha0_1_x_locator1(4'b0001, locator1, alpha0_1_x_locator1);
  gfmult gfm_alpha0_2_x_locator2(4'b0001, locator2, alpha0_2_x_locator2);
  gfmult gfm_alpha1_0_x_locator0(4'b0001, locator0, alpha1_0_x_locator0);
  gfmult gfm_alpha1_1_x_locator1(4'b1001, locator1, alpha1_1_x_locator1);
  gfmult gfm_alpha1_2_x_locator2(4'b1101, locator2, alpha1_2_x_locator2);
  gfmult gfm_alpha2_0_x_locator0(4'b0001, locator0, alpha2_0_x_locator0);
  gfmult gfm_alpha2_1_x_locator1(4'b1101, locator1, alpha2_1_x_locator1);
  gfmult gfm_alpha2_2_x_locator2(4'b1110, locator2, alpha2_2_x_locator2);
  gfmult gfm_alpha3_0_x_locator0(4'b0001, locator0, alpha3_0_x_locator0);
  gfmult gfm_alpha3_1_x_locator1(4'b1111, locator1, alpha3_1_x_locator1);
  gfmult gfm_alpha3_2_x_locator2(4'b1010, locator2, alpha3_2_x_locator2);
  gfmult gfm_alpha4_0_x_locator0(4'b0001, locator0, alpha4_0_x_locator0);
  gfmult gfm_alpha4_1_x_locator1(4'b1110, locator1, alpha4_1_x_locator1);
  gfmult gfm_alpha4_2_x_locator2(4'b1011, locator2, alpha4_2_x_locator2);
  gfmult gfm_alpha5_0_x_locator0(4'b0001, locator0, alpha5_0_x_locator0);
  gfmult gfm_alpha5_1_x_locator1(4'b0111, locator1, alpha5_1_x_locator1);
  gfmult gfm_alpha5_2_x_locator2(4'b0110, locator2, alpha5_2_x_locator2);
  gfmult gfm_alpha6_0_x_locator0(4'b0001, locator0, alpha6_0_x_locator0);
  gfmult gfm_alpha6_1_x_locator1(4'b1010, locator1, alpha6_1_x_locator1);
  gfmult gfm_alpha6_2_x_locator2(4'b1000, locator2, alpha6_2_x_locator2);
  gfmult gfm_alpha7_0_x_locator0(4'b0001, locator0, alpha7_0_x_locator0);
  gfmult gfm_alpha7_1_x_locator1(4'b0101, locator1, alpha7_1_x_locator1);
  gfmult gfm_alpha7_2_x_locator2(4'b0010, locator2, alpha7_2_x_locator2);
  gfmult gfm_alpha8_0_x_locator0(4'b0001, locator0, alpha8_0_x_locator0);
  gfmult gfm_alpha8_1_x_locator1(4'b1011, locator1, alpha8_1_x_locator1);
  gfmult gfm_alpha8_2_x_locator2(4'b1001, locator2, alpha8_2_x_locator2);
  gfmult gfm_alpha9_0_x_locator0(4'b0001, locator0, alpha9_0_x_locator0);
  gfmult gfm_alpha9_1_x_locator1(4'b1100, locator1, alpha9_1_x_locator1);
  gfmult gfm_alpha9_2_x_locator2(4'b1111, locator2, alpha9_2_x_locator2);
  gfmult gfm_alpha10_0_x_locator0(4'b0001, locator0, alpha10_0_x_locator0);
  gfmult gfm_alpha10_1_x_locator1(4'b0110, locator1, alpha10_1_x_locator1);
  gfmult gfm_alpha10_2_x_locator2(4'b0111, locator2, alpha10_2_x_locator2);
  gfmult gfm_alpha11_0_x_locator0(4'b0001, locator0, alpha11_0_x_locator0);
  gfmult gfm_alpha11_1_x_locator1(4'b0011, locator1, alpha11_1_x_locator1);
  gfmult gfm_alpha11_2_x_locator2(4'b0101, locator2, alpha11_2_x_locator2);
  gfmult gfm_alpha12_0_x_locator0(4'b0001, locator0, alpha12_0_x_locator0);
  gfmult gfm_alpha12_1_x_locator1(4'b1000, locator1, alpha12_1_x_locator1);
  gfmult gfm_alpha12_2_x_locator2(4'b1100, locator2, alpha12_2_x_locator2);
  gfmult gfm_alpha13_0_x_locator0(4'b0001, locator0, alpha13_0_x_locator0);
  gfmult gfm_alpha13_1_x_locator1(4'b0100, locator1, alpha13_1_x_locator1);
  gfmult gfm_alpha13_2_x_locator2(4'b0011, locator2, alpha13_2_x_locator2);
  gfmult gfm_alpha14_0_x_locator0(4'b0001, locator0, alpha14_0_x_locator0);
  gfmult gfm_alpha14_1_x_locator1(4'b0010, locator1, alpha14_1_x_locator1);
  gfmult gfm_alpha14_2_x_locator2(4'b0100, locator2, alpha14_2_x_locator2);

  assign codeword = received ^ error;
  assign error = {(~| (alpha14_0_x_locator0 ^ alpha14_1_x_locator1 ^ alpha14_2_x_locator2)), (~| (alpha13_0_x_locator0 ^ alpha13_1_x_locator1 ^ alpha13_2_x_locator2)), (~| (alpha12_0_x_locator0 ^ alpha12_1_x_locator1 ^ alpha12_2_x_locator2)), (~| (alpha11_0_x_locator0 ^ alpha11_1_x_locator1 ^ alpha11_2_x_locator2)), (~| (alpha10_0_x_locator0 ^ alpha10_1_x_locator1 ^ alpha10_2_x_locator2)), (~| (alpha9_0_x_locator0 ^ alpha9_1_x_locator1 ^ alpha9_2_x_locator2)), (~| (alpha8_0_x_locator0 ^ alpha8_1_x_locator1 ^ alpha8_2_x_locator2)), (~| (alpha7_0_x_locator0 ^ alpha7_1_x_locator1 ^ alpha7_2_x_locator2)), (~| (alpha6_0_x_locator0 ^ alpha6_1_x_locator1 ^ alpha6_2_x_locator2)), (~| (alpha5_0_x_locator0 ^ alpha5_1_x_locator1 ^ alpha5_2_x_locator2)), (~| (alpha4_0_x_locator0 ^ alpha4_1_x_locator1 ^ alpha4_2_x_locator2)), (~| (alpha3_0_x_locator0 ^ alpha3_1_x_locator1 ^ alpha3_2_x_locator2)), (~| (alpha2_0_x_locator0 ^ alpha2_1_x_locator1 ^ alpha2_2_x_locator2)), (~| (alpha1_0_x_locator0 ^ alpha1_1_x_locator1 ^ alpha1_2_x_locator2)), (~| (alpha0_0_x_locator0 ^ alpha0_1_x_locator1 ^ alpha0_2_x_locator2))};
endmodule
