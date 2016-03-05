#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define COMPRESS_MAX_LAST_NAME_SIZE 16
#define BUCKET_SIZE 5120
/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
// #define OPT 1
typedef struct __PHONE_BOOK_ENTRY {
    // char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email;
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];

} detail;
typedef struct _LAST_NAME_ENTRY {
    char  lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *detaildata;
    struct _LAST_NAME_ENTRY *pNext;
} entry;
struct PHONEBOOK {
    void (*initialize_Bucket)  (void);
    void(*backtohead) (void);
    void(*release) (void);
    void(*insertnode) (char* lastname,entry *node[]);
    entry* (*findName)(char lastname[], entry *pHead[]);
    void(*printnode) (void);
};

entry *pHead [BUCKET_SIZE];
entry *e [BUCKET_SIZE];
void *append(char lastName[], entry *e[]);
struct PHONEBOOK PHONEBOOK;
#endif
