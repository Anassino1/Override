Level04
-------

Main

Prologue & Stack:
<+0>  push ebp
<+1>  mov  ebp, esp
<+3>  push edi
<+4>  push ebx
<+5>  and  esp, 0xfffffff0
<+8>  sub  esp, 0xb0
Save registers
Align stack
Allocate 0xb0 bytes


Fork Process:

<+14> call fork
<+19> mov [esp+0xac], eax
eax = fork()
0 → child
>0 → parent



Initialize buffer:
<+26> lea ebx, [esp+0x20]
<+44> rep stos
Clear buffer at [esp+0x20] (~128 bytes)


Check fork result:
<+65> cmp [esp+0xac], 0
<+73> jne parent



------CHILD PROCESS:-----

//(Setup protections)
<+90>  call prctl
<+126> call ptrace
prctl → restrict process
ptrace → anti-debugging


Print message:
<+131> mov [esp], 0x8048903
<+138> call puts

User input:
<+143> lea eax, [esp+0x20]
<+150> call gets

⚠️ Vulnerability

gets → no bounds check → buffer overflow



Jump to end:
<+155> jmp end



-------PARENT PROCESS:-------

//(Wait child)
<+161> lea eax, [esp+0x1c]
<+168> call wait
//(Check child status)
<+191> and eax, 0x7f
<+194> test eax, eax
Checks how child exited

If normal case:
<+228> mov [esp], msg
<+235> call puts

Debugging check (ptrace)
<+276> call ptrace
<+281> mov [esp+0xa8], eax
<+288> cmp [esp+0xa8], 0xb
If result == 0xb → suspicious (debugging)

If debugging detected:

<+302> call puts
<+322> mov [esp+0x4], 9
<+332> call kill
Print warning
Kill child process


-----Summary--------
fork() → creates child + parent

Child:
anti-debug (ptrace)
reads input with gets ⚠️ (overflow)

Parent:
monitors child with wait
uses ptrace to detect debugging
may kill child
