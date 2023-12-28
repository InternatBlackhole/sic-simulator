poly START 0
. V tej nalogi izračunamo vrednost polinoma x⁴+2x³+3x²+4x+5 v točki x=2
    LDA x
    MUL x
    MUL x
    MUL x
    ADDR A,S

    LDA x
    MUL x
    MUL x
    MUL #2
    ADDR A,S

    LDA x
    MUL x
    MUL #3
    ADDR A,S

    LDA x
    MUL #4

    ADD #5
    ADDR A,S

    STS res

halt J halt

x   WORD 2
res WORD 0 . should be 57