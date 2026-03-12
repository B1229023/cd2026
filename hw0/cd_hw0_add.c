/*Use a hash table (ASCII index table) to map characters directly to
their nodes in the linked list, reducing the search time from O(n)
to O(1).*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word{
    char letter;
    int num;
    struct word* next;
};

void printReverse(struct word *node){
    if(node == NULL)
        return;

    printReverse(node->next);
    printf("%c : %d\n", node->letter, node->num);
}

int main(){

    FILE *fp;
    char ch;

    fp = fopen(__FILE__, "r");
    if(fp == NULL){
        printf("file open error\n");
        return 1;
    }
    struct word *head = NULL;
    
    struct word *table[128] = {NULL};
    
    while((ch = fgetc(fp)) != EOF){
        if(ch == ' ' || ch == '\n' || ch == '\t')
            continue;
        if(table[(int)ch] != NULL){
            table[(int)ch]->num++;
        }
        else{
            
            struct word *newnode = malloc(sizeof(struct word));
            newnode->letter = ch;
            newnode->num = 1;
            newnode->next = head;
            head = newnode;
            table[(int)ch] = newnode;
        }
    }

    fclose(fp);
    printReverse(head);

    return 0;
}
