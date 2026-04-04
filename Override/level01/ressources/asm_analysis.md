Main:


Prologue & Stack:
<+0>  push ebp
<+5>  and esp, 0xfffffff0
<+8>  sub esp, 0x60
Allocate 0x60 bytes

Init Buffer:
<+11> lea ebx, [esp+0x1c]
<+29> rep stos
Clear buffer

Read Username:
<+81> mov [esp], 0x804a040   ; buffer
<+88> call fgets

Verify Username:
<+93> call verify_user_name
<+102> cmp [esp+0x5c], 0
<+107> je success

If Wrong Username:
<+109> call puts

Read Password:
<+157> lea eax, [esp+0x1c]
<+164> call fgets

Verify Password:
<+176> call verify_user_pass
<+185> cmp [esp+0x5c], 0

Result:
<+199> puts("fail")
<+218> return 0/1

verify_user_name:

Compare Username:
<+30> mov ecx, 0x7
<+39> repz cmpsb
Equivalent:
strncmp(input, "secret", 7)

Return Result
<+41> seta dl
<+44> setb al
Returns 0 if equal
Otherwise ≠ 0

--Summary--
Read username
Compare with hardcoded string (7 bytes)
If correct → ask password
Verify password