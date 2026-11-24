;*******************************************************************************
; assemble.inc
;   Copied from "compile.inc" and modified to call the X16-assembler
;	by Pete Wyspianski (AKA Gollan petegollan@outlook.com)
;   March 2026 for the X16-Assembler in ROM project.
;   Ottawa, Canada
;
; Copyright 2022-2024, Stefan Jakobsson
;
;Redistribution and use in source and binary forms, with or without modification, 
;are permitted provided that the following conditions are met:
;
;1. Redistributions of source code must retain the above copyright notice, this 
;   list of conditions and the following disclaimer.
;
;2. Redistributions in binary form must reproduce the above copyright notice, 
;   this list of conditions and the following disclaimer in the documentation 
;   and/or other materials provided with the distribution.
;
;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” 
;AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
;IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
;DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
;FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
;DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
;SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
;CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
;OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
;OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;*******************************************************************************

;******************************************************************************
;Function name.......: x16_assembler
;Purpose.............: Assembles the current file with X16-assembler
;Input...............: Nothing
;Returns.............: Nothing
;Error returns.......: None
;In ROM bank.........: 2nd
.segment "CODE2"
.proc x16_assembler
    ;Clear status message
    call_firstbank screen_clear_status

    ;Check file name existence
    ldx file_cur_filename_len
    bne :+                      ; Taken if name exists (length is non-zero)

    ; Show no file error message and exit asm tool
    lda #2
    sta APP_MOD
    
    call_firstbank screen_print_default_footer
    ldx #<assemble_msg_nofile
    ldy #>assemble_msg_nofile
    call_firstbank screen_print_status
    call_firstbank cursor_activate
    rts

    ; Check that the file is saved (added by Gollan)
:
    lda mem_modified
    beq :+                  ; Taken if NOT modified (flag is zero/false)
    
    ; Show file no saved error message and exit asm tool
    lda #2
    sta APP_MOD

    call_firstbank screen_print_default_footer
    ldx #<assemble_msg_notsaved
    ldy #>assemble_msg_notsaved
    call_firstbank screen_print_status
    call_firstbank cursor_activate
    rts

    ;Find x16-assembler
:   bridge_setaddr KERNAL_FETCH
    ldx #0          ; This is the ROM bank number
    lda #<$c00c     ; Assembler signature is in the low part of the ROM image
    sta TMP1_ADR
    lda #>$c00c
    sta TMP1_ADR+1
loop:
    ldy #0      ; Index into signature string
loop2:
    lda #TMP1_ADR
    phx
    bridge_call KERNAL_FETCH
    plx
    cmp assemble_signature,y
    bne next
    iny
    cpy #assemble_signature_end-assemble_signature
    bcc loop2
    bra found
next:
    inx
    bne loop
    
    lda #2
    sta APP_MOD
    
    call_firstbank screen_print_default_footer
    ldx #<msg_noassembler
    ldy #>msg_noassembler
    call_firstbank screen_print_status
    call_firstbank cursor_activate
    rts

found:

    ;Print loading message
    ldx #<assemble_msg_loading
    ldy #>assemble_msg_loading
    call_firstbank screen_print_status

    ;Copy file name (only the characters; string is NOT null terminalted
    stz BNK_SEL
    ldx file_cur_filename_len   ; *** bug in the original compile.inc
:   lda file_cur_filename-1,x
    sta $bf00-1,x
    dex
    bne :-

    ;Set file len and device params
    lda file_cur_filename_len
    sta r0
    lda file_cur_device
    sta r0+1

    ;Run x16-assembler
    sei
    lda #$10        ; *** BANK $10/16 for the final build
;    lda #$20         ; *** BANK $20/32 for debugging with ROAM cartridge
    sta jsrfar_bank
    stz jsrfar_addr
    lda #$c0
    sta jsrfar_addr+1
    jsr jsrfar2
    cli

    ;Restore footer
    call_firstbank screen_print_default_footer

    ;Show response message
    call_firstbank screen_clear_status
    stz BNK_SEL
    ldx #$00
    ldy #$bf
    call_firstbank screen_print_status

    ;On error or warning, goto line
    lda r1
    beq :+
    ldx r1+1
    ldy r2
    lda r2+1
    call_firstbank cmd_goto_line
    call_firstbank screen_refresh

    ;Clean up
:   lda #2
    sta APP_MOD
    call_firstbank cursor_activate
    rts

assemble_bank = tempvars

assemble_signature:
    .byt "x16-assembler"
assemble_signature_end:

.CODE
assemble_msg_loading:
    .byt "assembling...",0

assemble_msg_nofile:
    .byt "no file to assemble",0

assemble_msg_notsaved:
    .byt "file not saved",0

msg_noassembler:
    .byt "x16-assembler not found",0
.endproc