# Level0

## Analysis

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

## Solution

If we take a look at the [decompiled program](./level0_decomp.c), there is a simple solution right here:

```c
iVar1 = atoi(*(char **)(param_2 + 4));
  if (iVar1 == 0x1a7) {
    local_20 = strdup("/bin/sh");
    local_1c = 0;
    local_14 = getegid();
    local_18 = geteuid();
    setresgid(local_14,local_14,local_14);
    setresuid(local_18,local_18,local_18);
    execv("/bin/sh",&local_20);
  }
```

`iVar1` is our argv, and it will execute a shell with the UID of level1 if it is equel to `0x1a7 (423)`.

Which leads us to:

```bash
level0@RainFall:~$ ./level0 423
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
$
```

## Flag

1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a