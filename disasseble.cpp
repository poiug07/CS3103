
int64_t __gmon_start__ = 0;

void _init() {
    int64_t rax1;

    rax1 = __gmon_start__;
    if (rax1) {
        rax1();
    }
    return;
}

int64_t __cxa_finalize = 0;

void fun_c60(int64_t rdi) {
    goto __cxa_finalize;
}

struct s0 {
    signed char[8] pad8;
    void* f8;
    int64_t f16;
    int64_t f24;
};

int64_t fun_b40(void* rdi);

int32_t* fun_ae0(void* rdi, struct s0* rsi);

void fun_b90(int64_t rdi);

int32_t isFile(void* rdi, struct s0* rsi) {
    int64_t rax3;
    int32_t* rax4;
    int32_t eax5;

    rax3 = fun_b40(rdi);
    if (!rax3) {
        rax4 = fun_ae0(rdi, rsi);
        if (*rax4 != 20) {
            eax5 = -1;
        } else {
            eax5 = 1;
        }
    } else {
        fun_b90(rax3);
        eax5 = 0;
    }
    return eax5;
}

int64_t closedir = 0xb96;

void fun_b90(int64_t rdi) {
    goto closedir;
}

int64_t __stack_chk_fail = 0xb76;

uint64_t fun_b70(int32_t* rdi, struct s0* rsi, int32_t* rdx) {
    goto __stack_chk_fail;
}

int64_t atoi = 0xc26;

int32_t fun_c20(int64_t rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto atoi;
}

int64_t opendir = 0xb46;

int64_t fun_b40(void* rdi) {
    goto opendir;
}

int64_t __errno_location = 0xae6;

int32_t* fun_ae0(void* rdi, struct s0* rsi) {
    goto __errno_location;
}

int64_t strerror = 0xc56;

int32_t* fun_c50(int64_t rdi, struct s0* rsi) {
    goto strerror;
}

int64_t fprintf = 0xbc6;

void fun_bc0(int64_t rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto fprintf;
}

int64_t fclose = 0xb36;

void fun_b30(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto fclose;
}

int64_t strcat = 0xc36;

void fun_c30(void* rdi, int64_t rsi) {
    goto strcat;
}

int64_t fopen = 0xc06;

int32_t* fun_c00(void* rdi, struct s0* rsi) {
    goto fopen;
}

int64_t qsort = 0xb16;

void fun_b10(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto qsort;
}

int64_t puts = 0xb06;

void fun_b00(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto puts;
}

int64_t clock = 0xb26;

int64_t fun_b20(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx, int64_t r8, int64_t r9) {
    goto clock;
}

int64_t free = 0xad6;

void fun_ad0(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto free;
}

int64_t _ITM_deregisterTMCloneTable = 0;

int64_t deregister_tm_clones(int64_t rdi) {
    int64_t rax2;

    rax2 = 0x202010;
    if (1 || (rax2 = _ITM_deregisterTMCloneTable, rax2 == 0)) {
        return rax2;
    } else {
        goto rax2;
    }
}

uint64_t g28;

int64_t fun_ba0(int32_t* rdi, struct s0* rsi, int32_t* rdx);

int64_t fun_bd0(int32_t* rdi, void* rsi, int64_t rdx);

struct s1 {
    int32_t f0;
    int32_t f4;
};

