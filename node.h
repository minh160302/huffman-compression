#ifndef NODE_H_
#define NODE_H_

struct Node {
        int freq;
        char data;
        struct Node *next, *left, *right;
};

//typedef struct Node Node;

#endif
