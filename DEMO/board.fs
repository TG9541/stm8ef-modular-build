\ DEMO board Forth code

#require hi

\ get Ain4 from PD3 and print it
: show ( -- )
  4 ADC! ADC@ .
;

\ run show as the background task
: init ( -- )
  [ ' show ] LITERAL BG ! 
  hi
;
