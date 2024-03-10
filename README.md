# MyCCompiler-1stGen
The 1st genaration of C compiler to meet a simple lab's requirements(which called '1th genaration of C compiler').

full requirements goto see the `./labinfo/Lab1实验说明和要求-V0.5.pdf`

# Grammer
 - Only two keywords: `int`, `return`.
 `int` means declare a local virable.
 `return` means mov the value of following expression into `eax` register and waiting to be printed.
 - Identities can be consist of only one letter.
 - Support using constants no more than `int`.
 - Each sentence must end with a `;`.
 - All words(including identites, keywords, constants, punctuations, etc.)must be devided by at least a space or newline.

## Sample-1
 input:
```C
int a ;
int b ;
int d ;
a = 1 ;
b = 2 ;
d = a + b ;
return d ;
```
output:
```asm
mov DWORD PTR [ebp-4], 0 # int a
mov DWORD PTR [ebp-8], 0 # int b
mov DWORD PTR [ebp-12], 0 # int d
mov DWORD PTR [ebp-4], 1 # a = 1
mov DWORD PTR [ebp-8], 2 # b = 2

mov eax, DWORD PTR [ebp-4] # d = a + b
push eax
mov eax, DWORD PTR [ebp-8]
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov DWORD PTR [ebp-12], eax
mov eax, DWORD PTR [ebp-12] # return d
```

## Sample-2
input:
```C
int a ;
int b ;
int c ;
int d ;
a = 1 ;
b = 2 ;
c = 3 ;
d = ( a + b * 2 ) / c - 3 ;
return d ;
```
output:
```asm
 mov DWORD PTR [ebp-4], 0 # int a
 mov DWORD PTR [ebp-8], 0 # int b
 mov DWORD PTR [ebp-12], 0 # int c
 mov DWORD PTR [ebp-16], 0 # int d
 mov DWORD PTR [ebp-4], 1 # a = 1
 mov DWORD PTR [ebp-8], 2 # b = 2
 mov DWORD PTR [ebp-12], 3 # c = 3

 mov eax, DWORD PTR [ebp-4] # d = ( a + b * 2 ) / c - 3
 push eax
 mov eax, DWORD PTR [ebp-8]
 push eax
 mov eax, 2
 push eax
 pop ebx
 pop eax
 imul eax, ebx
 push eax
 pop ebx
 pop eax
 add eax, ebx
 push eax
 mov eax, DWORD PTR [ebp-12]
 push eax
 pop ebx
 pop eax
 cdq
 idiv ebx
 push eax
 mov eax, 3
 push eax
 pop ebx
 pop eax
 sub eax, ebx
 push eax
 pop eax
 mov DWORD PTR [ebp-16], eax

 mov eax, DWORD PTR [ebp-16] # return d
```

# Other Requirements
 - The corresponding assembly code is required to be printed to the standard output.
 - Environment requiments:
    - Host language(used to program compiler): C++(C++14)
    - Host compiler(used to compile the compiler's source code): `g++-11`
    - Command line arguments: only 1, to figure out the input file.

# Code Testing
 the code is be deifinited to be right only when the following assembly program added with the generated code can print the right value:
 ```asm
    .intel_syntax noprefix # 使用 Intel 语法
    .global main # 声明 main 函数为全局符号，这使得链接器能够识别程序的入口点。
    .extern printf # 声明外部函数 printf，表示该函数在其他地方定义，通常是 C 标准库中。
    .data # 开始数据段，用于定义程序中的初始化数据。
format_str:
    .asciz "%d\n" # 定义一个用于 printf 的格式字符串，输出整数并换行。
    .text # 开始代码段，包含程序的实际指令。
main:
    push ebp # 将基指针寄存器 ebp 的当前值压入堆栈，保存上一个函数栈帧的基指针
    mov ebp, esp # 将栈指针 esp 的值复制到基指针 ebp ，设置新的栈帧基指针
    sub esp, 0x100 # 从栈指针 esp 减去 256 字节，为局部变量分配栈空间
    ###############################
    # 
    #
    # [Here put the code printed by your compiler.]
    #
    #
    ###############################
    # 打印 d (当前 eax 的值)
    push eax # 将结果 (eax 的值) 作为 printf 的参数
    push offset format_str # 将格式字符串的地址作为 printf 的参数
    call printf # 调用 printf 函数
    add esp, 8 # 清理栈
    # 恢复 eax 的值并退出 main
    pop eax
    leave
    ret
 ```
