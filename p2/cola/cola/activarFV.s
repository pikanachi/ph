
            AREA datos, DATA

SET_V       EQU 0x10000000      ; mascara para activar FV en cpsr
            PRESERVE8

            AREA codigo, CODE
            EXPORT activarFV
            ENTRY

activarFV   PUSH {r4,lr}        ; guardar regs a usar y el pc de vuelta
            mrs r4,cpsr         ; copio en r4 el cpsr actual
            orr r4,r4,#SET_V    ; or bit a bit entre el cpsr y la mascara que actuva flag V
            MSR cpsr_cxsf,r4    ; escribir el nuevo cpsr
            POP {r4,pc}         ; pop de los regs usados y vuelta del pc

; cpsr
; -----------------------------------
; | n  | z  | c  | v  |        ...  |
; -----------------------------------
; | 31 | 30 | 29 | 28 |        ...0 |