#ifndef _NAVIL_MEM
#define _NAVIL_MEM

#define MAXMEMBLK 40


typedef struct _navil_free_mem {
    unsigned int block_start_addr;
    unsigned int block_end_addr;
    int is_used;
} Navil_free_mem;

typedef struct _navil_mem_mng {
    Navil_free_mem free_mem_pool[MAXMEMBLK];

    void (*init)(void);
    unsigned int (*alloc)(void);
} Navil_mem_mng;

void mem_init(void);
unsigned int mem_alloc(void);

#endif
