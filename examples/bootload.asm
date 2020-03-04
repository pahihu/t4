--
-- (c) Inmos 1989
--
-- Assembly file for the Generic Primary bootstrap TA HALT mode
--
--
--
-- VAL  BASE            IS  1 :         -- loop index
-- VAL  COUNT           IS  2 :         -- loop count
--
-- VAL  LOAD_START      IS  0 :         -- start of loader
-- VAL  LOAD_LENGTH     IS  1 :         -- loader block length
-- VAL  NEXT_ADDRESS    IS  2 :         -- start of next block to load
-- VAL  BOOTLINK        IS  3 :         -- link booted from
-- VAL  NEXT_WPTR       IS  4 :         -- work space of loaded code
-- VAL  RETURN_ADDRESS  IS  5 :         -- return address from loader
-- VAL  TEMP_WORKSPACE  IS  RETURN_ADDRESS : -- workspace used by both
--                                      -- preamble and loader
-- VAL  NOTPROCESS      IS  6 :         -- copy of MinInt
-- VAL  LINKS           IS  NOTPROCESS : -- 1st param to loader (MinInt)
-- VAL  BOOTLINK_IN_PARAM  IS  7 :      -- 2nd parameter to loader
-- VAL  BOOTLINK_OUT_PARAM IS  8 :      -- 3nd parameter to loader
-- VAL  MEMORY          IS  9 :         -- 4th parameter to loader
-- VAL  EXTERNAL_ADDRESS IS 10 :        -- 5th parameter to loader
-- VAL  ENTRY_POINT     IS 11 :         -- 6th parameter to loader
-- VAL  DATA_POINT      IS 12 :         -- 7th parameter to loader
-- VAL  ENTRY_ADDRESS   IS 13 :         -- referenced from entry point
-- VAL  DATA_ADDRESS    IS 14 :         -- referenced from data point
-- VAL  MEMSTART        IS 15 :         -- start of boot part 2
--
--
-- The initial workspace requirement is found by reading the workspace
-- requirement from the loader \occam\ and substracting the size of the workspace
-- used by both the loader and the bootstrap (\verb|temp.workspace|). This value
-- is incremented by 4 to accomodate the workspace adjustment by the call
-- instruction used to preserve the processor registers.
--
-- initial.adjustment := (loader.workspace + 4) - temp.workspace
-- occam work space, + 4 for call to save registers, - adjustment made
-- when entering occam. Must be at least 4
-- IF
--   initial.adjustment < 4
--     initial.adjustment := 4
--   TRUE
--     SKIP
--
-- set up work space, save registers,
-- save MemStart and NotProcess

    align

    byte    (Endprimary-Primary)    -- Length of the primary bootstrap

Primary:

global Primary

    ajw     INITIAL_ADJUSTMENT  -- see above (is 20)
    call    0                   -- save register

    ldc     _Start - Addr0      -- distance to start byte
    ldpi                        -- address of start
Addr0:
    stl     MEMSTART            -- save for later use

    mint
    stl     NOTPROCESS          -- save for later use

-- initialise process queues and clear error
    ldl     NOTPROCESS
    stlf                        -- reset low priority queue

    ldl     NOTPROCESS
    sthf                        -- reset high priority queue

-- use clrhalterr here to create bootstrap for REDUCED application

    sethalterr                  -- set halt on error
    testerr                     -- read and clear error bit

-- initialise T8 error and rounding
    ldl     MEMSTART            -- Check if processor has floating point unit by
    ldl     NOTPROCESS          --   checking if (memstart >< mint) >= #70
    xor
    ldc     #70                 -- Memstart for T5, T8
    rev                         -- B = #70, A = (MemStart >< MINT)
    gt
    eqc     0
    cj      Nofpu

    fptesterr                   -- floating check and clear error instruction

Nofpu:

-- initialise link and event words
    ldc     0
    stl     BASE                -- index to words to initialise
    ldc     11                  -- no. words to initialise
    stl     COUNT               -- count of words left
