Main:


Prologue & Stack:

<+0>  push rbp
<+4>  sub  rsp, 0xb0
Allocate 0xb0 bytes
Stack canary enabled

Check Args:

<+50> cmp argc, 2
<+57> je  continue
<+59> call printf
If wrong args → print usage

Open Log File:

<+106> call fopen
<+118> cmp rax, 0
<+126> jne ok
<+146> call printf
<+156> call exit

Log Start:

<+190> call log_wrapper
Open Input File
<+220> call fopen
<+229> cmp rax, 0
<+234> jne ok
<+266> call printf
<+276> call exit

Build Output Path:

<+281> copy "base_path" → buffer
<+344> repnz scas       ; strlen
<+397> call strncat

⚠️ User controls appended path

Open Output File:

<+424> call open
<+432> cmp eax, 0
<+436> jns ok
<+470> call printf
<+480> call exit

Copy File:

<+517> call fgetc
<+522> mov [buf], al
<+529> cmp al, 0xff     ; EOF
<+531> jne write

<+500> call write

Loop:

while (c != EOF)
    write(fd, &c, 1);
Log End + Cleanup
<+562> call log_wrapper
<+574> call fclose
<+584> call close
Stack Canary Check
<+598> xor rdi, fs:0x28
<+609> call __stack_chk_fail

--log_wrapper--
Copy Message (⚠️):
<+67> call strcpy

⚠️ Buffer Overflow

Append Input:
<+210> call snprintf

Write Log:
<+273> call fprintf



---Summary----

Behavior:
Opens file from argv[1]
Copies it to another path
Logs actions

⚠️ Vulnerabilities:

call strcpy   ; overflow
call strncat  ; path control