#include <avr/io.h>

; Calling a assembler function from C:
; Registers:
;  r18-r27, r30:r31: free usable
;  r2-r17:           have to be saved
;  r0, r1:           don't use!
;
; Function paramters (read from left to right)
;  r25:r24 – 1st argument
;  r23:r22 – 2nd argument
;  r21:r20 - 3rd argument
;
; Function returncode
;  char:     r24 (r25=0)
;  int:  r25:r24 (r24 = lowbyte, r25 = highbyte)
;  long: r27-r24 (r24 = lowbyte, r27 = highbyte)
;
; see for more details: http://gcc.gnu.org/wiki/avr-gcc#Register_Layout

.text
.global TestFunc

; uint8_t QUE_uPut(uint8_t* puQueue, uint8_t uVal);
; Parameters:
;   r25:r24   puQueue
;   r22       uVal
; Returns:
;   r24       1: byte stored, otherwise 0
TestFunc:   mov     r31, r25    ; load queue pointer into Z
            mov     r30, r24
            mov     r24, r22
            inc     r24
            ret









