int32_t _init()
{
    if (__gmon_start__ != 0)
    {
        __gmon_start__();
    }
    frame_dummy();
    return __do_global_ctors_aux();
}

int32_t sub_8048330()
{
    int32_t var_4 = data_8049790;
    /* jump -> data_8049794 */
}

char* gets(char* buf)
{
    /* tailcall */
    return gets(buf);
}

int32_t sub_8048346()
{
    /* tailcall */
    return sub_8048330(0);
}

size_t fwrite(void const* buf, size_t size, size_t count, FILE* fp)
{
    /* tailcall */
    return fwrite(buf, size, count, fp);
}

int32_t sub_8048356()
{
    /* tailcall */
    return sub_8048330(8);
}

int32_t system(char const* line)
{
    /* tailcall */
    return system(line);
}

int32_t sub_8048366()
{
    /* tailcall */
    return sub_8048330(0x10);
}

int32_t __gmon_start__()
{
    /* tailcall */
    return __gmon_start__();
}

int32_t sub_8048376()
{
    /* tailcall */
    return sub_8048330(0x18);
}

void __libc_start_main(int32_t (* main)(int32_t argc, char** argv, char** envp), int32_t argc, char** ubp_av, void (* init)(), void (* fini)(), void (* rtld_fini)(), void* stack_end) __noreturn
{
    /* tailcall */
    return __libc_start_main(main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}

int32_t sub_8048386()
{
    /* tailcall */
    return sub_8048330(0x20);
}

int32_t __convention("regparm") _start(int32_t arg1, void (* arg2)()) __noreturn
{
    int32_t stack_end = arg1;
    __libc_start_main(main, __return_addr, &ubp_av, __libc_csu_init, __libc_csu_fini, arg2, &stack_end);
    /* no return */
}

void __do_global_dtors_aux()
{
    if (completed.6159 == 0)
    {
        uint32_t dtor_idx.6161_1 = dtor_idx.6161;
        if (dtor_idx.6161_1 < 0)
        {
            do
            {
                dtor_idx.6161 = (dtor_idx.6161_1 + 1);
                __DTOR_LIST__[(dtor_idx.6161_1 + 1)]();
                dtor_idx.6161_1 = dtor_idx.6161;
            } while (dtor_idx.6161_1 < 0);
        }
        completed.6159 = 1;
    }
}

int32_t frame_dummy()
{
    return __JCR_END__;
}

int32_t run()
{
    fwrite("Good... Wait what?\n", 1, 0x13, stdout);
    return system("/bin/sh");
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    void buf;
    return gets(&buf);
}

void __libc_csu_init()
{
    _init();
}

void j___libc_csu_fini()
{
    /* tailcall */
    return __libc_csu_fini();
}

void __libc_csu_fini() __pure
{
    return;
}

int32_t __i686.get_pc_thunk.bx() __pure
{
    return;
}

int32_t __do_global_ctors_aux()
{
    return 0xffffffff;
}

int32_t _fini()
{
    return __do_global_dtors_aux();
}

