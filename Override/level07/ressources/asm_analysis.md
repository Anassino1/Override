--MAIN:--


Prologue & Stack:
<+0>  push ebp
<+6>  and esp, 0xfffffff0
<+9>  sub esp, 0x1d0
Large stack (~464 bytes)
Stack canary enabled

Init Array:
<+110> lea ebx, [esp+0x24]
<+128> rep stos
Zero buffer (array)

Wipe argv/env:
<+132> call memset
<+199> loop over argv
<+211> loop over envp
Clears arguments from memory

Command Loop:
<+351> call fgets
Read user command into buffer

Command Check:
store:
<+427> repz cmpsb  ; "store"
<+455> call store_number
read:
<+492> repz cmpsb  ; "read"
<+520> call read_number
quit:
<+557> repz cmpsb  ; "quit"
Loop Continues:
<+679> jmp main_loop


--READ_NUMBER--


Get Index:
<+26> call get_unum
Read Value:
<+37> shl eax, 2
<+40> add eax, array
<+43> mov edx, [eax]
⚠️ No bounds check

--STORE_NUMBER--

Get Inputs:
<+33> call get_unum   ; value
<+54> call get_unum   ; index
Restricted Index Check:
<+86> test edx, edx
<+88> je fail
Blocks some indexes (multiples of 3)
Special Value Check:
<+93> shr eax, 0x18
<+96> cmp eax, 0xb7
Blocks values starting with 0xb7
Store Value:
<+149> shl eax, 2
<+152> add eax, array
<+158> mov [eax], edx


---Summary---

Behavior:
-Interactive program:
    store → save number
    read  → read number
    quit  → exit
-Uses internal array