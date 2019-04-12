; I386.INC

%define SIZEOF_DESC             8   ;bytes

; Segment descriptor attributes
; G
%define GRANU_BYTE              00000000_00000000b
%define GRANU_4K                10000000_00000000b
; D
%define CODESEG_32              01000000_00000000b
%define CODESEG_16              00000000_00000000b
%define EXPDN_DATASEG_4G        01000000_00000000b
%define EXPDN_DATASEG_64K       00000000_00000000b
%define STACKSEG_32             01000000_00000000b
%define STACKSEG_16             00000000_00000000b
; P
%define PRESENT                 00000000_10000000b
%define NOT_PRESENT             00000000_00000000b
; DPL
%define DPL_0                   00000000_00000000b
%define DPL_1                   00000000_00100000b
%define DPL_2                   00000000_01000000b
%define DPL_3                   00000000_01100000b
; DT
%define DATA_SEG                00000000_00010000b
%define SYSTEM_SEG              00000000_00000000b
; TYPE
%define ACCESSED                00000000_00000001b       ; A
%define NOT_ACCESSED            00000000_00000000b
%define SEG_TYPE_CODE           00000000_00001000b       ; E
%define SEG_TYPE_DATA           00000000_00000000b
%define WRITABLE                00000000_00000010b       ; DATA_SEG (E=0)
%define NOT_WRITABLE            00000000_00000000b
%define EXPAND_DOWN             00000000_00000100b
%define EXPAND_UP               00000000_00000000b
%define READABLE                00000000_00000010b       ; CODE_SEG (E=1)
%define NOT_READABLE            00000000_00000000b
%define CONFORMING              00000000_00000100b
%define NOT_CONFORMING          00000000_00000000b
%define TYPE_LDT                00000000_00000010b       ; System & gate descs
%define TYPE_TSS_AVAIL          00000000_00001001b       ; 286 descs are not
%define TYPE_TSS_BUSY           00000000_00001011b       ; shown here.
%define TYPE_CALL_GATE          00000000_00001100b
%define TYPE_TASK_GATE          00000000_00000101b
%define TYPE_INTR_GATE          00000000_00001110b
%define TYPE_TRAP_GATE          00000000_00001111b

; Selector attributes
; RPL
%define RPL_0                   00000000_00000000b
%define RPL_1                   00000000_00000001b
%define RPL_2                   00000000_00000010b
%define RPL_3                   00000000_00000011b
; TI
%define GDT_SEL                 00000000_00000000b
%define LDT_SEL                 00000000_00000100b

; DESC_STRUCT
struc   DescStruct
        .LimitLW        resw    1
        .BaseLB         resb    1
        .BaseMW         resw    1
        .AttribW        resw    1
        .BaseHB         resb    1
endstruc

; MAKE_DESC             BASE, LIMIT, ATTRIB
%macro  MAKE_DESC       3
        dw      (%2 & 0xFFFF)                           ; Limit[15..0]
        dw      ((%1) & 0xFFFF)                         ; Base[15..0]
        db      ((%1 >> 16) & 0xFF)                     ; Base[23..16]
        dw      (%3 | (((%2 >> 16) << 8) & 0xFFFF))     ; Attrib, Limit[19..16]
        db      ((%1 >> 24) & 0xFF)                     ; Base[31..24]
%endmacro

; MAKE_NULL_DESC
%define MAKE_NULL_DESC  MAKE_DESC 0, 0, 0

; MAKE_GATE_DESC        SELECTOR, OFFSET, ATTRIB, DWORD_COUNT
%macro  MAKE_GATE_DESC  4
        dw      (%2 & 0xFFFF)                           ; Offset[15..0]
        dw      (%1 & 0xFFFF)                           ; Selector
        dw      (((%3 << 8) | (%4 & 0x001F)) & 0xFFFF)  ; Attributes
        dw      ((%2 >> 16) & 0xFFFF)                   ; Offset[31..16]
%endmacro

; MAKE_CALL_GATE        SELECTOR, OFFSET, ATTRIB, DWORD_COUNT
%define MAKE_CALL_GATE(a,b,c,d) MAKE_GATE_DESC a, b, c, d

; MAKE_TASK_GATE        SELECTOR, ATTRIB
%define MAKE_TASK_GATE(a,b)     MAKE_GATE_DESC a, 0, b, 0

; MAKE_INTR_GATE        SELECTOR, OFFSET, ATTRIB
%define MAKE_INTR_GATE(a,b,c)   MAKE_GATE_DESC a, b, c, 0

; MAKE_TRAP_GATE        SELECTOR, OFFSET, ATTRIB
%define MAKE_TRAP_GATE(a,b,c)   MAKE_TRAP_GATE a, b, c, 0

; MAKE_SELECTOR         INDEX, TI, RPL
%macro  MAKE_SELECTOR   3
        dw      (((%1 << 3) & 0xFFF8) | %2 | %3)
%endmacro

; MAKE_SEL_INLINE       INDEX, TI, RPL
%define MAKE_SEL_INLINE(a,b,c)  (((a << 3) & 0xFFF8) | b | c)

; MAKE_GDT              BASE, LIMIT
%macro  MAKE_GDT        2
        dw      %2                              ; Limit
        dd      %1                              ; Base
%endmacro

; GDT_STRUCT
struc   GdtStruct
        .Limit          resw    1
        .Base           resd    1
endstruc

; TSS_STRUCT
struc   TssStruct
        .Backlink       resw    1               ; Link to previous TSS
                        resw    1               ; Dummy
        .Esp0           resd    1
        .Ss0            resw    1
                        resw    1
        .Esp1           resd    1
        .Ss1            resw    1
                        resw    1
        .Esp2           resd    1
        .Ss2            resw    1
                        resw    1
        .Cr3            resd    1
        .Eip            resd    1
        .Eflags         resd    1
        .Eax            resd    1
        .Ecx            resd    1
        .Edx            resd    1
        .Ebx            resd    1
        .Esp            resd    1
        .Ebp            resd    1
        .Esi            resd    1
        .Edi            resd    1
        .Es             resw    1
                        resw    1
        .Cs             resw    1
                        resw    1
        .Ss             resw    1
                        resw    1
        .Ds             resw    1
                        resw    1
        .Fs             resw    1
                        resw    1
        .Gs             resw    1
                        resw    1
        .Ldtr           resw    1
                        resw    1
        .Trap           resw    1
        .IOMapBase      resw    1
endstruc

