; x16asm.s
; 	Copied from x16edit.s and modified by Pete Wyspianski (AKA Gollan petegollan@outlook.com)
;   April 2026 for the X16-Assembler in ROM project

.include "banks.inc"

.export x16asm
.import bajsrfar, frmevl, valtyp, fcerr, frefac, index1, basic_fa

.segment "ANNEX"

.proc x16asm
	beq new ; No input param... 
	jsr frmevl ; Evaluate input
	bit valtyp
	bmi set_file ; Input is a string, try to open file in the assembler

error:
	jmp fcerr ; Input was not a string => error

set_file:
	jsr frefac ; Get string
	beq new ; String is empty
	
	sta $04 ; R1L = file name length
	lda index1
	sta $02 ; R0L = file name address low
	lda index1+1
	sta $03 ; R0H = file name address high
	bra launch

new:
	stz $04 ; R1L = file name length, 0 => no file

; Launch X16-ASSEMBLER
launch:
  	jsr bajsrfar
	.word $c003		; This is the BASIC_vec entry.
	.byte BANK_ASM
	rts

.endproc
