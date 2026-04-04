Main:


Prologue & Stack:
<+0>  push ebp
<+3>  and  esp, 0xfffffff0
<+6>  sub  esp, 0x50
Allocate 0x50 bytes
Stack canary enabled

Print Messages:
<+37> call puts
<+81> call printf

Read Username:
<+103> lea eax, [esp+0x2c]
<+110> call fgets
Max 32 bytes (safe)

Read Serial:
<+173> lea edx, [esp+0x28]
<+180> call scanf

Call auth:
<+193> call auth
<+205> test eax, eax
<+207> jne fail

Success:
<+216> call puts
<+228> call system
Executes command

Stack Canary Check:
<+249> xor edx, gs:0x14
<+258> call __stack_chk_fail


auth:
-Clean Input;
<+20> call strcspn
Remove \n
-Length Check:
<+62> cmp len, 5
<+66> jg continue
Must be > 5
-Anti-Debug:
<+109> call ptrace
<+117> cmp eax, -1
-Init Hash:
<+171> mov al, login[3]
<+177> xor eax, 0x1337
<+182> add eax, 0x5eeded
-Loop (Hash Computation):
<+199> loop
<+233> xor ecx, var
<+243> mul edx
<+268> add var, eax
Iterates over username
Builds custom hash
-Final Check:
<+286> cmp serial, var
<+289> je success


⚠️ Vulnerability
-Logic-Based (No Memory Bug):
<+286> cmp serial, var
Serial = hash(username)
Algorithm fully visible
-Weakness:
No secret
Fully reversible



--Summary--:
Safe input handling
Stack protected
Security relies on obscurity only