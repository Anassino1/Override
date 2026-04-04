In the main:


Prologue & Stack:

<+0>  push ebp
<+1>  mov  ebp, esp
<+3>  push edi
<+4>  push ebx
<+5>  and  esp, 0xfffffff0
<+8>  sub  esp, 0x90
Save registers
Align stack (16-byte)
Allocate 0x90 bytes (~144 bytes)


Initialize variable:
<+14> mov [esp+0x8c], 0x0
Counter / index = 0


Read input (SAFE):
<+25> mov eax, ds:0x80497f0   ; FILE* (stdin)
<+30> mov [esp+0x8], eax
<+34> mov [esp+0x4], 0x64     ; size = 100
<+42> lea eax, [esp+0x28]     ; buffer
<+49> call fgets
Reads input into buffer [esp+0x28]
Max size = 100 bytes
Uses fgets → safe (bounded input)


Reset counter:
<+54> mov [esp+0x8c], 0x0

LOOP START:
<+65> jmp loop_check

--Character Processing Loop--

Load current character:
<+67> lea eax, [esp+0x28]
<+71> add eax, [esp+0x8c]
<+78> movzx eax, BYTE PTR [eax]
Access buffer[index]

Check if >= 'A':
<+81> cmp al, 0x40   ; '@'
<+83> jle skip

Check if <= 'Z':
<+99> cmp al, 0x5a   ; 'Z'
<+101> jg skip

If uppercase → transform:
<+117> mov edx, eax
<+119> xor edx, 0x20
xor 0x20 → flips case
Converts:
'A' → 'a'
'Z' → 'z'

Write back:
<+133> mov BYTE PTR [eax], dl
Increment index:
<+135> add [esp+0x8c], 1
Loop Condition (strlen-like):
Compute string length:
<+173> repnz scas al, BYTE PTR [edi]
Equivalent to:
strlen(buffer)
Compare index vs length:
<+184> cmp ebx, eax
<+186> jb loop_start
Loop continues while:
index < strlen(buffer)

Print result:
<+188> lea eax, [esp+0x28]
<+195> call printf
⚠️ Format string vulnerability
Input is used directly as format string
No " %s "
Exit:
<+200> mov [esp], 0
<+207> call exit

⚠️ Vulnerabilities
Format String Vulnerability
printf(buffer)
User controls format string
Can exploit with:
%x %x %x
%n