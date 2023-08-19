module ERASURE_LOCATION(input [9:0] DUE_information_in,
                      output [3:0] First_error_location_out,
                      output [3:0] Second_error_location_out
                      );

  // (i) First error location: 0~8
  // (j) Second error location: 1~9
  // i < j

  reg [3:0] First_error_loation;
  reg [3:0] Second_error_location;

   always @(*) begin
        int cnt=0;
        for (int index=9; index>=0; index=index-1)begin
            if(cnt==0 && DUE_information_in[index]==1)begin
               First_error_loation=10-index-1;
               cnt=1;
            end
            else if(cnt==1 && DUE_information_in[index]==1)begin
               Second_error_location=10-index-1;
               cnt=2;
            end
        end
   end

  assign First_error_location_out = First_error_loation;
  assign Second_error_location_out = Second_error_location;

endmodule


