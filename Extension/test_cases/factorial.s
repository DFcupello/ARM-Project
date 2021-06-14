mov r0,#1
mov r1,#5
loop:
mul r2,r1,r0
mov r0,r2
sub r1,r1,#1
cmp r1,#0
bne loop
mov r3,#0x100
str r2,[r3]
