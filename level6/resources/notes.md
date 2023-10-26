## Level6

You can find the decompiled level6 code [here](./level6_decomp.c).

There are several interesting parts in this decomp, first of all, two functions `m()` and `n()`. `m()` doesn't do much, but is used inside the main, and `n()` which cat the password.

Inside our main, there is a call to `strcpy()` and a pointer to function `*ppcVar1 = m;`, which points to the function `m()` and calls it later.

We also noticed that it segfaults after 72 characters:
```bash
level6@RainFall:~$ ./level6 aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrr
Segmentation fault (core dumped)
```