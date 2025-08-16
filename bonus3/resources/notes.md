# Bonus3

## Analysis

The first thing we noticed is that the program reads the password with `fopen()` and later spawns a shell using `execl()`.

We can pass an argument to the program, which will be used to put a null byte inside of the buffer in which the password is stored (`ptr[132]`). It will put the null byte as follows: `ptr[atoi(argv[1])] = 0`.

After this manipulation, the other part of the password is stored and the fd closed.

At first, we thought about reading the content directly from the memory using GDB. However, `fopen()` will fail if used through GDB.

Following this, there is a call to `strcmp()` which will compare the value of `argv[1]` with `ptr`, if they are both the same, the shell is spawned and we can get the password.

## Exploit

The exploit lies in the call to `strcmp()` and our ability to place a null byte in one of the element it compares. Since it uses the value returned by `atoi()` to place the null byte in `ptr`, we can exploit the fact that `atoi()` returns 0 in several cases.

What we went for was the following case:

If we give and empty string to `atoi()`, it will return 0 since the string doesn't contain any number that can be represented, doing so will replace the first character of `ptr` with a `\0` as such: `ptr[atoi("")] = 0 -> ptr[0] = 0`.

Therefore, `strcmp()` will stop at `\0` and both strings will be equal, allowing us to pass the check:

```bash
bonus3@RainFall:~$ ./bonus3 ""
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c


bonus3@RainFall:~$ su end
Password: 
end@RainFall:~$ cat end 
Congratulations graduate!
end@RainFall:~$ 
```
