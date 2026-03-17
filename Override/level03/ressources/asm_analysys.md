1) Understanding the main


   0x080488c1 <+103>:	call   0x8048530 <__isoc99_scanf@plt>
   0x080488c6 <+108>:	mov    0x1c(%esp),%eax
   0x080488ca <+112>:	movl   $0x1337d00d,0x4(%esp)
   0x080488d2 <+120>:	mov    %eax,(%esp)
   0x080488d5 <+123>:	call   0x8048747 <test>


scanf reads our number → stored at [esp+0x1c], That value goes into eax

Translate to C:

int input;
scanf("%d", &input);
test(input, 0x1337d00d);

So test gets:

arg1 = our input
arg2 = 0x1337d00d

2)  Start of test()

   0x0804874d <+6>:	    mov    0x8(%ebp),%eax                   eax = arg1 (input)
   0x08048750 <+9>:	    mov    0xc(%ebp),%edx                   edx = arg2 (0x1337d00d)
   0x08048753 <+12>:	mov    %edx,%ecx
   0x08048755 <+14>:	sub    %eax,%ecx                        ecx = arg2 - arg1
   0x08048757 <+16>:	mov    %ecx,%eax
   0x08048759 <+18>:	mov    %eax,-0xc(%ebp)                  store diff

means :   diff = 0x1337d00d - input;

3) The critical condition in test()

    0x0804875c <+21>: cmpl   $0x15,-0xc(%ebp)
    0x08048760 <+25>: ja     0x804884a

0x15 = 21
ja = jump if above (unsigned)

So:

if (diff > 21)
    goto random_path;

3) If diff ≤ 21 → jump table

0x08048766 <+31>: mov    -0xc(%ebp),%eax
0x08048769 <+34>: shl    $0x2,%eax         ; eax = diff * 4
0x0804876c <+37>: add    $0x80489f0,%eax   ; base address of table
0x08048771 <+42>: mov    (%eax),%eax       ; load function address
0x08048773 <+44>: jmp    *%eax             ; jump

Translate:

This is a switch-case:

switch(diff) {
    case 0:
    case 1:
    ...
    case 21:
}

So the program jumps to a specific block depending on diff

4) The “bad” path (when diff > 21)

0x0804884a <+259>: call   rand
0x0804884f <+264>: mov    %eax,(%esp)
0x08048852 <+267>: call   decrypt

Translate:
decrypt(rand());

Random → not controllable → useless

