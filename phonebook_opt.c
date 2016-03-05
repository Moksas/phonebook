#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "phonebook_opt.h"

static int ASCII_ENG_CHAR_START = 96 ;//not use 97  to vaoid 0 which  present NULL in ASCII
static int COMPRESS_CHAR_BITS =5;
static int ORIGIN_CHAR_BITS =8;
static inline unsigned long hash_func_djb2(char *lastname)
{
    unsigned long key=5381;

    int lastname_length = strlen(lastname);
    for(int i = 0 ; i < lastname_length ; i++) {
        key = ((key<<5) + key ) ^ lastname[i];
    }
    return key;
}

static inline void compress(char *input,char *output )
{
    bool charbinary[80];///5* MAX_LAST_NAME_SIZE( 16)
    int count=0;
    int tempchar;
    int inputsize = strlen(input);
    int totalcompress_size = (inputsize<<2)+inputsize;
    //printf("size of bytes : %lu bytes\n", sizeof(charbinary[0]));
    //printf("size of char : %lu bytes\n", sizeof(input[0]));

    for(int i = 0 ; i < inputsize ; i++) {
        if( input[i] == '\0' ) break;
        tempchar = input[i]-ASCII_ENG_CHAR_START;

        for (int j = COMPRESS_CHAR_BITS - 1; j >= 0 ; j--) {
            charbinary[count++] = tempchar & (1<<j);//if the bit is no zero the charbinary set 1

        }

    }

    count=ORIGIN_CHAR_BITS-1;//8

    int charsum=0;
    int index = 0;
    for (int i = 0; i < totalcompress_size; i++) {
        charsum+=charbinary[ i ]<<(count--);
        if(count < 0) {
            output[index++] =charsum;
            count=ORIGIN_CHAR_BITS-1;
            charsum=0;
        }
    }
    if(count!=(ORIGIN_CHAR_BITS-1))//if totalcompress_size%8 ! =0
        output[index++] =charsum;
    output[index] = '\0';//end of char array
}
static inline void decompress(char *input,char *output )
{
    bool charbinary[80];///5* MAX_LAST_NAME_SIZE( 16)
    int count=0;

    int inputsize = strlen(input);

    //printf("size of bytes : %lu bytes\n", sizeof(charbinary[0]));
    //printf("size of char : %lu bytes\n", sizeof(input[0]));
    for(int i = 0 ; i < inputsize ; i++) {
        if( input[i] == '\0' ) break;
        for (int j = ORIGIN_CHAR_BITS - 1; j >= 0 ; j--) {
            charbinary[count++] = input[i] & (1<<j);//if the bit is no zero the charbinary set 1

        }
    }


    int charsum=0;
    int  tempcount = 0;
    int index =0;
    while (tempcount<count) {
        for( int j = COMPRESS_CHAR_BITS-1 ; j >= 0 ; j --)
            charsum += charbinary[tempcount++] << j;
        if(charsum!=0)
            output[index++] = charsum + ASCII_ENG_CHAR_START;
        charsum=0;
    }
    output[index] = '\0';//end of char array
}
/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry *pHead[])
{
    /* TODO: implement */

    unsigned int key = hash_func_djb2(lastname)%BUCKET_SIZE;

    entry *node = pHead[key];
    while (node != NULL) {

        if (strcasecmp(lastname, node->lastName) == 0) {


            return node;
        }


        node = node->pNext;
    }
    printf("fail\n");
    return NULL;
}

void *append(char lastName[], entry *e[])
{

    unsigned int key = hash_func_djb2(lastName)%BUCKET_SIZE;
    e[key]->pNext = (entry *) malloc(sizeof(entry));
    e[key] = e[key]->pNext;
    strcpy(e[key]->lastName, lastName);
    e[key]->pNext = NULL;
    return NULL;
}
void initialize_Bucket()
{
    for(int i = 0 ; i < BUCKET_SIZE ; i++) {
        pHead[i] = (entry*)malloc(sizeof(entry));
        pHead[i]->pNext = NULL;
        e[i] = pHead[i] ;
    }
}
void insertnode(char* lastname,entry *node[])
{
    append(lastname,node);

}
void printnode()
{
    int count=0;
    int NULL_count=0;
    for(int i= 0; i<BUCKET_SIZE; i++) {
        if(e[i]==NULL) {
            printf("%d bucket is NULL\n",i );
            NULL_count ++;
        }
        while(e[i]!=NULL) {
            //   printf("node[%d]%s\n",i,e[i]->lastName );
            e[i]=e[i]->pNext;
            count++;
        }
        printf("%d bucket:%d\n",i,count );
        count=0;
    }
    printf(" number of %d bucket is NULL\n",NULL_count );
}
void backtohead()
{
    for(int i = 0 ; i < BUCKET_SIZE ; i++) {
        e[i] = pHead[i] ;
    }
}
void release()
{
    for (int i = 0; i < BUCKET_SIZE; i++) {
        free(pHead[i]);
    }
}
struct PHONEBOOK PHONEBOOK= {
    .initialize_Bucket = initialize_Bucket,
    .release = release,
    .backtohead = backtohead,
    .insertnode =insertnode,
    .findName =findName,
    .printnode = printnode
};
