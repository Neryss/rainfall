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


