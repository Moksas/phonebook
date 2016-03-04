#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "phonebook_opt.h"

static int ASCII_ENG_CHAR_START = 96 ;//not use 97  to vaoid 0 which  present NULL in ASCII
static int COMPRESS_CHAR_BITS =5;
static int ORIGIN_CHAR_BITS =5;
static inline unsigned long hash_func_djb2(char *lastname)
{
    unsigned long key;
    int lastname_length = strlen(lastname);
    for(int i = 0 ; i < lastname_length ; i++) {
        key = ((key<<5) + key ) +lastname[i];
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
    //compress( lastname, lastname);
    char compresstmp[COMPRESS_MAX_LAST_NAME_SIZE];
    char decompresstmp[MAX_LAST_NAME_SIZE];
    compress( lastname, compresstmp);
    unsigned int key = hash_func_djb2(compresstmp)%BUCKET_SIZE;
    entry  *Return_entry= (entry *) malloc(sizeof(entry));
    entry *node = pHead[key];
    while (node != NULL) {

        if (strcasecmp(compresstmp, node->lastName) == 0) {


            decompress(node->lastName,decompresstmp);

            strcpy(Return_entry->lastName, decompresstmp);
            Return_entry->pNext=node->pNext;
            return Return_entry;
        }


        node = node->pNext;
    }
    printf("fail\n");
    return NULL;
}

void *append(char lastName[], entry *e[])
{
    char compresstmp[COMPRESS_MAX_LAST_NAME_SIZE];
    compress(lastName,compresstmp);
    unsigned int key = hash_func_djb2(compresstmp)%BUCKET_SIZE;

    // printf("%d////%s\n",key, compresstmp);
    e[key]->pNext = (entry *) malloc(sizeof(entry));
    e[key] = e[key]->pNext;
    strcpy(e[key]->lastName, compresstmp);
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
    char decompresstmp[MAX_LAST_NAME_SIZE];

    for(int i= 0; i<BUCKET_SIZE; i++) {
        while(e[i]!=NULL) {
            decompress(e[i]->lastName,decompresstmp);
            printf("node[%d]%s\n",i,decompresstmp );
            e[i]=e[i]->pNext;
            count++;
        }
    }
    printf("%d\n",count );
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
