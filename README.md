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
| level4   | b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa |
| level5   | 0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a |
| level6   | d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31 |
| level7   | f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d |
| level8   | 5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9 |
| level9   | c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a |
|----------|------------------------------------------------------------------|

| bonus    | pass                                                             |
|----------|------------------------------------------------------------------|
| bonus0   | f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728 |
| bonus1   | cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9 |

## GDB tips

- You can pipe args into it using `run < file` while in degub mode
- `disas <function>` lets you see the execution frame of each function

## General tips

- `xdd` to see the content of files or `objdump`
- `objdump` -R to see dynamic relocations, useful for `plt` functions
