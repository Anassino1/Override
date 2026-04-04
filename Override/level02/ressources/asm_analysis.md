Main

Prologue & Stack:

<+0>  push rbp
<+1>  mov  rbp, rsp
<+4>  sub  rsp, 0x120
Save old base pointer
Create stack frame
Allocate 0x120 bytes


Save arguments:

<+11> mov [rbp-0x114], edi
<+17> mov [rbp-0x120], rsi
Save argc and argv


Initialize buffers:

<+24> lea rdx, [rbp-0x70]
<+41> rep stosq
Clear buffer at [rbp-0x70] (username)
<+53> lea rdx, [rbp-0xa0]
<+73> rep stosq
Clear buffer at [rbp-0xa0] (file content)
<+85> lea rdx, [rbp-0x110]
<+105> rep stosq
Clear buffer at [rbp-0x110] (password)



Open file:
<+148> call fopen
<+153> mov [rbp-0x8], rax
<+157> cmp [rbp-0x8], 0
<+162> jne <success>
Call fopen
If NULL → error path


Error (fopen failed):

<+174> mov eax, 0x400bd0
<+195> call fwrite
<+205> call exit
Print error message
Exit program


Read file:

<+237> call fread
<+242> mov [rbp-0xc], eax
Read 0x29 bytes into [rbp-0xa0]




Remove newline:

<+260> call strcspn
<+265> mov BYTE PTR [...], 0
Replace \n with 0



Check size:

<+273> cmp [rbp-0xc], 0x29
<+277> je <ok>
If not 41 bytes → error + exit



Close file:
<+368> call fclose


Print messages:
<+378> call puts
<+388> call puts
<+398> call puts
<+408> call puts


Read username:
<+453> call fgets
<+470> call strcspn
<+475> mov BYTE PTR [...], 0
Stored at [rbp-0x70]


Read password:
<+523> call fgets
<+543> call strcspn
<+548> mov BYTE PTR [...], 0
Stored at [rbp-0x110]


Compare:
<+591> call strncmp
<+596> test eax, eax
<+598> jne <fail>
Compare:
[rbp-0xa0] (file)
[rbp-0x110] (input)


Success:
<+620> call printf
<+630> call system
<+640> leave
<+641> ret
Print success message
Execute command



Failure:
<+654> call printf
<+664> call puts
<+674> call exit


Important:

<+654> call printf
Uses user buffer ([rbp-0x70]) directly
No format string → vulnerability

