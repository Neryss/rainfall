# Bonus0

## Analysis

```bash
bonus0@RainFall:~$ ./bonus0 
 - 
hi 
 - 
there
hi there
bonus0@RainFall:~$ ./bonus0 
 - 

 - 
```

Executing the program prompts us with inputs fields, it then prints what we wrote as an input. The program seems to read stdin 2 times.

### Code

We have 3 interesting functions:

### 1. `main()`

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char s[42]; // [esp+16h] [ebp-2Ah] BYREF

  pp(s);
  puts(s);
  return 0;
}
```

- We have a 42bytes buffer `s` on the stack
- `s` is passed to `pp()`
- `s` is then printed by `puts()`

### 2. `pp()`

```c
char *__cdecl pp(char *dest)
{
  char src[20]; // [esp+28h] [ebp-30h] BYREF
  char v3[28]; // [esp+3Ch] [ebp-1Ch] BYREF

  p(src, " - ");
  p(v3, " - ");
  strcpy(dest, src);
  *(_WORD *)&dest[strlen(dest)] = unk_80486A4;
  return strcat(dest, v3);
}
```

- 2 buffers: `src` of 20bytes and `v3` of 28bytes
- `src` is passed to `p()` first.
- `v3` is also passed to `p()`.
- Both `p()` calls don't specify any length for the passed buffers.
- notes:
    - `strcpy()` doesn't check for length -> buffer overflow
    - `strcat()` doens't check for available length and modified `s` passed from `main()`

### 3. `p()`

```c
char *__cdecl p(char *dest, char *s)
{
  char buf[4104]; // [esp+10h] [ebp-1008h] BYREF

  puts(s);
  read(0, buf, 0x1000u);
  *strchr(buf, 10) = 0;
  return strncpy(dest, buf, 0x14u);
}
```

- a 4104bytes buffer `buf`.
- prints `s`.
- reads 4096bytes from `stdin` into `buf`.
- replaces `10` or newline, in `buf` by 0.
- copies 20 bytes of `buf` into `dest`

## What we have:

`pp()` seems very unsafe because of the non existent length check:

It copies `src` (20bytes) and `v3` (28bytes) into `dest` (42bytes) which means we could overwrite the return address saved on the stack during this function frame

AAAAAAAAAAAAAAAAA