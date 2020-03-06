; M.Bruestle 15feb2012
;
; Today I’ve written a little assembler program to confirm my assumption 
; about the following (wrong) sentence in the ACWG (p 86): 

; "When an communication instruction is executed the value of Wdesc, the 
; pointer to the data and the message length are copied into registers 
; in the link hardware, and the process is descheduled. The value of 
; Wdesc is also copied into the relevant link channel control word - but 
; this is to aid analysing only." 


.t414 
    .pub _main 

#define BOOT_CHAN_IN  1 
#define BOOT_CHAN_OUT 2 

#define BUFFER        3 


_main 

    ; make room for special workspace locations 

    ajw  5 

    ; get input bootstrap channel address (held in register "C" on 
entry). 

    diff 
    stl  BOOT_CHAN_IN 
    stl  BOOT_CHAN_IN           ; save boot input link address 

    ; compute corresponding output channel address. 

    ldc  4 
    bcnt                        ; Areg will be 8 for T2, 16 for T4/T8 
    ldl  BOOT_CHAN_IN 
    xor 
    stl  BOOT_CHAN_OUT          ; compute boot output link address 

    ; init high and low priority front of queue register 

    mint 
    sthf 
    mint 
    stlf 

    ; init errorflag and haltonerrorflag 

    clrhalterr 
    testerr 

    ; --- test --- 

    ; init buffer data 

    ldc  0xADDE 
    stl  BUFFER 

    ; prepare process 3 (set PW_IPTR) 

    ldc  P3-P0 
    ldpi 
P0: 
    ldlp 32-1 
    stnl 0 

    ; start process 2 

    ldc  P2-P1 
    ldlp 16 
    startp 

P1: ;------------------------------------------------------- 

    ldlp BUFFER 
    mint 
    ldnlp 5 ; link1in 
    ldc  1 
    in 

    ; four values from P1 

    ldl  BOOT_CHAN_OUT 
    ldc  0x1 
    outword 
    ldl  BOOT_CHAN_OUT 
    ldlp 0 
    outword 
    ldl  BOOT_CHAN_OUT 
    ldpri 
    outword 
    ldl  BOOT_CHAN_OUT 
    ldl  BUFFER 
    outword 

    start 

P2: ;------------------------------------------------------- 

    ; three values from P2 

    ldl  BOOT_CHAN_OUT-16 
    ldc  0x22 
    outword 
    ldl  BOOT_CHAN_OUT-16 
    ldlp 0 
    outword 
    ldl  BOOT_CHAN_OUT-16 
    ldpri 
    outword 

    ; replace channel control word of the hardlink 

    mint 
    ldnl   5 ; link1in wdesc P1 (Prio1) 
    ldnlp 32 
    ldc    1 
    xor 
    mint 
    stnl   5 ; link1in wdesc P3 (Prio0) 

    ; final value from P2 

    ldl  BOOT_CHAN_OUT-16 
    ldc  7 
    outword 

    ; send one byte to P1 which is now P3 

    mint 
    ldnlp 1 ; link1out 
    ldc  0xEE 
    outbyte 

    stopp 

P3: ;------------------------------------------------------- 

    ; four values from P3 

    ldl  BOOT_CHAN_OUT-32 
    ldc  0x333 
    outword 
    ldl  BOOT_CHAN_OUT-32 
    ldlp 0 
    outword 
    ldl  BOOT_CHAN_OUT-32 
    ldpri 
    outword 
    ldl  BOOT_CHAN_OUT-32 
    ldl  BUFFER-32 
    outword 

    start 

    .end 

; -=EOF=- 