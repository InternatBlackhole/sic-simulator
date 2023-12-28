echo START 0

. main part test the subroutines
. char test
      LDA  #32
chtst JSUB char
      JSUB nl
      COMP #126
      ADD  #1
      JLT  chtst

. string test
      LDA  #printniz
      JSUB string
      JSUB nl

. num test
     LDA #42
     JSUB num
     JSUB nl
     LDA  minusone
     JSUB num
     JSUB nl
     +LDA #234213
     JSUB num
     JSUB nl
     +LDA #322124
     MUL  minusone
     JSUB num
     JSUB nl
     +LDA #0xfffff
     JSUB num
     JSUB nl
     MUL minusone
     JSUB num
     JSUB nl
     +LDA #0
     JSUB num
     JSUB nl

halt J halt

char  WD #1
      RSUB

nl  STA regcopy1
    LDA #10
    WD  #1
    LDA regcopy1
    RSUB

string STA   regcopy1
       STA   dref
       . index addressing can't be done with indirect addressing
loopp  LDA   #0 . clear A
       LDCH  @dref
       COMP  #0
       JEQ   loppd
       STL   regcopy2
       JSUB  char
       LDL   regcopy2
       LDA   dref
       ADD   #1
       STA   dref
       J     loopp
loppd  LDA   regcopy1
       RSUB

. first check if it a negative number and print minus if necessary and convert it to a positive number
num STA  regcopy1
    STX  regcopy3
    STT  regcopy4
    . easy fast fix for 0
    COMP #0
    JEQ  is0
    LDT  #0
    STT  regcopy2 . set it to zero
    AND  minusmask
    SHIFTR A,16
    SHIFTR A,7
    COMP minusone . shiftr is aritmetic
    LDA  regcopy1
    JEQ  eq
    . number is positive
pos LDX  #0
    RMO  A,S  . S has number before division
lop COMPR S,T
    . we are done, print it
    JEQ  ifd
    RMO  S,A
    DIV  #10
    STA  regcopy2
    MUL  #10
    SUBR A,S
    . S has the modulo
    STA  dref
    RMO  S,A
    ADD  #48
    STCH decWord,X
    LDA  dref
    TIX  #0
    LDS  regcopy2
    J    lop
eq  STA  dref
    LDCH #45
    STL  regcopy2
    JSUB char
    LDL  regcopy2
    LDA  dref
    MUL  minusone
    J    pos
    . now print it
ifd LDT  #1
    SUBR T,X
ifl LDT  #0
    COMPR X,T
    JLT  don
    LDCH decWord,X
    STL  regcopy2
    JSUB char
    LDL  regcopy2
    LDT  #1
    SUBR T,X
    J    ifl
is0 LDA #48 . 0 in ascii
    STL regcopy2
    JSUB char
    LDL regcopy2
don LDX regcopy3
    LDA regcopy1
    LDT regcopy4
    RSUB

printniz WORD C'HELLO PRINT ME i am a test of this working.,...!!!!'
regcopy1 WORD 0
regcopy2 WORD 0
regcopy3 WORD 0
regcopy4 WORD 0

dref     WORD 0
minusmask WORD 0x800000
minusone WORD -1

decWord  RESW 8 . ceil(log(2^24))