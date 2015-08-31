#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <time.h>  
#include <sha256.h>  
  
unsigned char Message[1024]="abc";  
  
//="abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";  
//{0xcc,0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11};  
  
  
int main(int argc, char *argv[])  
{  
    unsigned char H[35] = "\0";  
    int MLen = strlen(Message);  
    //MLen = sizeof(Message);
    printf("%d\n",MLen);
    int i =0;  
    int CycleNum = 100000;  
    struct timeval T1,T2;  
  
    if(argc > 1)  
    {  
        MLen = atoi(argv[1]);  
        if (MLen <= 0 || MLen > 1024)  
        {  
            printf("Input para error\n");  
            return -1;  
        }  
        if (argc > 2)  
        {  
            CycleNum = atoi(argv[2]);  
            if (CycleNum <= 0)  
            {  
                printf("Input para error\n");  
                return -1;  
            }  
        }  
    }  
  
    printf("------------------------start SHA256 test-----------------------------------\n");  
    printf("option      message length  crycle num  cost time   performance\n");  
    //gettimeofday(&T1,NULL);  
    //for(i = 0; i < CycleNum; i++)  
    {  
        if(!SHA256(Message, MLen, H))  
        {  
            printf("error\n");  
        }  
        for(i = 0; i < 32; i++)  
        printf("%02x", H[i]);  
        printf("\n");  
    }  
    /*gettimeofday(&T2,NULL);  
    printf("SHA256      %d bits %d times    %ds     %d times/s\n",   
        MLen, CycleNum, (int)(T2.tv_sec - T1.tv_sec + (T2.tv_usec - T1.tv_usec)/1000000),  
        CycleNum/(int)(T2.tv_sec - T1.tv_sec + (T2.tv_usec - T1.tv_usec)/1000000)); */ 
    return 0;  
  
}  
