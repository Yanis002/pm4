#include "types.h"

extern void execBefore();
extern void execAfter();
extern void func_02000d44();
extern void func_02000d68();
extern void initInterrupt();
extern void func_02000de4();
extern void func_02001074();
extern void func_02001148();
extern void func_020011c8();
extern void func_020011f4();
extern void initVRAM();
extern void func_020015c8();
extern void func_02001634();
extern void func_02001660();
extern void func_02001b24();
extern void func_02001b94();
extern void func_02001cf4();
extern void func_02001f74();
extern void func_02002124();
extern void func_020022d0();
extern void func_02002454();
extern void func_02002474();
extern void func_020025bc();
extern void func_02002624();
extern void func_020028b8();
extern void func_02002afc();
extern void func_02002b64();
extern void func_020031f0();
extern void func_02003218();
extern void func_02003240();
extern void func_02003250();
extern void func_0200344c();
extern void func_02003478();
extern void func_02003578();
extern void func_02003780();
extern void func_020037ec();
extern void func_0200385c();
extern void func_020038b8();
extern void func_0200391c();
extern void func_02003a8c();
extern void func_02003ae8();
extern void func_02003b4c();
extern void func_02003de4();
extern void func_02003df8();
extern void func_02004558();
extern void func_02004cdc();
extern void func_02004d58();
extern void func_02004db8();
extern void func_02004e44();
extern void func_02004f04();
extern void func_02004ff8();
extern void func_02005098();
extern void func_020052c4();
extern void func_02005394();
extern void func_020053d8();
extern void func_02005528();
extern void func_0200562c();
extern void func_02005674();
extern void func_0200571c();
extern void func_020057d8();
extern void func_0200584c();
extern void func_0200595c();
extern void func_02005a08();
extern void func_02005ac0();
extern void func_02005c04();
extern void func_02005d4c();
extern void func_02005e34();
extern void func_02005fb8();
extern void func_020060f4();
extern void func_02006234();
extern void func_020062f8();
extern void func_02006370();
extern void initPad();
extern void func_02006418();
extern void func_02006450();
extern void func_02006700();
extern void func_02006710();
extern void func_02006720();
extern void func_02006730();
extern void func_02006740();
extern void func_02006764();
extern void func_02006828();
extern void func_02006d6c();
extern void func_02006de8();
extern void func_02006e10();
extern void func_02006e24();
extern void func_02006e34();
extern void func_02006e44();
extern void func_02006e54();
extern void func_02006e70();
extern void func_02006e80();
extern void func_02006e90();
extern void func_020071ac();
extern void func_0200728c();
extern void func_0200736c();
extern void func_02007568();
extern void func_02007594();
extern void func_020075b4();
extern void initFileSystem();
extern void func_02007670();
extern void func_02007674();
extern void func_02007718();
extern void func_02007810();
extern void func_020078bc();
extern void func_02007b48();
extern void func_02007dc0();
extern void func_02007e14();
extern void initMemory();
extern void func_02007edc();
extern void func_02007ef4();
extern void func_02007f10();
extern void func_02007f20();
extern void memcpyBDS();
extern void func_02007f48();
extern void func_02007f54();
extern void func_02007f78();
extern void func_02007fa0();
extern void initSound();
extern void func_020082f8();
extern void func_02008320();
extern void func_02008340();
extern void func_0200836c();
extern void func_020084c0();
extern void func_02008588();
extern void func_02008664();
extern void func_020086bc();
extern void func_020086fc();
extern void func_02008728();
extern void func_02008754();
extern void func_0200878c();
extern void func_020087c4();
extern void func_02008808();
extern void func_0200884c();
extern void func_02008878();
extern void func_0200892c();
extern void func_02008984();
extern void func_020089b4();
extern void func_020089fc();
extern void func_02008c18();
extern void func_02008c80();
extern void func_02008c94();
extern void func_02008ca4();
extern void func_02008cc8();
extern void func_02008dcc();
extern void func_02008dfc();
extern void func_02008e24();
extern void func_02008e80();
extern void func_02008ef4();
extern void func_02008f08();
extern void func_02008f14();
extern void func_02008f20();
extern void func_020093d8();
extern void initMovie();
extern void func_020095e8();
extern void func_02009614();
extern void func_02009644();
extern void func_0200975c();
extern void func_0200976c();
extern void func_0200977c();
extern void func_0200978c();
extern void initRandom();
extern void func_02009858();
extern void initCard();
extern void func_020099d8();
extern void func_02009a44();
extern void func_02009b84();
extern void func_02009cdc();
extern void getNowTime();

#define POWER_CNT (*(u16 *) 0x04000304)
#define REG_B_DISPCNT (*(u32 *) 0x04001000)
#define REG_IME (*(u16 *) 0x04000208)

#define GX_SetPower()            \
    {                            \
        POWER_CNT &= 0xFFFFFDF1; \
    }
#define GXS_DispOff()              \
    {                              \
        REG_B_DISPCNT &= ~0x10000; \
    }
#define OS_EnableIrq() \
    {                  \
        REG_IME = 1;   \
    }

static s32 *g_pack_data_info_adrs;

