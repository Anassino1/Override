Level06
-------

1) After checking the program with gdb we found that it has 2 functions (main() & auth())

(gdb) info functions

0x08048748  auth
0x08048879  main

2) main()

   0x08048941 <+200>:	call   0x8048748 <auth>
   0x08048946 <+205>:	test   %eax,%eax
   0x08048948 <+207>:	jne    0x8048969 <main+240>

If auth() returns 0, we go here:

0x0804894a <+209>: movl   $0x8048b52,(%esp)
0x08048951 <+216>: call   puts
0x08048956 <+221>: movl   $0x8048b61,(%esp)
0x0804895d <+228>: call   system   ; /bin/sh

So:

auth(login, serial) == 0 → shell

3) What we want to do ?


if (auth(login, serial) == 0)
    system("/bin/sh");

So your objective is:

Make auth() return 0

4) what auth() does ?

key = hash(login)

if (serial == key)
    return 0;
else
    return 1;

serial must equal computed key


5) let's use gdb

level06@OverRide:~$ gdb ./level06

6) Set breakpoints 

(gdb) b *0x080487b5                         Before ptrace
Breakpoint 1 at 0x80487b5
(gdb) b *0x08048866                         Before final comparison
Breakpoint 2 at 0x8048866

Why ????
--------

Breakpoint 1:
-------------

   0x080487b5 <+109>:	call   0x80485f0 <ptrace@plt>


This is:

if (ptrace(...) == -1)
    exit;

Anti-debug protection

Breakpoint 2:
------------

   0x08048866 <+286>:	cmp    -0x10(%ebp),%eax


This compares: serial == key

And: key is stored at -0x10(%ebp)


AFTER the ptrace check
----------------------

   0x080487ed <+165>:	mov    0x8(%ebp),%eax

SAFE zone (after anti-debug)


7) The solution

(gdb) b *0x080487b5
Breakpoint 1 at 0x80487b5
(gdb) b *0x08048866
Breakpoint 2 at 0x8048866


(gdb) run
Starting program: /home/users/level06/level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: bonjour
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 0

Breakpoint 1, 0x080487b5 in auth ()



(gdb) jump *0x080487ed
Continuing at 0x80487ed.

Breakpoint 2, 0x08048866 in auth ()
(gdb) p *(int*)($ebp-0x10)
$1 = 6233767
(gdb) q
A debugging session is active.

	Inferior 1 [process 1887] will be killed.

Quit anyway? (y or n) y





level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: bonjour
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233767
Authenticated!
$ pwd
/home/users/level06
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8

