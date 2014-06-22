#include <avr/io.h>

; Calling a assembler function from C:
; Registers:
;  r18-r27, r30:r31: free usable
;  r2-r17:           have to be saved
;  r0, r1:           don't use!
;
; Function paramters (read from left to right)
;  r25:r24 - 1st argument
;  r23:r22 - 2nd argument
;  r21:r20 - 3rd argument
;
; Function returncode
;  char:     r24 (r25=0)
;  int:  r25:r24 (r24 = lowbyte, r25 = highbyte)
;  long: r27-r24 (r24 = lowbyte, r27 = highbyte)
;
; see for more details: http://gcc.gnu.org/wiki/avr-gcc#Register_Layout

.text
.global que_init, que_put, que_get

; void QUE_vInit(uint8_t* puQueue, uint8_t uSize);
; Parameters:
;   r25:r24   puQueue
;   r22       uSize
; Returns:
;   nothing
que_init:   mov     r31, r25    ; load queue pointer into Z
            mov     r30, r24
            mov     r18, r22
            subi    r18, 3      ; subtract queue header size
            st      Z, r18      ; store size
            clr     r18
            std     Z+1, r18    ; reset writeindex
            std     Z+2, r18    ; reset readindex
            ret


; uint8_t QUE_uPut(uint8_t* puQueue, uint8_t uVal);
; Parameters:
;   r25:r24   puQueue
;   r22       uVal
; Returns:
;   r24       1: byte stored, otherwise 0
que_put:    mov     r31, r25    ; load queue pointer into Z
            mov     r30, r24
            clr     r24         ; returncode = 0
            ld      r18, Z      ; r18 = size of queue
            ; save SREG and disable interrupts
            ;in      r20, _SFR_IO_ADDR(SREG) ;
            ;cli                 ;
            ; load writeindex into r19, increase it and adjust it to queue size.
            ldd     r19, Z+1    ; r19 = writeindex
            inc     r19
            cp      r18, r19    ; writeindex = size of the queue
            brne    QuPu1
            clr     r19
            ; check if queue is full
QuPu1:      ldd     r18, Z+2    ; r18 = readindex
            cp      r18, r19    ; queue full?
            breq    QuPuF
            ; save uVal to queue
            ldd     r18, Z+1    ; r18 = old writeindex
            std     Z+1, r19    ; store new writeindex
            add     r30, r18    ; load pointer to queue element...
            adc     r31, r24    ; add carry bit. r24 is 0
            adiw    r30, 3      ; ... and skip queue header
            st      Z, r22      ; store uVal

            inc     r24         ; returncode = 1
QuPuF:      ;out     _SFR_IO_ADDR(SREG), R20   ; restore SREG
            ret


; uint16_t QUE_uGet(uint8_t* puQueue);
; Parameters:
;   r25:r24   puQueue
; Returns:
;   r25       1: byte read, otherwise 0
;   r24       byte which has been read, otherwise the content is undefined
que_get:    mov     r31, r25    ; load queue pointer into Z
            mov     r30, r24
            clr     r25
            ; save SREG and disable interrupts
            ;in r20, _SFR_IO_ADDR(SREG)
            ;cli
            ; get queue readindex and queue writeindex
            ldd     r18, Z+1    ; r18 = writeindex
            ldd     r19, Z+2    ; r19 = readindex
            ; queue empty?
            cp      r18, r19    ;readindex == writeindex?
            breq    QuGeE
            ; get element from queue
            mov     r20, r30
            mov     r21, r31
            add     r30, r19    ; load pointer to queue element...
            adc     r31, r25    ; add carry bit. r25 is 0
            adiw    r30, 3      ; ...and skip queue header
            ld      r24, Z
            mov     r30, r20    ; restore r30
            mov     r31, r21    ; restore r31
            ; increase readindex and reset it to the beginning of the queue if
            ; it runs out of the queues end.
            ld      r18, Z      ; r18 = size of the queue
            inc     r19         ; increase readindex
            cp      r18, r19    ; readindex = queue-size?
            brne    QuGe1
            clr     r19
QuGe1:      std     Z+2, r19    ; store new readindex
            ; set flag, that a byte has been taken from the queue
            inc     r25         ; returncode = 1
QuGeE:      ;out     _SFR_IO_ADDR(SREG), r20    ; restore SREG
            ret
