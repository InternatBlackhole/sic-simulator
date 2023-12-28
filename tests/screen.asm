scrasm START 0

     LDA #65
     JSUB scrfill

     JSUB scrclear

halt J halt

scrclear LDA   #0
scrfill  LDX   screen
         STX   scrptr
         RMO   A,T
         LDA   screen
         ADD   #scrlen
         RMO   A,S
         RMO   T,A
loop     +STCH @scrptr
         TIX   #scrlen
         STX   scrptr
         COMPR X,S
         JLT   loop
done     RSUB

scrptr  WORD 0

screen  WORD 0xB800
scrcols WORD 80
scrrows WORD 25
scrlen  EQU  80*25