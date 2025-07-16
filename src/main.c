#include "types.h"

extern void initBDS(s32 *);
extern void initGame();
extern void execBefore();
extern void execGame();
extern void execAfter();

extern s32 g_pack_data_info[6106];

void NitroMain(void) {
    initBDS(g_pack_data_info);
    initGame();

    do {
        execBefore();
        execGame();
        execAfter();
    } while (true);
}
