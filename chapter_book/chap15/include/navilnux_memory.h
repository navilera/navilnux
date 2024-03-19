#ifndef _NAVIL_MEM
#define _NAVIL_MEM

#define MAXMEMBLK 40


typedef struct _navil_free_mem {
    unsigned int block_start_addr;
    unsigned int block_end_addr;
    int is_used;
} Navil_free_mem;

typedef struct _dy_mem_header {
    int used;
    int size;
} Dy_mem_header;


typedef struct _navil_mem_mng {
    Navil_free_mem free_mem_pool[MAXMEMBLK];

    unsigned int heap_start;
    unsigned int used_end;
    
    void (*init)(void);
    unsigned int (*alloc)(void);

    void* (*malloc)(int);
    int (*free)(void *);
} Navil_mem_mng;

void mem_init(void);
unsigned int mem_alloc(void);

void* mem_malloc(int);
int mem_free(void *);

#endif
