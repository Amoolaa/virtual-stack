FUNC LABEL 0
    MOV STK A VAL 10
    REF STK B STK A
    MOV REG 0 STK B
    MOV STK C REG 0
    PRINT PTR C
    RET
