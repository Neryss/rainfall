int v()
{
  int result; // eax
  char s[520]; // [esp+10h] [ebp-208h] BYREF

  fgets(s, 512, stdin);
  printf(s);
  result = m;
  if ( m == 64 )
  {
    fwrite("Wait what?!\n", 1u, 0xCu, stdout);
    return system("/bin/sh");
  }
  return result;
}
// 804988C: using guessed type int m;

//----- (0804851A) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  return v();
}

