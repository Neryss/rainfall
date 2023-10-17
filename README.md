# Rainfall

Another security project this time aiming towards reverse engineering C code.

### VM Install
Using virtualBox

1) Create a network adapter
2) Create your vm, select your os (linux 64bit) but don't input any iso for now.
3) Do the usual disk stuff
4) "settings" -> "network" -> set "attached to" to "bridged adapter"
5) Launch the VM
6) When asked, enter the ISO path and reboot.

## Resources

- [iso](https://files.neryss.pw/random/RainFall.iso)
- [subject](./en.subject.pdf)
- [cutter](https://cutter.re/): reverse engineering tool, you can also use [Ghidra](https://ghidra-sre.org/) for example!

## Level/pass

| level    | pass                                                             |
|----------|------------------------------------------------------------------|
| level0   | level0                                                           |
| level1   | 1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a |
| level2   | 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77 |
| level3   | 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02 |