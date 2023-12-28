arithr START 0
    LDS x
    LDT y

    . sum
    ADDR S,T
    STT  sum
    SUBR S,T

    . diff
    SUBR T,S
    STS  diff
    ADDR T,S

    . prod
    MULR S,T
    STT  prod
    DIVR S,T

    . quot
    STS  mod
    DIVR T,S
    STS  quot 

    .mod
    MULR T,S
    LDB  mod
    SUBR S,B
    STB  mod


halt J halt



x     WORD 100
y     WORD 22

sum   WORD 0
diff  WORD 0
prod  WORD 0
quot  WORD 0
mod   WORD 0