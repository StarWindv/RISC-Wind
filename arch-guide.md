寄存器定义


从功能上看, 实际上只有 12 个寄存器才是真正常用的:

01 - AX  - Accumulator
02 - BX  - Base
03 - CX  - Count
04 - DX  - DATA
05 - SI  - Source Index
06 - DI  - Destination Index

07 - STP - Stack Top Pointer
08 - SBP - Stack Bottom Pointer
09 - LR  - Link  Resgister
10 - PC  - Programm Pointer
11 - FP  - Frame Pointer
12 - MX  - Misc

而现在大家都把其中部分寄存器当成通用寄存器使用, 所以 1~6 完全可以退化成通用寄存器
那么我们就只剩下了 [STP, SBP, LR, PC, FP, MX] 这六个, 然后再加 6 个通用寄存器
按照一些传统, 直接提升到 2^4=16 个寄存器即可, 6 具名, 10 通用

具名寄存器更名如下

01 - TPR - (Stack) Top Pointer Register     栈顶指针
02 - BPR - (Stack) Bottom Pointer Register  "整个栈"的底
03 - LR  - Link  Register
04 - PCR - Programm Pointer
05 - FPR - Frame Pointer Register  "当前"帧底
06 - MR  - Misc  Register

通用寄存器使用"N R {HexIndex}"作为名称, 
例如第四号通用寄存器就是 "NR4", 释义为 "Normal Register 4"
而第十号通用寄存器则是 "NRA", A 也就是十六进制的十

---

内存与寄存器访问(L/S)

用于砍掉特殊括号语法, 变量在非对应指令下都直接取内容, 而不是传统的直接操作地址

LOAD  访存, 将内存的内容装载进寄存器, 只能用于搬内容
ALOAD "Address Load", 访存, 
      将变量的值视为地址, 然后取出此地址对应的内容装载进寄存器, 只能用于解引用

STR   访存, 将寄存器的值存入内存
STRAI 访存, 将立即数的值存入给定内存,
      例如 "STRAI 16, 65534 value", 16 代表 "word", 总比查表才能搞清楚长度的好
      不过如你所见, 这里目前也只是一个示意, 未来或许也不会有"16"这个length参数,
      能留给立即数的直接长度肯定也不会特别长
STRA  访存, 将变量的值视为地址, 然后将此地址对应的内容存入给定目标(例如内存)

COPY  不可访存, 只用于寄存器间传递数据
      (其实就是传统汇编的MOVE, 功能是一致的, 只是语义更合理)

LOADI 砍掉,
      用于把立即数存入指定寄存器,
      可以用 STRAI + LOAD 模拟, 保持精简, 多一点内存访问也认了

---

ALU
我们不允许ALU操作直接对接内存, 内存操作只能由L/S指令发出
下方可能出现的"val"等均指寄存器

SADD "Special Add", 三参, "SADD reg1, reg2 result", 不隐式改标志位/寄存器
     是一种类似 RISC-V 的方案
SSUB "Special Sub", 三参, 类似SADD

NANDT "NAnd Something to receiver" -> "NAnd to",
     三参, "NANDT reg1, reg2 receiver",
     不改变全局标志位, 将结果存入指定位置, 全称为 NAnd a, b to result
XORT 砍掉, 可用NANDT模拟
CMPT 砍掉, 可用SUBM模拟

ARSFT 算数右移, 砍掉, 可以用逻辑位移模拟得到
LRSFT 逻辑右移
LSFT  左移, "LSFT 4, Value_or_Register Receiver"

(循环模拟的SHIFT太慢了, 请允许我开一点挂吧)
(左移没有算数/逻辑的区分)


LGR "Left Greater than Right", 左是否大于右?
    为真时, result 赋值为 1
    "LGR val1, val2 result" -> result = val1 > val2 ? 1 : 0

---

跳转族
支持寄存器间接跳转, 不允许使用变量值作为地址进行跳转

JMP  标准无条件跳转, "JMP address"
JZ   两参(Jump-if-Zero variable target), 不依赖全局标志位, 当 variable 变量/寄存器的值==0时跳转到target
JNZ  类似JZ, 当 variable 变量!=0时跳转到 target

