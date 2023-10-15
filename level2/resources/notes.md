# Level2

When we take a look at the [decom](./level2_decomp.c), we notice the use of `gets()` a line to check if an address is located in the `0xb0000000` range.
We can also see a call to `strdup()`.