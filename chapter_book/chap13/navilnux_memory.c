#include <navilnux.h>

Navil_mem_mng memmng;

#define STARTUSRSTACKADDR   0xA0400000  // 4M
#define USRSTACKSIZE        0x00100000  // 1M

unsigned int mem_alloc(void)
{
    int i;
    for(i = 0 ; i < MAXMEMBLK ; i++){
        if(memmng.free_mem_pool[i].is_used == 0){
            memmng.free_mem_pool[i].is_used = 1;
            return memmng.free_mem_pool[i].block_end_addr;
        }
    }
    return 0;
}

void mem_init(void)
{
    unsigned int pt = STARTUSRSTACKADDR;
    int i;

    for(i = 0 ; i < MAXMEMBLK ; i++){
        memmng.free_mem_pool[i].block_start_addr = pt;
        memmng.free_mem_pool[i].block_end_addr = pt + USRSTACKSIZE -4;
        memmng.free_mem_pool[i].is_used = 0;
        pt += USRSTACKSIZE;
    }

    memmng.init = mem_init;
    memmng.alloc = mem_alloc;
}
