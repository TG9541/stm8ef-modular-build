# An STM8 eForth Demo Board Configuration

This STM8 eForth board configuration folder contains all that's neccessary for building a configured STM8 eForth binary including I/O device drivers and any Forth code needed for bringing it up.

## Description of Configuration Files
The following files are included in the board configuration folder:

File|Description
-|-
README.md|this file
board.fs|board support Forth code (after building the core)
boardcore.inc|STM8 assembler routines, e.g. initialization and I/O drivers
globconf.inc|STM8 eForth build options
target.inc|type and parameters of the STM8 device used for the board 

The parent folder contains a `Makefile` that fetches a STM8 eForth release archive and then uses the contents of this folder to build a configured STM8 eForth. Please refer to the parent folder for details.


### The file `board.fs`

The file `board.fs` may contain board support code written in Forth, or unit tests, or both. STM8 eForth compiles to machine code and it can be used to write [interrupt routines](https://github.com/TG9541/stm8ef/wiki/STM8-eForth-Interrupts). 

The `Makefile` assumes that `board.fs` exists, but it can be empty. 

Driver or initialization code should be pretected by using `PERSIST` so that it will be the baseline for `RESET`:

```Forth
#require PERSIST
NVM
\ anything defined here will be in releases!

: start
  ."Look Ma, no hands!"
;

' start 'BOOT !
RAM
PERSIST  \ reset won't remove the code above 
```

An example for more complex `board.fs` files is the one of [XH-M194](https://github.com/TG9541/stm8ef/blob/master/XH-M194/board.fs) which contains an DS1302 RTC driver. It's also possible to code complete applications like the [C0135 configuration](https://github.com/TG9541/stm8ef-modbus/tree/master/C0135) in STM8 eForth MODBUS.


### The file `boardcore.inc`

The STM8 eForth core includes the file `boardcore.inc` which should be provided in the board folder. Normally, this file contains basic hardware initialization 

The STM8 eForth core contains machine code for [board char I/O](https://github.com/TG9541/stm8ef/wiki/STM8-eForth-Board-Character-IO), and sometimes it makes sense to use these instead of writing Forth code.

The file [CORE/boardcore.inc](https://github.com/TG9541/stm8ef/blob/master/CORE/boardcore.inc) contains all necessary extension points but in the minimal configuration it only adds one `RET` to the code.

### The file `globconf.inc`

The STM8 eForth core is configured by conditionals. Available options and defaults are listed in [defconf.inc](https://github.com/TG9541/stm8ef/blob/master/inc/defconf.inc). Depending on needs and space constraints configurations can be [very lean](https://github.com/TG9541/stm8ef/tree/master/CORE) or rather [full-featured](https://github.com/TG9541/stm8ef/blob/master/MINDEV/globconf.inc).

### The file `target.inc`

Characteristics of STM8 devices (e.g. STM8S or STM8L core, available RAM, addresses of peripherals, etc) should be defined in `target.inc`. The STM8 eForth repository contains examples for different devices.

## Designing a Board
When designing an STM8 board it's a good practive to keep PD1 in input state most of the time (in theory that shouldn't the timing of applying `NRST` can be tricky).

Also keep in mind that different GPIO groups not only have speacial features but also different electrical properties, e.g.

* port A has limited sink/source capabilities
* port B4 & B5 are "true open drain"

In an STM8 eForth board configuration folder, initialization of GPIO settings often happens in the `BOARDINIT` section in `boardcore.inc`. 

A test breadboard may look like this:

![20200411_120506~2](https://user-images.githubusercontent.com/5466977/79041233-6f6bf380-7bee-11ea-8f6f-7c69a55cce32.jpg)

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
