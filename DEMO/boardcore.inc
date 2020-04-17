;  STM8S device dependent HW routines


;       BOARDINIT  ( -- )
;       Init board GPIO (except COM ports)
BOARDINIT:
        ; Board I/O initialization
        ; STM8S103F3 init GPIO
        MOV     PA_DDR,#0b00001110 ; G,D,E
        MOV     PA_CR1,#0b00001110
        MOV     PB_DDR,#0b00110000 ; A,P
        MOV     PB_CR1,#0b00110000
        MOV     PC_DDR,#0b11111000 ; d3,d2,F,C,B
        MOV     PC_CR1,#0b11111000
        MOV     PD_DDR,#0b00010100 ; d4,d1
        MOV     PD_CR1,#0b00010100

        .ifne   HAS_OUTPUTS
        CLR     A
        JRA     AOUTSTOR
        .else
        RET
        .endif

;===============================================================

;      Dummy labels for PSIM interrupts declared in main.c

        .ifne   PSIM-PORTA
;       Dummy label for _EXTIA_IRQHandler
_EXTI0_IRQHandler:
        .endif

        .ifne   PSIM-PORTB
;       Dummy label for _EXTIB_IRQHandler
_EXTI1_IRQHandler:
        .endif

        .ifne   PSIM-PORTC
;       Dummy label for _EXTIC_IRQHandler
_EXTI2_IRQHandler:
        .endif

        .ifne   PSIM-PORTD
;       Dummy label for _EXTID_IRQHandler
_EXTI3_IRQHandler:
        .endif

        .ifne   PSIM-PORTE
;       Dummy label for _EXTIE_IRQHandler
_EXTI4_IRQHandler:
        .endif

;===============================================================

        .ifne   HAS_LED7SEG
;       LED_MPX driver ( -- )
;       Output bit pattern in A to 7S-LED digit hardware

; Common Anode display:
;   Digits - active high
;   Segments - active low

LED_MPX:
        BRES    PD_ODR,#2       ; Digit 4... d1
        BRES    PC_ODR,#7       ; Digit .3.. d2
        BRES    PC_ODR,#6       ; Digit ..2. d3
        BRES    PD_ODR,#4       ; Digit ...1 d4

        LD      A,TICKCNT+1
        AND     A,#3            ; 4 digits MPX

        JRNE    0$
        BSET    PD_ODR,#2       ; digit 4...
        JRA     3$
0$:     CP      A,#1
        JRNE    1$
        BSET    PC_ODR,#7       ; digit .3..
        JRA     3$
1$:     CP      A,#2
        JRNE    2$
        BSET    PC_ODR,#6       ; digit ..2.
        JRA     3$

2$:     BSET    PD_ODR,#4       ; digit ...1
        ; fall through

3$:     CLRW    X
        LD      XL,A
        LD      A,(LED7LAST-3,X)

        CPL     A               ; invert bits for active low
        ; 7S LED display row
        ; bit 76543210 in A
        ;  PA ....GDE.
        ;  PB ..AP....
        ;  PC ..FCB...
        RRC     A               ; A
        BCCM    PB_ODR,#5       
        RRC     A               ; B
        BCCM    PC_ODR,#3      
        RRC     A               ; C
        BCCM    PC_ODR,#4     
        RRC     A               ; D
        BCCM    PA_ODR,#2  
        RRC     A               ; E
        BCCM    PA_ODR,#1   
        RRC     A               ; F
        BCCM    PC_ODR,#5    
        RRC     A               ; G
        BCCM    PA_ODR,#3  
        RRC     A               ; P
        BCCM    PB_ODR,#4 
4$:     RET
        .endif

;===============================================================


        .ifne   HAS_OUTPUTS
;       OUT!  ( c -- )
;       Put c to board outputs, storing a copy in OUTPUTS
        .dw     LINK

        LINK =  .
        .db     (4)
        .ascii  "OUT!"
OUTSTOR:
        RET
        .endif

;===============================================================

        .ifne   HAS_KEYS

;       BKEY  ( -- f )     ( TOS STM8: -- A,Z,N )
;       Read board key state as a bitfield

        .dw     LINK

        LINK =  .
        .db     (4)
        .ascii  "BKEY"
BKEY:
        CLR     A
        JP      ASTOR


;       BKEYC  (  -- c )   ( TOS STM8: -- A,Z,N )
;       Read and translate board dependent key bitmap into char

BKEYCHAR:
        JRA     BKEY            ; Dummy: get "no key" and leave it as it is

       .endif
