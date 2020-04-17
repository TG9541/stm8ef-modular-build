# An STM8 eForth Demo Board Configuration


When designing an STM8 board it's a good practive to keep PD1 in input state most of the time (in theory that shouldn't the timing of applying `NRST` can be tricky).

Also keep in mind that different GPIO groups not only have speacial features but also different electrical properties, e.g.

* port A has limited sink/source capabilities
* port B4 & B5 are "true open drain"

In an STM8 eForth board configuration folder, initialization of GPIO settings often happens in the `BOARDINIT` section in `boardcore.inc`. 

In this demo, most GPIOs are used for driving a 4-digit LED display:

```
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
```

It's certainly not a good practice but it's not always necessary to use segment resistors for LEDs since the GPIOs limit the current to 20mA anyway. 

A test breadboard may look like this:

![20200411_120506~2](https://user-images.githubusercontent.com/5466977/79041233-6f6bf380-7bee-11ea-8f6f-7c69a55cce32.jpg)

Here is the mapping routine. Note the offset in the `LED7LAST` indexed addressing:


```
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
```
