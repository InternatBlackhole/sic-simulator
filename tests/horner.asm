horner START 0
     LDX #0 . tracks coeff position and power
     LDA #koef
     STA next
loop LDA @next
     RMO A,T . save A to T
     JSUB power . saves the power value to A, X has the power
     MULR T,A . mul T and A
     RMO A,S
     LDA result
     ADDR S,A
     STA result
     LDA next
     ADD #3
     TIX #0
     STA next . i hate this assembly language
     COMP #x
     JLT loop
halt J  halt

. saves the power value to A, X has the power
power   STA copyA
        STX copyX
        STS copyS
        LDX #0
        LDS copyX
        COMPR X,S
        JEQ donA
        LDA #1
ploop   MUL x
        TIXR S
        JLT ploop
        J  done
donA    LDA #1
done    LDX copyX
        LDS copyS
        RSUB

copyA   WORD 0
copyX   WORD 0
copyL   WORD 0
copyS   WORD 0
copyT   WORD 0
copyB   WORD 0

koef    WORD 5
        WORD 4
        WORD 3
        WORD 2
        WORD 1

x      WORD 2
result RESW 1
next   WORD 0