uint64_t read_one_file(int32_t* rdi, int32_t esi, int32_t edx, int32_t* rcx) {
    void* rsp5;
    void* rbp6;
    int32_t* v7;
    int32_t v8;
    int32_t* v9;
    uint64_t rax10;
    uint64_t v11;
    int32_t* v12;
    int64_t rax13;
    int64_t rax14;
    int64_t rax15;
    int64_t rcx16;
    int64_t rdx17;
    struct s1* rax18;
    uint64_t rax19;

    rsp5 = reinterpret_cast<void*>(reinterpret_cast<int64_t>(__zero_stack_offset()) - 8);
    rbp6 = rsp5;
    v7 = rdi;
    v8 = edx;
    v9 = rcx;
    rax10 = g28;
    v11 = rax10;
    v12 = reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(rsp5) - 8 - 0x68 - 48);
    fun_b20(rdi, 16, 8, rcx, 41, 0);
    while (rax13 = fun_ba0(v12, 40, v9), !!rax13) {
        rax14 = fun_bd0(v12, reinterpret_cast<int64_t>(rbp6) - 64, 10);
        rax15 = rax14;
        *reinterpret_cast<int32_t*>(&rcx16) = *reinterpret_cast<int32_t*>(&rax15) - v8;
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rcx16) + 4) = 0;
        *reinterpret_cast<int32_t*>(&rdx17) = __intrinsic();
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdx17) + 4) = 0;
        rax18 = reinterpret_cast<struct s1*>(v7 + ((static_cast<int32_t>(rdx17 + rcx16) >> 11) - (*reinterpret_cast<int32_t*>(&rcx16) >> 31)) * 2);
        rax18->f4 = rax18->f4 + 1;
    }
    rax19 = v11 ^ g28;
    if (rax19) {
        rax19 = fun_b70(v12, 40, v9);
    }
    return rax19;
}

int64_t readdir = 0xbe6;

int64_t fun_be0(int64_t rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto readdir;
}

int64_t strcmp = 0xbb6;

int32_t fun_bb0(int64_t rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto strcmp;
}

int64_t strcpy = 0xaf6;

void fun_af0(void* rdi, void* rsi) {
    goto strcpy;
}

int64_t strlen = 0xb66;

void* fun_b60(struct s0* rdi, void* rsi, void* rdx, int32_t* rcx) {
    goto strlen;
}

int64_t sprintf = 0xc46;

void fun_c40(void* rdi, int64_t rsi, int64_t rdx, int32_t* rcx) {
    goto sprintf;
}

int64_t strtoul = 0xc16;

int64_t fun_c10(void* rdi, void* rsi, void* rdx, int32_t* rcx) {
    goto strtoul;
}

int64_t ctime = 0xb56;

struct s0* fun_b50(void* rdi, void* rsi, void* rdx, int32_t* rcx) {
    goto ctime;
}

int64_t printf = 0xb86;

void fun_b80(int32_t* rdi, struct s0* rsi, int32_t* rdx, int32_t* rcx) {
    goto printf;
}

int64_t fgets = 0xba6;

int64_t fun_ba0(int32_t* rdi, struct s0* rsi, int32_t* rdx) {
    goto fgets;
}

int64_t malloc = 0xbf6;

int32_t* fun_bf0(int64_t rdi) {
    goto malloc;
}

int64_t strtol = 0xbd6;

int64_t fun_bd0(int32_t* rdi, void* rsi, int64_t rdx) {
    goto strtol;
}

int64_t __libc_start_main = 0;

uint64_t main(void* rdi, struct s0* rsi);

void __libc_csu_init(int32_t edi, int64_t rsi, int64_t rdx);

void __libc_csu_fini();

void _start() {
    void* rsp1;
    int64_t rdx2;
    int64_t rax3;

    rsp1 = reinterpret_cast<void*>(reinterpret_cast<int64_t>(__zero_stack_offset()) + 8);
    __libc_start_main(main, __return_address(), rsp1, __libc_csu_init, __libc_csu_fini, rdx2, (reinterpret_cast<uint64_t>(rsp1) & 0xfffffffffffffff0) - 8 - 8, rax3);
    __asm__("hlt ");
}

void _fini() {
    return;
}

void fun_d17() {
    int64_t v1;

    goto v1;
}

void __libc_csu_init(int32_t edi, int64_t rsi, int64_t rdx) {
    int64_t r15_4;
    int32_t r13d5;
    int64_t r14_6;
    int64_t rbx7;
    int64_t rdi8;

    r15_4 = rdx;
    r13d5 = edi;
    r14_6 = rsi;
    _init();
    if (!0) {
        *reinterpret_cast<int32_t*>(&rbx7) = 0;
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rbx7) + 4) = 0;
        do {
            *reinterpret_cast<int32_t*>(&rdi8) = r13d5;
            *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdi8) + 4) = 0;
            *reinterpret_cast<int64_t*>("p\r" + rbx7 * 8)(rdi8, r14_6, r15_4);
            ++rbx7;
        } while (1 != rbx7);
    }
    return;
}

