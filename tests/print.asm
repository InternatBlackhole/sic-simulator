print START 0

      LDX   #0
loop  LDCH   niz,X
      COMP  #0
      JEQ   lopd
      WD    #0xAA
      TIX   #0
      J     loop
lopd  LDA   #10
      WD    #0xAA
halt  J halt

niz   WORD C'SIC/XE'