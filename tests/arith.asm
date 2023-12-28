arith START 0

. sum
      LDA x
      ADD y
      STA sum

. diff
      LDA x
      SUB y
      STA diff

. prod
      LDA x
      MUL y
      STA prod

. quot
      LDA x
      DIV y
      STA quot

. mod
      MUL y
      STA mod
      LDA x
      SUB mod
      STA mod

halt  J halt

x     WORD 100
y     WORD 22

sum   WORD 0
diff  WORD 0
prod  WORD 0
quot  WORD 0
mod   WORD 0