void __libc_csu_fini() {
    return;
}

int64_t _ITM_registerTMCloneTable = 0;

void fun_cd2() {
    int64_t rax1;

    if (1) 
        goto 0xd20;
    rax1 = _ITM_registerTMCloneTable;
    if (!rax1) 
        goto 0xd20;
    goto rax1;
}

/* completed.7698 */
signed char completed_7698 = 0;

int64_t __dso_handle = 0x202008;

int64_t __do_global_dtors_aux() {
    int1_t zf1;
    int64_t rax2;
    int1_t zf3;
    int64_t rdi4;
    int64_t rax5;

    zf1 = completed_7698 == 0;
    if (!zf1) {
        return rax2;
    } else {
        zf3 = __cxa_finalize == 0;
        if (!zf3) {
            rdi4 = __dso_handle;
            fun_c60(rdi4);
        }
        rax5 = deregister_tm_clones(rdi4);
        completed_7698 = 1;
        return rax5;
    }
}

int64_t g201f08 = 0;

void fun_b96() {
    goto g201f08;
}

struct s2 {
    int32_t f0;
    int32_t f4;
};

struct s3 {
    int32_t f0;
    int32_t f4;
};

int64_t counter_comp(struct s2* rdi, struct s3* rsi);

int64_t stderr = 0;

