int __cdecl greetuser(char src)
{
  __int128 dest; // [esp+10h] [ebp-48h] BYREF
  __int16 v3; // [esp+20h] [ebp-38h]
  char v4; // [esp+22h] [ebp-36h]

  switch ( language )
  {
    // this sets the buffer to random 
    case 1:
      *(_QWORD *)&dest = 0x20A4C3A4C3767948LL;
      *((_QWORD *)&dest + 1) = 0xC3A4C37669A4C370LL;
      v3 = unk_8048727;
      v4 = unk_8048729;
      break;
    case 2:
      strcpy((char *)&dest, "Goedemiddag! ");
      break;
    case 0:
      strcpy((char *)&dest, "Hello ");
      break;
  }
  strcat((char *)&dest, &src);
  return puts((const char *)&dest);
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  char v4[76]; // [esp+0h] [ebp-ACh] BYREF
  char dest[76]; // [esp+50h] [ebp-5Ch] BYREF
  char *v6; // [esp+9Ch] [ebp-10h]

  if ( argc != 3 )
    return 1;
  memset(dest, 0, sizeof(dest));
  strncpy(dest, argv[1], 0x28u);
  strncpy(&dest[40], argv[2], 0x20u);
  v6 = getenv("LANG");
  if ( v6 )
  {
    if ( !memcmp(v6, "fi", 2u) )
    {
      language = 1;
    }
    else if ( !memcmp(v6, "nl", 2u) )
    {
      language = 2;
    }
  }
  qmemcpy(v4, dest, sizeof(v4));
  return greetuser(v4[0]);
}
