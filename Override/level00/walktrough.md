Level00
-------

1) 
After checking we found 

level00@OverRide:~$ ls -la
total 13
dr-xr-x---+ 1 level01 level01   60 Sep 13  2016 .
dr-x--x--x  1 root    root     260 Oct  2  2016 ..
-rw-r--r--  1 level01 level01  220 Sep 10  2016 .bash_logout
lrwxrwxrwx  1 root    root       7 Sep 13  2016 .bash_profile -> .bashrc
-rw-r--r--  1 level00 level00 3533 Sep 10  2016 .bashrc
-rwsr-s---+ 1 level01 users   7280 Sep 10  2016 level00
-rw-r--r--  1 level01 level01  675 Sep 10  2016 .profile

And when we run ./level00 it prints

level00@OverRide:~$ ./level00 
***********************************
* 	     -Level00 -		  *
***********************************
Password:

and we can see that it's waiting for a password. 

2) 

Let's check it with gdb (check ressources folder)

we found inside the main function a cmp between a number and %eax, and the functions it jumps if they are not equal,

The number is 0x149c = 5276 

and that's our password

level00@OverRide:~$ ./level00 
***********************************
* 	     -Level00 -		  *
***********************************
Password:5276

Authenticated!
$ whoami
level01
$ pwd
/home/users/level00
$ cat /home/users/level01
cat: /home/users/level01: Is a directory
$  cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