uint64_t main(void* rdi, struct s0* rsi) {
    void* rbp3;
    struct s0* v4;
    uint64_t rax5;
    uint64_t v6;
    int32_t* rax7;
    int32_t* rax8;
    int32_t* v9;
    int32_t v10;
    int32_t* rdx11;
    void* rdi12;
    int32_t eax13;
    int64_t rdi14;
    int32_t* rcx15;
    int32_t eax16;
    int64_t v17;
    int64_t rdi18;
    int32_t* rcx19;
    int32_t eax20;
    int32_t v21;
    struct s0* rsi22;
    int32_t* rdx23;
    int32_t* rdi24;
    int32_t v25;
    void* rdi26;
    int64_t rax27;
    int64_t v28;
    int64_t rax29;
    int32_t eax30;
    int32_t eax31;
    void* rdx32;
    void* rax33;
    uint32_t eax34;
    void* rdi35;
    int32_t* rax36;
    int64_t rax37;
    int32_t* rax38;
    int64_t rdi39;
    int32_t* rax40;
    int64_t rax41;
    int32_t* rcx42;
    int32_t* rdi43;
    int32_t* v44;
    int32_t* rcx45;
    uint64_t rax46;
    int64_t v47;
    struct s0* rax48;
    void* rax49;
    int64_t r8_50;
    int64_t r9_51;
    uint64_t rcx52;
    int32_t* rax53;
    int64_t rdi54;
    int32_t* rax55;
    int64_t rax56;

    rbp3 = reinterpret_cast<void*>(reinterpret_cast<int64_t>(__zero_stack_offset()) - 8);
    v4 = rsi;
    rax5 = g28;
    v6 = rax5;
    rax7 = fun_ae0(rdi, rsi);
    *rax7 = 0;
    rax8 = fun_bf0(0x40000);
    v9 = rax8;
    v10 = 0;
    while (v10 < 0x8000) {
        v9[v10 * 2] = v10 * 0xe10;
        rdx11 = reinterpret_cast<int32_t*>(v10 * 8);
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(v9) + reinterpret_cast<int64_t>(rdx11) + 4) = 0;
        ++v10;
    }
    rdi12 = v4->f8;
    eax13 = isFile(rdi12, rsi);
    rdi14 = v4->f16;
    eax16 = fun_c20(rdi14, rsi, rdx11, rcx15);
    v17 = eax16;
    if (eax13) {
        addr_1269_5:
        rdi18 = v4->f24;
        eax20 = fun_c20(rdi18, rsi, rdx11, rcx19);
        v21 = eax20;
        rsi22 = reinterpret_cast<struct s0*>(0x8000);
        *reinterpret_cast<int32_t*>(&rdx23) = 8;
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdx23) + 4) = 0;
        fun_b10(v9, 0x8000, 8, counter_comp);
        rdi24 = reinterpret_cast<int32_t*>("Top K frequently accessed hour:");
        fun_b00("Top K frequently accessed hour:", 0x8000, 8, counter_comp);
        v25 = 0;
    } else {
        rdi26 = v4->f8;
        rax27 = fun_b40(rdi26);
        v28 = rax27;
        if (v28) {
            while (rax29 = fun_be0(v28, rsi, rdx11, rcx19), !!rax29) {
                rsi = reinterpret_cast<struct s0*>(".");
                eax30 = fun_bb0(rax29 + 19, ".", rdx11, rcx19);
                if (eax30) {
                    rsi = reinterpret_cast<struct s0*>("..");
                    eax31 = fun_bb0(rax29 + 19, "..", rdx11, rcx19);
                    if (eax31) {
                        rdx32 = v4->f8;
                        fun_af0(reinterpret_cast<int64_t>(rbp3) - 0x110, rdx32);
                        rax33 = fun_b60(reinterpret_cast<int64_t>(rbp3) - 0x110, rdx32, rdx32, rcx19);
                        eax34 = *reinterpret_cast<unsigned char*>(reinterpret_cast<int64_t>(rbp3) + (*reinterpret_cast<int32_t*>(&rax33) - 1) - 0x110);
                        if (*reinterpret_cast<signed char*>(&eax34) != 47) {
                            *reinterpret_cast<signed char*>(reinterpret_cast<int64_t>(rbp3) + *reinterpret_cast<int32_t*>(&rax33) - 0x110) = 47;
                            *reinterpret_cast<signed char*>(reinterpret_cast<int64_t>(rbp3) + (*reinterpret_cast<int32_t*>(&rax33) + 1) - 0x110) = 0;
                        }
                        fun_c30(reinterpret_cast<int64_t>(rbp3) - 0x110, rax29 + 19);
                        rdi35 = reinterpret_cast<void*>(reinterpret_cast<int64_t>(rbp3) - 0x110);
                        rax36 = fun_c00(rdi35, "r");
                        if (!rax36) 
                            goto addr_11c7_13;
                        rax37 = v17;
                        *reinterpret_cast<int32_t*>(&rsi) = 0x8000;
                        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rsi) + 4) = 0;
                        rcx19 = rax36;
                        *reinterpret_cast<int32_t*>(&rdx11) = *reinterpret_cast<int32_t*>(&rax37);
                        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdx11) + 4) = 0;
                        read_one_file(v9, 0x8000, *reinterpret_cast<int32_t*>(&rdx11), rcx19);
                        fun_b30(rax36, 0x8000, rdx11, rcx19);
                    }
                }
            }
            goto addr_1269_5;
        } else {
            rax38 = fun_ae0(rdi26, rsi);
            *reinterpret_cast<int32_t*>(&rdi39) = *rax38;
            *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdi39) + 4) = 0;
            rax40 = fun_c50(rdi39, rsi);
            rdx23 = rax40;
            rax41 = stderr;
            rsi22 = reinterpret_cast<struct s0*>("Error : Failed to open input directory - %s\n");
            fun_bc0(rax41, "Error : Failed to open input directory - %s\n", rdx23, rcx42);
            rdi43 = v44;
            fun_b30(rdi43, "Error : Failed to open input directory - %s\n", rdx23, rcx45);
            *reinterpret_cast<int32_t*>(&rax46) = 1;
            *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rax46) + 4) = 0;
            goto addr_13dd_18;
        }
    }
    while (v25 < v21) {
        v47 = v17 + v9[v25 * 2];
        fun_c40(reinterpret_cast<int64_t>(rbp3) - 0x130, "%ld", v47, counter_comp);
        fun_c10(reinterpret_cast<int64_t>(rbp3) - 0x130, 0, 0, counter_comp);
        rax48 = fun_b50(reinterpret_cast<int64_t>(rbp3) - 0x178, 0, 0, counter_comp);
        rax49 = fun_b60(rax48, 0, 0, counter_comp);
        *reinterpret_cast<signed char*>(reinterpret_cast<int64_t>(rax48) + (reinterpret_cast<int64_t>(rax49) - 1)) = 0;
        *reinterpret_cast<int32_t*>(&rdx23) = (v9 + v25 * 2)[1];
        *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdx23) + 4) = 0;
        rsi22 = rax48;
        rdi24 = reinterpret_cast<int32_t*>("%s\t%d\n");
        fun_b80("%s\t%d\n", rsi22, rdx23, counter_comp);
        ++v25;
    }
    fun_b20(rdi24, rsi22, rdx23, counter_comp, r8_50, r9_51);
    rdi43 = v9;
    fun_ad0(rdi43, rsi22, rdx23, counter_comp);
    *reinterpret_cast<int32_t*>(&rax46) = 0;
    *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rax46) + 4) = 0;
    addr_13dd_18:
    rcx52 = v6 ^ g28;
    if (rcx52) {
        rax46 = fun_b70(rdi43, rsi22, rdx23);
    }
    return rax46;
    addr_11c7_13:
    rax53 = fun_ae0(rdi35, "r");
    *reinterpret_cast<int32_t*>(&rdi54) = *rax53;
    *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rdi54) + 4) = 0;
    rax55 = fun_c50(rdi54, "r");
    rdx23 = rax55;
    rax56 = stderr;
    rsi22 = reinterpret_cast<struct s0*>("Error : Failed to open entry file - %s\n");
    fun_bc0(rax56, "Error : Failed to open entry file - %s\n", rdx23, rcx19);
    rdi43 = rax36;
    fun_b30(rdi43, "Error : Failed to open entry file - %s\n", rdx23, rcx19);
    *reinterpret_cast<int32_t*>(&rax46) = 1;
    *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rax46) + 4) = 0;
    goto addr_13dd_18;
}

