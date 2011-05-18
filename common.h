#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

typedef struct fb_info
{
    int fd;
    struct fb_var_screeninfo fb_var;
    unsigned int w;
    unsigned int h;
    unsigned int bpp;
    void * fb_mem;
}info_t, *pinfo_t;

typedef struct 
{
    int dx;
    int dy;
    int dz;
    int button;
}mevent_t;

typedef unsigned int u32_t;
typedef unsigned char u8_t;

typedef  char s8_t;
int mouse_open(const char *mdev);
int mouse_parse(int fd,mevent_t *mevent);
int mouse_draw(const pinfo_t fb,int x,int y);
int mouse_restore(const pinfo_t fb,int x,int y);
int fb_init(pinfo_t fb);
void fb_drawpixel(pinfo_t fb,int x,int y,u32_t color);
void fb_close(pinfo_t fb);
int mouse_test(pinfo_t fb);

#define C_WIDTH     10
#define C_HEIGHT    17
#define T___        0XFFFFFFFF
#define BORD        0x0
#define X___        0xFFFF
#define READ_MOUSE  8
static U32_t cursor_pixel[C_WIDTH *C_HEIGHT] = {
    BORD,T___,T___,T___,T___,T___,T___,T___,T___,T___,
    BORD,BORD,T___,T___,T___,T___,T___,T___,T___,T___,
    BORD,X___,BORD,T___,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,BORD,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,X___,BORD,T___,T___,T___,T___,T___,
    BORD,X___,X___,X___,X___,BORD,T___,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,BORD,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,BORD,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,BORD,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,X___,BORD,
    BORD,X___,X___,X___,X___,X___,BORD,BORD,BORD,BORD,
    BORD,X___,X___,BORD,X___,X___,BORD,T___,T___,T___,
    BORD,X___,BORD,T___,BORD,X___,X___,BORD,T___,T___,
    BORD,BORD,T___,T___,BORD,X___,X___,BORD,T___,T___,
    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,
    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,
    T___,T___,T___,T___,T___,T___,BORD,BORD,T___,T___
};

static u32_t save_cursor[C_WIDTH * C_HEIGHT];