Startloop:
    ldl     NOTPROCESS
    ldl     BASE                -- index
    ldl     NOTPROCESS
    wsub                        -- point to next address
    stnl    0                   -- put NotProcess into addressed word
    ldlp    BASE                -- address of loop control info
    ldc     Endloop - Startloop -- return jump
    lend                        -- go back if more
Endloop:

-- set up some loader parameters. See the parameter
-- structure of the loader
    ldl     MEMSTART            -- clear data and entry addresses
    stl     DATA_ADDRESS
    ldl     MEMSTART
    stl     ENTRY_ADDRESS

    ldlp    DATA_ADDRESS        -- address of entry word
    stl     DATA_POINT          -- store in param 7

    ldlp    ENTRY_ADDRESS       -- address of entry word
    stl     ENTRY_POINT         -- store in param 6

    ldl     NOT_PROCESS
    stl     EXTERNAL_ADDRESS    -- buffer offset in param 5

    ldl     MEMSTART            -- start of memory
    stl     MEMORY              -- store in param 4

    ldl     BOOTLINK            -- copy of bootlink
    stl     BOOTLINK_IN_PARAM   -- store in param 2

-- Now find the corresponding output link and place in the parameter

    ldl     BOOTLINK
    ldnlp   -4                  -- Calculate the output link address
    stl     BOOTLINK_OUT_PARAM  -- store in param 3

-- load bootloader over bootstrap
-- code must be 2 bytes shorter than bootstrap
    ldlp    LOAD_LENGTH         -- packet size word
    ldl     BOOTLINK            -- address of link
    ldc     1                   -- bytes to load
    in                          -- input length byte

    ldl     MEMSTART            -- area to load bootloader
    ldl     BOOTLINK            -- address of link
    ldl     LOAD_LENGTH         -- message length
    in                          -- input bootloader

-- enter code just loaded

    pfix    0                   -- For the next bootstrap to be 2 bytes bigger
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0
    pfix    0

    ldl     MEMSTART            -- start of loaded code
    gcall                       -- enter bootloader

    align

Endprimary:

--
-- (c) Inmos 1989
-- Assembly file for the generic secondary loader TA IGNORE mode
--
--
--
-- VAL  BASE            IS  1 :         -- loop index
-- VAL  COUNT           IS  2 :         -- loop count
--
-- VAL  LOAD_START      IS  0 :         -- start of loader
-- VAL  LOAD_LENGTH     IS  1 :         -- loader block length
-- VAL  NEXT_ADDRESS    IS  2 :         -- start of next block to load
-- VAL  BOOTLINK        IS  3 :         -- link booted from
-- VAL  NEXT_WPTR       IS  4 :         -- work space of loaded code
-- VAL  RETURN_ADDRESS  IS  5 :         -- return address from loader
-- VAL  TEMP_WORKSPACE  IS  RETURN_ADDRESS : -- workspace used by both
--                                      -- preamble and loader
-- VAL  NOTPROCESS      IS  6 :         -- copy of MinInt
-- VAL  LINKS           IS  NOTPROCESS : -- 1st param to loader (MinInt)
-- VAL  BOOTLINK_IN_PARAM  IS  7 :      -- 2nd parameter to loader
-- VAL  BOOTLINK_OUT_PARAM IS  8 :      -- 3nd parameter to loader
-- VAL  MEMORY          IS  9 :         -- 4th parameter to loader
-- VAL  BUFFER          IS 10 :         -- 5
-- VAL  NEXT_POINT      IS 11 :         -- 6th parameter to loader
-- VAL  ENTRY_POINT     IS 12 :         -- 7th parameter to loader
-- VAL  DATA_POINT      IS 13 :         -- 8th parameter to loader
-- VAL  ENTRY_ADDRESS   IS 14 :         -- referenced from entry point
-- VAL  DATA_ADDRESS    IS 15 :         -- referenced from Data point
-- VAL  NEXT_ADDRESS    IS 16 :         -- referenced from Nextat point
-- VAL  MEMSTART        IS 17 :         -- start of boot part 2
--
--
-- VAL  PACKET_LENGTH   IS 120 :
-- VAL  OCCAM_WORKSPACE IS 18 :

    byte    (Endsecondary-Secondary)    -- Length of the secondary bootstrap

