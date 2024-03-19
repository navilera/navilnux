#ifndef _NAVIL_DRV
#define _NAVIL_DRV

#define DRVLIMIT 100

#define O_RDONLY    0
#define O_WRONLY    1
#define O_RDWR      2

typedef struct _fops{
    int (*open)(int drvnum, int mode);
    int (*read)(int drvnum, void *buf, int size);
    int (*write)(int drvnum, void *buf, int size);
    int (*close)(int drvnum);
} fops;

typedef struct _navil_free_drv {
    fops *navil_fops;
    int usecount;
    const char *drvname;
} Navil_free_drv;

typedef struct _navil_drv_mng {
    Navil_free_drv free_drv_pool[DRVLIMIT];

    void (*init)(void);
    int (*register_drv)(int, const char*, fops*);
} Navil_drv_mng;

void drv_init(void);
int drv_register_drv(int, const char*, fops*);


// user define device drivers init function
#define MYDRV 0
int mydrv_init(void);

#endif