void fun_b76() {
    goto 0xac0;
}

void fun_c26() {
    goto 0xac0;
}

void fun_b46() {
    goto 0xac0;
}

void fun_ae6() {
    goto 0xac0;
}

void fun_c56() {
    goto 0xac0;
}

void fun_bc6() {
    goto 0xac0;
}

void fun_b36() {
    goto 0xac0;
}

void fun_c36() {
    goto 0xac0;
}

void fun_c06() {
    goto 0xac0;
}

void fun_b16() {
    goto 0xac0;
}

void fun_b06() {
    goto 0xac0;
}

void fun_b26() {
    goto 0xac0;
}

void fun_ad6() {
    goto 0xac0;
}

void fun_be6() {
    goto 0xac0;
}

void fun_bb6() {
    goto 0xac0;
}

void fun_af6() {
    goto 0xac0;
}

void fun_b66() {
    goto 0xac0;
}

void fun_c46() {
    goto 0xac0;
}

void fun_c16() {
    goto 0xac0;
}

void fun_b56() {
    goto 0xac0;
}

void fun_b86() {
    goto 0xac0;
}

void frame_dummy() {
    goto 0xce0;
}

void fun_ba6() {
    goto 0xac0;
}

void fun_bf6() {
    goto 0xac0;
}

void fun_bd6() {
    goto 0xac0;
}

int64_t counter_comp(struct s2* rdi, struct s3* rsi) {
    int32_t v3;
    int64_t rax4;

    v3 = rdi->f4 - rsi->f4;
    if (!v3) {
        v3 = rdi->f0 - rsi->f0;
    }
    *reinterpret_cast<int32_t*>(&rax4) = -v3;
    *reinterpret_cast<int32_t*>(reinterpret_cast<int64_t>(&rax4) + 4) = 0;
    return rax4;
}
