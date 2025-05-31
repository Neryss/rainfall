extern unsigned int c;

int m()
{
    unsigned int v0;  // [bp-0x18]
    unsigned int v1;  // [bp-0x14]

    v1 = time(NULL);
    v0 = &c;
    return printf("%s - %d\n");
}

typedef struct struct_0 {
    unsigned int field_0;
    char *field_4;
} struct_0;

extern char c;

int main(unsigned int a0, char *a1[3])
{
    struct_0 *v0;  // [bp-0xc]
    struct_0 *v1;  // [bp-0x8]

    v1 = malloc(8);
    v1->field_0 = 1;
    v1->field_4 = malloc(8);
    v0 = malloc(8);
    v0->field_0 = 2;
    v0->field_4 = malloc(8);
    strcpy(v1->field_4, a1[1]);
    strcpy(v0->field_4, a1[2]);
    fgets(&c, 68, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
    return 0;
}