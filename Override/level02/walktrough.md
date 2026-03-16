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

We can say that the program calls : fopen("/home/users/level03/.pass", "r")



   0x00000000004008ad <+153>:	mov    %rax,-0x8(%rbp)

The result of fopen() is stored at: rbp - 0x8