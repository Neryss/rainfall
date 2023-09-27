# Level1

## Analysis

Another executable, [level1](./level1).

When we execute it, it will open a prompt where we can type stuff:

```bash
level1@RainFall:~$ ./level1
asddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
Segmentation fault (core dumped)
level1@RainFall:~$ ./level1

level1@RainFall:~$ ./level1
asdasdalsd
level1@RainFall:~$
```

First thing we notice is that it segfaults when typing too much characters.

Precisely after 76 characters as shown next:

```bash
~/Documents/42/rainfall/level1/resources (main) » python -c 'print "a"*76' | ./level1                                                                                                                                                                                                                                                         ckurt@z1r6p3
[1]    145781 done                              python -c 'print "a"*76' | 
       145782 segmentation fault (core dumped)  ./level1
```

We also decompiled the program [here](./level1_decomp.c) and noticed several things:

- The main is mostly empty

```c
int32_t main(int32_t argc, char** argv, char** envp)
{
    void buf;
    return gets(&buf);
}
```
But it returns the result of `gets()`.

- There is also another function outside of the main:

```c
int32_t run()
{
    fwrite("Good... Wait what?\n", 1, 0x13, stdout);
    return system("/bin/sh");
}
```

## Exploit