跳转族函数支持将寄存器/变量中的值视为地址来进行跳转

---

无题
(我真的不知道他应该起什么名字)


HALT "挂空挡怠速行驶"

---

1.
 - Q: 如何判断发生了溢出?
 - A: Num_1 > ( Num_1 + Num_2 )

2.
 - Q: 如何模拟其它比较?
 - A:
    >   : val1>val2?1:0
    <=  : !(>)
    ==  : JZ
    <   : !(>+JZ)
    >=  : JZ+>

---

寄存器:
 1. 具名寄存器: [ TPR, BPR, LR, PCR, FPR, MR ]
 2. 通用寄存器: [ NR1, NR2, ..., NR9, NRA ]

助记符:
 1. 内存与寄存器: [ LOAD, ALOAD, STR, STRA, STRAI, COPY ]
 2. ALU     : [ SADD, SSUB, NANDT, LSFT, LRSFT ]
 3. 跳转族  : [ JMP, JZ, JNZ ]
 4. CPU 状态: [ HALT ]
 5. 比较    : [ LGR ]

共计:
 6 + 10 = 16 寄存器
 6 + 5 + 3 + 1 + 1 = 16 助记符

---

行注释    // Comment
块注释    /* Comment */
符号绑定  LET myvar SBP-1    伪指令
(总之就是一点高级语言的习惯, let 用习惯了)
(为什么一定要什么 EQU 之类的东西)

---

此汇编的书写:

[Action] [Group] <Receiver>
 * 读作 "Action from(if) Group to Receiver"

此处各括号仅做位置标记, 实际书写时不应存在
中括号 [/]: 必须
尖括号 </>: 可选

Action   即为助记符
Receiver 可选的结果接收者

Group: 动作所需的参数与条件, 按照下图排序

[ Condition | Object 1 | Object 2 | Immediate ]

各块之间按照英文半角逗号来连接, 中间可以有可选空格
Group 和 Receiver 之间只能有空格, 不接受 Group 末尾逗号

这里的 Condition, 指的是 Action 所需要的额外参数,
    Object 是 Action 所读取的对象, 可以是寄存器/变量, 或者其它什么东西

---

寄存器/变量使用符号数学, 得到的是内存地址计算
寄存器/变量使用汇编数学, 得到的是值计算

---

举个例子, 对于逻辑左移, 它需要有一个被读取的源, 还需要一个值来告诉它位移多少位, 这里的offset就是条件

```WindAssembly
COPY BPR TPR

COPY TPR FPR

SSUB 4, TPR TPR

LET original FPR-1
LET offset   FPR-2

STRAI 1, 6 original
STRAI 1, 8 offset

// original = original << offset
LSFT offset, original original

COPY FPR TPR
```

--

对于我来说, 我并不愿意见到全局变量满天飞的情况,
也很不喜欢传统汇编的 "地址占右边, 接收者放中间" 的操作,
这不仅使得初学者极其难以理解, 也导致语义上和习惯不相符

对于每一个会英语的人来说, "from ... to ..." 应该是刻在习惯里的操作, 所以此方案完全没有任何特殊的记忆成本

---

精简化?
确实可以...
比如, COPY(MOVE) 就是 `SADD 寄存器A, 0 <接收寄存器>`
LSFT 和 LRSFT 可以用跳转族模拟循环来做
JMP 和 JZ 也完全可以砍掉, 用 JNZ 和 NAND 表达
这样就只剩下了 11 条 op
但是你不觉得那样写的很累很慢吗.... 虽然有伪码和宏, 但是我不觉得全砍掉是什么好主意...

---

内存序: 小端序
MMU 和 译码器 必做
硬盘持久化存储: 物理机上一个文件句柄直接mmap进虚拟机
输入/输出: MMIO
位宽: 32-bit
不考虑有符号数. CPU 本身就不该认识什么叫"有符号", "无符号",
    它必须只能认识"操作", "内存" 和 "寄存器" (以及立即数)

