#include <navilnux.h>

Navil_mem_mng memmng;

#define STARTUSRSTACKADDR   0xA0400000  // 4M
#define ENDUSRSTACKADDR     0xA4000000  // 64M
#define USRSTACKSIZE        0x00100000  // 1M

#define STARTDYMEMADDR      0xA2C00000  /// 44M

void* mem_malloc(int size)
{
    int req_size = size + sizeof(Dy_mem_header);
    unsigned int cur_pos = memmng.heap_start;
    unsigned int ret_addr = 0;

    Dy_mem_header *dy_header;

    while(cur_pos != memmng.used_end){
        dy_header = (Dy_mem_header *)cur_pos;
        if(!dy_header->used){
            if(dy_header->size >= req_size){
                dy_header->used = 1;
                ret_addr = cur_pos;
                break;
            }
        }
        cur_pos += dy_header->size;
    }

    if(!ret_addr){
        ret_addr = memmng.used_end;
        memmng.used_end += req_size;

        if(memmng.used_end > ENDUSRSTACKADDR){
            return (void *)0;
        }

        dy_header = (Dy_mem_header *)ret_addr;
        dy_header->used = 1;
        dy_header->size = req_size;
    }

    ret_addr += sizeof(Dy_mem_header);

    return (void *)ret_addr;
}

int mem_free(void *addr)
{
    Dy_mem_header *dy_header;

    unsigned int add_header = (unsigned int)addr - sizeof(Dy_mem_header);
    dy_header = (Dy_mem_header *)add_header;

    if(dy_header->used != 1){
        return -1;
    }

    dy_header->used = 0;

    return 0;
}

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

    memmng.heap_start = STARTDYMEMADDR;
    memmng.used_end = memmng.heap_start;

    memmng.init = mem_init;
    memmng.alloc = mem_alloc;

    memmng.malloc = mem_malloc;
    memmng.free = mem_free;
}
