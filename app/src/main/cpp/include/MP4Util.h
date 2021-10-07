#ifndef MEDIATOOLS_MP4UTIL_H
#define MEDIATOOLS_MP4UTIL_H


#include <stdio.h>
#include <malloc.h>
#include <string.h>

#if _WIN32
#define __func__ __FUNCTION__
#endif

#define SWAP_EDIAN_U32(X) (((0x000000ff&(X))<<24)|((0x0000ff00&(X))<<8)|((0x00ff0000&(X))>>8)|((0xff000000&(X))>>24))

typedef unsigned long long int VSize_t;


typedef struct {
    unsigned int len;
    char name[4];
    unsigned int offset;
} MP4Box;

int findBox(const char *vidname, MP4Box *box, int offset);

void changeStco(unsigned char *buf, int len, int chunkDeta);

int move(int argc, char **argv);

#endif //MEDIATOOLS_MP4UTIL_H
