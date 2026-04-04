Main:


Prologue & Stack:
<+0>  push ebp
<+3>  and  esp, 0xfffffff0
<+6>  sub  esp, 0x20
Print + Prompt
<+16> call puts
<+53> call printf

Read Input:
<+58> lea edx, [esp+0x1c]
<+74> call scanf
scanf("%d", &input);

Check:
<+83> cmp eax, 0x149c (5276)
<+88> jne fail

--Summary--
Reads integer
Compares with 0x149c (5276)
If correct → success