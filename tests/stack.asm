stack START 0
    
        JSUB stackinit

        . fakulteta
        LDA #12
        . A has value of fakulteta
        JSUB fakulteta
        JSUB num
        JSUB nl
halt    J    halt

fakulteta COMP #0
          JEQ  fdone
          STL  @stackptr
          JSUB stackpush
          STA  @stackptr
          JSUB stackpush
          SUB  #1
          JSUB fakulteta
          JSUB stackpop
          LDS  @stackptr
          MULR S,A
          JSUB stackpop
          LDL  @stackptr
          RSUB
fdone     LDA  #1
          RSUB  

char  WD #1
      RSUB

nl  STA regcopy1
    LDA #10
    WD  #1
    LDA regcopy1
    RSUB

num STA  regcopy1
    STX  regcopy3
    STT  regcopy4
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
don LDX regcopy3
    LDA regcopy1
    LDT regcopy4
    RSUB

stackinit STA  regcopy1
          +LDA  #0xfffc
          STA  stackptr
          LDA  regcopy1
          RSUB

stackpush STA  regcopy1
          LDA  stackptr
          ADD  #3
          STA  stackptr
          LDA  regcopy1
          RSUB

stackpop  STA  regcopy1
          LDA  stackptr
          SUB  #3
          STA  stackptr
          LDA  regcopy1
          RSUB

regcopy1 WORD 0
regcopy2 WORD 0
regcopy3 WORD 0
regcopy4 WORD 0
dref     WORD 0
minusmask WORD 0x800000
minusone WORD -1
decWord  RESW 8 . ceil(log(2^24))

stackptr WORD 0