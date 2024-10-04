#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>

#include "node.h"
#include "share.h"

typedef struct Node Node;

void freeLL(Node *p){
        Node* tmp;
        while (p != NULL) {
                tmp = p;
                p = p->next;
                free(tmp);
        }
}

/*
 * Free memory of linked list
 */
void freeTree(Node *p){
        if (p != NULL) {
                Node *tmpLeft = p->left;
                Node *tmpRight = p->right;

                if (p->left != NULL)
                        freeTree(tmpLeft);

                if (p->right != NULL)
                        freeTree(tmpRight);
			
                free(p);
        }
}


char* decToBinary(int num) {
        //      Malloc of size CHAR_BIT + 1 because of the null terminator      //
        char *bin = (char*) malloc((CHAR_BIT+1) * sizeof(char));
        memset(bin, 0, CHAR_BIT);

        for (int i = 0; i < CHAR_BIT; i++) {
                char* tmp = strdup(bin);
                char* x = (char*) malloc(CHAR_BIT * sizeof(char));
                char* digit;
                if (num == 0) {
                        digit = "0";
                }
                else {
                        if (num % 2 == 0)
                                digit = "0";
                        else
                                digit = "1";
                        num = num / 2;
                }

                memset(x, 0, CHAR_BIT);
                strcat(x, digit);
                strcpy(bin, x);
                strcat(bin, tmp);

                free(tmp);
                free(x);
        }

        return bin;
}
