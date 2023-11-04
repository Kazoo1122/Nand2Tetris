// Stack is initialized.
@256
D=A
@SP
M=D
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_EQ.0
D;JNE
@SP
A=M
M=-1
(END_EQ.0)
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_EQ.1
D;JNE
@SP
A=M
M=-1
(END_EQ.1)
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_EQ.2
D;JNE
@SP
A=M
M=-1
(END_EQ.2)
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_LT.3
D;JGE
@SP
A=M
M=-1
(END_LT.3)
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_LT.4
D;JGE
@SP
A=M
M=-1
(END_LT.4)
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_LT.5
D;JGE
@SP
A=M
M=-1
(END_LT.5)
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_GT.6
D;JLE
@SP
A=M
M=-1
(END_GT.6)
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_GT.7
D;JLE
@SP
A=M
M=-1
(END_GT.7)
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
M=0
@END_GT.8
D;JLE
@SP
A=M
M=-1
(END_GT.8)
@SP
M=M+1
// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 53
@53
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M+D
@SP
M=M+1
// push constant 112
@112
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1
// neg
@SP
AM=M-1
D=M
M=-M
@SP
M=M+1
// and
@SP
AM=M-1
D=M
@SP
AM=M-1
M=D&M
@SP
M=M+1
// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
// or
@SP
AM=M-1
D=M
@SP
AM=M-1
M=D|M
@SP
M=M+1
// not
@SP
AM=M-1
D=M
M=!M
@SP
M=M+1
// Add infinite loop at the end.
(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