void initBDS(s32 *in_pack_data_info) {
    g_pack_data_info_adrs = in_pack_data_info;
    OS_Init();
    FX_Init();
    GX_SetPower();
    GX_Init();
    OS_InitTick();
    OS_InitAlarm();
    GX_DispOff();
    GXS_DispOff();
    FS_Init(3);
    SND_Init();
    TP_Init();
    initMemory();
    initVRAM();
    initFileSystem();
    initPad();
    initCard();
    initInterrupt();
    initSound();
    initMovie();
    OS_SetIrqFunction(1, func_02000de4);
    OS_SetIrqFunction(2, func_02001074);
    OS_EnableIrqMask(3);
    OS_EnableIrq();
    GX_VBlankIntr(true);
    GX_HBlankIntr(true);
    RTC_Init();
    initRandom();
    OS_WaitVBlankIntr();
}

void execBefore() {}
void execAfter() {}
void func_02000d44() {}
void func_02000d68() {}
void initInterrupt() {}
void func_02000de4() {}
void func_02001074() {}
void func_02001148() {}
void func_020011c8() {}
void func_020011f4() {}
void initVRAM() {}
void func_020015c8() {}
void func_02001634() {}
void func_02001660() {}
void func_02001b24() {}
void func_02001b94() {}
void func_02001cf4() {}
void func_02001f74() {}
void func_02002124() {}
void func_020022d0() {}
void func_02002454() {}
void func_02002474() {}
void func_020025bc() {}
void func_02002624() {}
void func_020028b8() {}
void func_02002afc() {}
void func_02002b64() {}
void func_020031f0() {}
void func_02003218() {}
void func_02003240() {}
void func_02003250() {}
void func_0200344c() {}
void func_02003478() {}
void func_02003578() {}
void func_02003780() {}
void func_020037ec() {}
void func_0200385c() {}
void func_020038b8() {}
void func_0200391c() {}
void func_02003a8c() {}
void func_02003ae8() {}
void func_02003b4c() {}
void func_02003de4() {}
void func_02003df8() {}
void func_02004558() {}
void func_02004cdc() {}
void func_02004d58() {}
void func_02004db8() {}
void func_02004e44() {}
void func_02004f04() {}
void func_02004ff8() {}
void func_02005098() {}
void func_020052c4() {}
void func_02005394() {}
void func_020053d8() {}
void func_02005528() {}
void func_0200562c() {}
void func_02005674() {}
void func_0200571c() {}
void func_020057d8() {}
void func_0200584c() {}
void func_0200595c() {}
void func_02005a08() {}
void func_02005ac0() {}
void func_02005c04() {}
void func_02005d4c() {}
void func_02005e34() {}
void func_02005fb8() {}
void func_020060f4() {}
void func_02006234() {}
void func_020062f8() {}
void func_02006370() {}
void initPad() {}
void func_02006418() {}
void func_02006450() {}
void func_02006700() {}
void func_02006710() {}
void func_02006720() {}
void func_02006730() {}
void func_02006740() {}
void func_02006764() {}
void func_02006828() {}
void func_02006d6c() {}
void func_02006de8() {}
void func_02006e10() {}
void func_02006e24() {}
void func_02006e34() {}
void func_02006e44() {}
void func_02006e54() {}
void func_02006e70() {}
void func_02006e80() {}
void func_02006e90() {}
void func_020071ac() {}
void func_0200728c() {}
void func_0200736c() {}
void func_02007568() {}
void func_02007594() {}
void func_020075b4() {}
void initFileSystem() {}
void func_02007670() {}
void func_02007674() {}
void func_02007718() {}
void func_02007810() {}
void func_020078bc() {}
void func_02007b48() {}
void func_02007dc0() {}
void func_02007e14() {}
void initMemory() {}
void func_02007edc() {}
void func_02007ef4() {}
void func_02007f10() {}
void func_02007f20() {}
void memcpyBDS() {}
void func_02007f48() {}
void func_02007f54() {}
void func_02007f78() {}
void func_02007fa0() {}
void initSound() {}
void func_020082f8() {}
void func_02008320() {}
void func_02008340() {}
void func_0200836c() {}
void func_020084c0() {}
void func_02008588() {}
void func_02008664() {}
void func_020086bc() {}
void func_020086fc() {}
void func_02008728() {}
void func_02008754() {}
void func_0200878c() {}
void func_020087c4() {}
void func_02008808() {}
void func_0200884c() {}
void func_02008878() {}
void func_0200892c() {}
void func_02008984() {}
void func_020089b4() {}
void func_020089fc() {}
void func_02008c18() {}
void func_02008c80() {}
void func_02008c94() {}
void func_02008ca4() {}
void func_02008cc8() {}
void func_02008dcc() {}
void func_02008dfc() {}
void func_02008e24() {}
void func_02008e80() {}
void func_02008ef4() {}
void func_02008f08() {}
void func_02008f14() {}
void func_02008f20() {}
void func_020093d8() {}
void initMovie() {}
void func_020095e8() {}
void func_02009614() {}
void func_02009644() {}
void func_0200975c() {}
void func_0200976c() {}
void func_0200977c() {}
void func_0200978c() {}
void initRandom() {}
void func_02009858() {}
void initCard() {}
void func_020099d8() {}
void func_02009a44() {}
void func_02009b84() {}
void func_02009cdc() {}
void getNowTime() {}
