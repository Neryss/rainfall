# Level0

We found a file named [level0](./level0) which we decompiled [here](./level0_decomp.c).

We notice at first the use of `setresgid()` and `setresguid()` which means that the program will be executed as the owner.

When you try to execute it in several ways:

```bash
level0@RainFall:~$ ./level0
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 asd
No !
level0@RainFall:~$
```
