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

We also decompiled the program [here](./level1_decomp.c) and noticed several things: