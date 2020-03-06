An underhanded programming problem.
M.Bruestle 22jan2012

Declaration of intention: The objective of the program should be that 
I send 32bit values to a transputer on its boot link and get an 
incremented value back on the same link. Overflows don’t care. 

Let’s unravel the mystery! 

The obvious bug is the IN instruction against the BOOT_CHAN_OUT. But 
the unexpected effect is: There is NO PROBLEM at all – this program 
works as intended! 

Against a hard channel IN and OUT are interchangeable. So you can 
compile this sample with IN/OUT, IN/IN, OUT/IN and OUT/OUT. All 
combinations are well-functioning. 

Why? The answer is simple: The hard channel itself determines the 
direction. The IN or OUT instruction only contribute the pointer and 
the length. 

So you might ask: Why there are two instructions? They are required 
for the internal channels. In that case the process which synchronizes 
later determines the direction and dictates the length. The direction 
and length of the first ready process is of no relevance. 

This behavior also includes outword and outbyte. 

repeat_loop 

    ldl  BOOT_CHAN_IN 
    ldc  0 
    outword              ; read parameter 
    ldl  BOOT_CHAN_OUT 
    ldl  0 
    ldc  1 
    bsub                 ; compute 
    outword              ; send result 

    j @repeat_loop 
    
> ‘outword’ instruction sends the data (i.e. 4 bytes) in AReg using the channel 
> in Breg, 

That is correct, but in fact the transputer saves the data word in 
Areg at pw.temp and transfers this address (Wptr) to the link engine. 
So the "received" word will end up in Wptr[0] and not in Areg. The 
link engine can only access memory. 

There is no practical usage to execute an outword against an in 
channel. You saves one "ldc 4" for the costs of one memory reference. 
Not a good deal! 

The same behavior applies to outbyte. I don't know if the values of 
the upper bytes of Wptr[0] are affected - but this counts for its 
regular use also. 
    


; 
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

repeat_loop 

    ; read parameter 

    ldlp BUFFER 
    ldl  BOOT_CHAN_IN 
    ldc  1 
    bcnt 
    in 

    ; compute 

    ldl  BUFFER 
    ldc  1 
    sum 
    stl  BUFFER 

    ; send result 

    ldlp BUFFER 
    ldl  BOOT_CHAN_OUT 
    ldc  1 
    bcnt 
    in 

    j @repeat_loop 

    .end 

; -=EOF=- 