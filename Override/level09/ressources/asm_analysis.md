Main:

Prologue & Stack:
<+0>  push rbp
<+1>  mov  rbp, rsp

Standard function setup

Flow:
<+11> call puts
<+16> call handle_msg
Print banner
Call main logic

handle_msg:
Prologue & Stack:
<+4>  sub rsp, 0xc0
Allocate 0xc0 bytes (~192 bytes) → main buffer

Initialize structure:
<+11> lea rax, [rbp-0xc0]
<+63> mov [rbp-0xc], 0x8c
Buffer at [rbp-0xc0]
len = 0x8c (140)

Call handlers:
<+80> call set_username
<+95> call set_msg
Same buffer passed → shared structure


--set_username:--

Prologue & Stack:
<+4> sub rsp, 0xa0
Clear local buffer:
<+44> rep stos
User input:
<+107> call fgets
Reads up to 0x80 bytes

Copy loop:
<+146> mov [rdx + rax + 0x8c], cl
<+157> cmp i, 0x28
Equivalent:
for (i = 0; i <= 40; i++)
    buf[0x8c + i] = input[i];
Off-by-one overflow
Loop runs 41 times
Writes 1 byte past username

--set_msg:--

Prologue & Stack:
<+4> sub rsp, 0x410
Clear buffer:
<+44> rep stos
User input:
<+107> call fgets
Reads up to 0x400 bytes
Use controlled length:
<+119> mov eax, [buf + 0xb4]
<+148> call strncpy
Equivalent:
strncpy(buf, input, len);
Structure Layout
buf (rbp-0xc0)
offset 0x8c → username (40 bytes)
offset 0xb4 → len


--Vulnerabilities--
1. Off-by-one (set_username)
buf[0x8c + 40] → overwrite len

2. Controlled copy size
strncpy(buf, input, len)
len is user-controlled
Leads to overflow


---Summary---

Behavior:
-Read username
-Read message
-Store both in same buffer

Vulnerabilities:
-off-by-one → overwrite len
-strncpy    → overflow