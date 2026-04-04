Level03
-------


1) Checking the functions:

by using the command "info functions" inside gdb, we found 3 functions :
main()
test()
decrypt()

2) the main()

* Seeds the random number generator used by rand().

* Reads a number from the user and stores it in a buffer.

* Calls the test function with two arguments:

    The number input by the user (buff)

    The constant 0x1337d00d (decimal 322424845)

       0x080488ca <+112>:	movl   $0x1337d00d,0x4(%esp)


3) test(int input, int value)

Calculates the difference:

diff = value - input;

Behavior based on diff:

if (diff > 21)
    decrypt(rand());   // random number, useless
else
    decrypt(diff);     // controlled value

Only values where diff ≤ 21 are meaningful, because decrypt(diff) is called with predictable numbers.

4) decrypt(int n)

Contains a string: hash = "Q}|u`sfg~sf{}|a";

Each character is XORed with the argument n.

The resulting string is compared to "Congratulations".

If it matches → the shell is executed;

If not → the program exits.

5) How to find the correct input

Only inputs producing diff ≤ 21 are valid.

Therefore, possible inputs are:

322424845 - 21 = 322424824
...
322424845 - 1  = 322424844

By trying all numbers in this range, the only number that produces the correct hash (matching "Congratulations") is the one that makes diff = 18.

input = 322424845 - 18 = 322424827

6) Solution


level03@OverRide:~$ ./level03 
***********************************
*		level03		**
***********************************
Password:322424827
$ whoami
level04
$ pwd
/home/users/level03
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