Secondary:

global Secondary

-- initialise bootloader workspace

    ldc     PACKET_LENGTH   -- buffer size
    ldlp    MEMSTART+1      -- buffer start address
    bsub                    -- end of buffer address
    stl     NEXT_ADDRESS    -- start of area to load loader

    ldl     NEXT_ADDRESS

    ldlp    MEMSTART+1      -- buffer start address
    stl     MEMORY          -- Earliest place to load

    ldlp    TEMP_WORKSPACE  -- pointer to loader's work space zero
    stl     NEXT_WPTR       -- work space pointer of loaded code

    ldc     0
    stl     BUFFER          -- Buffer offset from Buffer start

    ldc     0
    stl     LOAD_LENGTH     -- clear bytes to load

Loadcode:
    ldl     NEXT_ADDRESS    -- address to load loader
    stl     LOAD_START      -- current load point
    
-- load code until terminator
Startload:
    ldlp    LOAD_LENGTH     -- packet length
    ldl     BOOTLINK        -- address of link
    ldc     1               -- bytes to load
    in                      -- input length byte

    ldl     LOAD_LENGTH     -- message length
    cj      Endload         -- quit if 0 bytes

    ldl     NEXT_ADDRESS    -- start of area to load loader
    ldl     BOOTLINK        -- address of link
    ldl     LOAD_LENGTH     -- message length
    in                      -- input code block
    ldl     LOAD_LENGTH     -- message length
    ldl     NEXT_ADDRESS    -- area to load
    bsub                    -- new area to load
    stl     NEXT_ADDRESS    -- save area to load

    j       Startload       -- go back for next block
Endload:

-- initialise return address and enter loaded code
    ldc     Return - Addr1  -- offset to return address
    ldpi                    -- return address
Addr1:
    stl     RETURN_ADDRESS  -- save in W0

    ldl     BOOTLINK        -- Get bootlink and save for later
    stl     OCCAM_WORKSPACE -- Save in area that will not be used
                            -- by network loader

    ldl     NEXT_WPTR       -- wspace of loaded code
    gajw                    -- set up his work space
    ldnl    LOAD_START      -- address of first load packet
    gcall                   -- enter loaded code

Return:

-- Now set up invocation stack for the Init_system

    ajw     (TEMP_WORKSPACE + 4)-- reset work space after return

    ldl     OCCAM_WORKSPACE -- get back boot link
    stl     BOOTLINK

    ldl     DATA_ADDRESS    -- get address of processor structure
    ldl     MEMORY
    bsub
    stl     DATA_POINT

    ldl     ENTRY_ADDRESS   -- convert to real entry address
    ldl     MEMORY
    bsub
    stl     LOAD_START

    ldl     NOTPROCESS
    stl     NEXT_POINT

    ldl     MEMORY          -- make DATA base offset and CODE base offset the same
    stl     BUFFER          --

    ldl     ENTRY_ADDRESS   --
    stl     TEMP_WORKSPACE  -- Set up entry point

    ldl     NEXT_ADDRESS    -- convert returned address of next sequence
    ldl     MEMORY          -- a real address
    bsub
    stl     NEXT_ADDRESS

    ldc     0
    stl     LOAD_LENGTH     -- clear bytes to load

    ldlp    NOT_PROCESS     -- Top of temp workspace used by bootloader
    stl     NEXT_WPTR

-- start clock

    ldc     0
    sttimer

    j       Startload       -- Go back for more and over write the network loader

    align

Endsecondary:
