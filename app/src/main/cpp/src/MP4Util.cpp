#include "MP4Util.h"

int findBox(const char *vidname,MP4Box *box,int offset)
{
    FILE *vfp=fopen(vidname,"rb");
    if(NULL==vfp){
        printf("[%s-%d]:Open %s failed !\n",__func__,__LINE__,vidname);
        return -1;
    }
    fseek(vfp,0,SEEK_END);
    VSize_t flen=ftell(vfp);
    fseek(vfp,offset,SEEK_SET);
    flen-=offset;

    VSize_t rlen=0;
    int ret=0;
    while(rlen<flen){
        MP4Box boxtmp;
        if(8!=fread((unsigned char *)&boxtmp,1,8,vfp)){
            printf("[%s-%d]:Read %s error !\n",__func__,__LINE__,vidname);
            ret=-1;
            break;
        }
        char srcname[5];
        char dstname[5];
        memcpy(srcname,boxtmp.name,4);
        srcname[4]=0;
        memcpy(dstname,box->name,4);
        dstname[4]=0;
        //printf("rlen=%x\n",rlen);
        //int t=0;
        //unsigned char *ppp=(unsigned char *)&boxtmp;
        //for(t=0;t<8;t++){
        //    printf("%02x ",ppp[t]);
        //}
        //printf("\n");
        VSize_t boxLen=SWAP_EDIAN_U32(boxtmp.len);
        if(boxLen<8){
            printf("[%s-%d]:%s file damage\n",__func__,__LINE__,vidname);
            ret=-1;
            break;
        }
        if(0==strcmp(srcname,dstname)){
            memcpy((unsigned char *)box,(unsigned char *)&boxtmp,8);
            if(rlen+boxLen>flen){
                ret=0;
                break;
            }
            box->offset=ftell(vfp)-8;
            ret=1;
            break;
        }
        else{
            if(rlen+boxLen>flen){
                printf("[%s-%d]:Box length beyond file !\n",__func__,__LINE__);
                ret=0;
                break;
            }
            fseek(vfp,boxLen-8,SEEK_CUR);
            rlen+=boxLen;
        }
    }
    fclose(vfp);
    return ret;
}

void changeStco(unsigned char *buf,int len,int chunkDeta)
{
    int i=0;
    unsigned int *pchunkOffset=0;
    for(i=16;i<len;i+=4){
        pchunkOffset=(unsigned int *)(buf+i);
        *pchunkOffset=SWAP_EDIAN_U32(SWAP_EDIAN_U32(*pchunkOffset)+chunkDeta);
    }
}

int move(int argc,char **argv)
{
    if(3!=argc){
        printf("Usage:\n");
        printf("mp4ThumbPick test.mp4 out.mp4");
        return -1;
    }

    MP4Box matBox,moovBox,stcoBox;
    {
        char dstName[4]={'m','d','a','t'};
        memcpy(matBox.name,dstName,4);
        if(findBox(argv[1],&matBox,0)>0){
            printf("mdat offset:0x%08x\n",matBox.offset);
        }
    }
    {
        char dstName[4]={'m','o','o','v'};
        memcpy(moovBox.name,dstName,4);
        if(findBox(argv[1],&moovBox,0)>0){
            printf("moov offset:0x%08x\n",moovBox.offset);
        }
    }
    {
        MP4Box box;
        char dstName[4]={'t','r','a','k'};
        memcpy(box.name,dstName,4);
        if(findBox(argv[1],&box,moovBox.offset+8)<=0){
            printf("trak box not founc\n");
            return -1;
        }
        char dstName1[4]={'m','d','i','a'};
        memcpy(box.name,dstName1,4);
        if(findBox(argv[1],&box,box.offset+8)<=0){
            printf("mdia box not founc\n");
            return -1;
        }
        char dstName2[4]={'m','i','n','f'};
        memcpy(box.name,dstName2,4);
        if(findBox(argv[1],&box,box.offset+8)<=0){
            printf("minf box not founc\n");
            return -1;
        }
        char dstName3[4]={'s','t','b','l'};
        memcpy(box.name,dstName3,4);
        if(findBox(argv[1],&box,box.offset+8)<=0){
            printf("stbl box not founc\n");
            return -1;
        }
        char dstName4[4]={'s','t','c','o'};
        memcpy(box.name,dstName4,4);
        if(findBox(argv[1],&box,box.offset+8)<=0){
            printf("stco box not founc\n");
            return -1;
        }
        stcoBox=box;
    }

    int matLen=SWAP_EDIAN_U32(matBox.len);
    int moovLen=SWAP_EDIAN_U32(moovBox.len);
    int stcoLen=SWAP_EDIAN_U32(stcoBox.len);
    printf("matBox.offset:%d,matBox.len=%d\n",matBox.offset,matLen);
    printf("moovBox.offset:%d,moovBox.len=%d\n",moovBox.offset,moovLen);
    printf("stcoBox.offset:%d,stcoBox.len=%d\n",stcoBox.offset,stcoLen);

    FILE *infp=fopen(argv[1],"rb");
    FILE *outfp=fopen(argv[2],"wb");
    if(NULL==infp||NULL==outfp){
        printf("Open %s/%s error!\n",argv[1],argv[2]);
    }
    fseek(infp,0,SEEK_END);
    int flen=ftell(infp);
    fseek(infp,0,SEEK_SET);
    unsigned char *buf=(unsigned char *)malloc(matBox.offset);
    fread(buf,1,matBox.offset,infp);
    fwrite(buf,1,matBox.offset,outfp);
    free(buf);
    //save moov
    buf=(unsigned char *)malloc(moovLen);
    fseek(infp,moovBox.offset,SEEK_SET);
    fread(buf,1,moovLen,infp);
    changeStco(buf+stcoBox.offset-moovBox.offset,stcoLen,moovLen);
    fwrite(buf,1,moovLen,outfp);
    free(buf);
    //save mdat
    int rrlen=0,rlen=0;
    buf=(unsigned char *)malloc(1024);
    fseek(infp,matBox.offset,SEEK_SET);
    while(rlen<matLen){
        if(rlen+1024<=matLen){
            rrlen=fread(buf,1,1024,infp);
            fwrite(buf,1,1024,outfp);
            rlen+=rrlen;
        }
        else{
            rrlen=fread(buf,1,matLen-rlen,infp);
            fwrite(buf,1,matLen-rlen,outfp);
            break;
        }
    }
    free(buf);
    //after moov
    if(moovBox.offset+moovLen<flen){
        int sparelen=flen-(moovBox.offset+moovLen);
        buf=(unsigned char *)malloc(sparelen);
        fseek(infp,moovBox.offset+moovLen,SEEK_SET);
        fread(buf,1,sparelen,infp);
        fwrite(buf,1,sparelen,outfp);
        free(buf);
    }
    fclose(infp);
    fclose(outfp);
    return 0;
}
