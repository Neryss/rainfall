int main(unsigned int a0, struct_0 *a1)
{
    char  v0[40];  // [bp-0x30]
    int   v1;  // [bp-0x8]

    v1 = atoi(a1->field_4);
    if (v1 > 9)
        return 1;
    memcpy(&v0, a1->field_8, v1 * 4);
    if (v1 != 0x574f4c46)
        return 0;
    execl("/bin/sh", "sh");
    return 0;
}