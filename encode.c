// Minh Nguyen

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include<stdbool.h>

#include "share.h"

/*
 * share.h contains:
 * 	struct Node
 * 	freeLL
 */
typedef struct Node Node;

/*
 * Global variable
 */
// Total number of binary digits
int digitCount = 0;

// Initial input bucket
int bucket[257];

// Store the traversal order in Huffman tree of each character
char* traversalBucket[257];



/*
 * Print linked list (testing)
 */
void printLL(Node *p) {
	Node *current = p;
	while (current != NULL) {
		if (current->data == '&')
			printf("%d -> ", current->freq);
		else if (current->data == '\n')
			printf("\\ -> ");
		else
			printf("%c -> ", current->data);

		if (current->left != NULL)
			printf("(%c - %c) -> ", current->left->data, current->right->data);
                current = current->next;
        }
        printf("END\n");
}


/*
 * Insertion Sort on linked list
 */
void sort(Node* current, Node** sorted, int hash) {
	bool cmp;
	if (hash == 0 && *sorted != NULL)
		cmp = (*sorted)->freq >= current->freq;
	else if (hash == 1 && *sorted != NULL)
		cmp = (*sorted)->freq > current->freq;

	if (*sorted == NULL || cmp) {
		current->next = *sorted;
		*sorted = current;
	}
	else {
		Node *dumbSorted = *sorted;
		while (dumbSorted->next != NULL && dumbSorted->next->freq < current->freq) {
			dumbSorted = dumbSorted->next;
		}
		current->next = dumbSorted->next;
		dumbSorted->next = current;
	}
}


/*
 * hash = 0: setup linked list
 * 	= 1: resort the linked list
 */
void insertionSort(Node **p, int hash) {
	Node *sorted = NULL;
	Node *current = *p;
	while (current != NULL) {
		Node *next = current->next;
		sort(current, &sorted, hash);
		current = next;
	}

	*p = sorted;
}


void insertInOrder(Node* current, Node** sorted) {
	if (*sorted == NULL) {
		*sorted = current;
		return;
	}
	Node *dumbSorted = *sorted;
	while (dumbSorted->next != NULL && dumbSorted->next->freq < current->freq) {
		dumbSorted = dumbSorted->next;
	}
	current->next = dumbSorted->next;
        dumbSorted->next = current;
}


/*
 * Preorder Traversal (Internal & Leaf node)
 */
void preorder(Node *p, char **order) {
	// Leaf node
	if (p->left == NULL && p->right == NULL) {
		digitCount++;
		strcat(*order, "1");
		int dec = (int) p->data;
		if (p->data == EOF) {
			dec = 255;
		}
		char* bin = decToBinary(dec);
		strcat(*order, bin);
		// 8 bits
		digitCount += CHAR_BIT;
//		strcat(*order, " ");
		free(bin);
		return;
	}
	// Internal node
	else {
		digitCount++;
		strcat(*order, "0");
		if (p-> left != NULL) 
			preorder(p->left, order);
		if (p->right != NULL)
			preorder(p->right, order);
	}
}


/*
 * Preorder Traversal (Left & Right)
 */
char* addChar(char* current, char* d) {
	strcat(current, d);
	return current;
}


//char* tmplr;
char* trash;
void preorderLR(Node *p, char* current) {
	if (p->left == NULL && p->right == NULL) {
		char data = p->data;
		int index;
		if (data == EOF) {
			index = 0;
		}
		else {
			index = ((int) data) + 1;
		}
		trash = strdup(current);
		traversalBucket[index] = trash;

		free(current);
//		free(trash);
		return;
	}
	else {
		char* tmplr = (char*) malloc((257*2+1) * sizeof(char));
	       	memset(tmplr, 0, 257*2+1);
		strcpy(tmplr, current);
		strcat(tmplr, "0");
		
//		printf("tmp %s\n: ", tmplr);	
//		printf("current: %s\n", current);	

		preorderLR(p->left, tmplr);

		strcat(current, "1");
		preorderLR(p->right, current);

//		free(tmplr);
	}
}


/*
 * Encode bitwise
 */
void bitwise(char* order, FILE *fp_huf) {
	char* t;
	int cnt = 0;
	int bitwiseStr[CHAR_BIT];
	for (t = order; *t != '\0'; t++) {
		char c = *t;
		bitwiseStr[cnt++] = c - '0';
		if (cnt == CHAR_BIT) {	
			int result = 0;
			int base = 1;
			for (int i = CHAR_BIT - 1; i >= 0; i--) {
				result += bitwiseStr[i] * base;
				base *= 2;
			}

			fputc(result, fp_huf);
/*
			char ch = result;
			char toAppend[2];
			toAppend[0] = ch;
			toAppend[1] = '\0';
			strcat(*encoded, toAppend);
			// Reset variable
*/
			cnt = 0;
		}
	}
}



int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("The program need to specify an input file!\n");
		exit(1);
	}
	char* inputFile = argv[1];
	char* outputFile = NULL;
	if (argc == 3) {
		outputFile = argv[2];
	}

	// Read file
	FILE *fp = fopen(inputFile, "r");
	if (fp == NULL) 
		return 0;
	

	// Set all elements to 0
	memset(bucket, 0, sizeof(bucket));


	// Read file 1st time.
	while (1) {
		char c = fgetc(fp);

		// EOF has frequency of 1
		if (feof(fp)) {
			bucket[0] = 1;
			break;
		}

		int asciiC = (int) c;
		bucket[asciiC + 1] += 1;
	}
	
	fclose(fp);
	
	Node *head = NULL;
	Node *dumbHead = NULL;

	// Add EOF to the head of LL
       	head = (Node*) malloc(sizeof(Node));
	head->freq = 1;
	head->data = EOF;
	head->next = NULL;
	head->left = NULL;
	head->right = NULL;
	dumbHead = head;

	// Create the linked list
	for (int i = 1; i <= 256; i++) {
		if (bucket[i] != 0) {
			Node *nextNode = (Node*) malloc(sizeof(Node));
			nextNode->freq = bucket[i];
			char ch = (char) i - 1;
			nextNode->data = ch;
			/* Must initialize all these null values to avoid valgrind errors */
			nextNode->left = NULL;
			nextNode->right = NULL;
			nextNode->next = NULL;

			dumbHead->next = nextNode;
			dumbHead = dumbHead->next;
		}
	}

	insertionSort(&head, 0);

//	printf("Original Linked list: ");
//	printLL(head);


	// Convert to Huffman trie
	while (head->next != NULL) {
		// First node: copy the head node
		Node* first = (Node*) malloc(sizeof(Node));
		first->next = NULL;
		first->freq = head->freq;
		first->data = head->data;
		first->left = head->left;
		first->right = head->right;
		
		// Second node
		Node *second = (Node*) malloc(sizeof(Node));
		second->next = NULL;
                second->freq = head->next->freq;
                second->data = head->next->data;
                second->left = head->next->left;
                second->right = head->next->right;
		

		// Free the first 2 nodes
		Node* tmpH = head;
		head = head->next->next;
		free(tmpH->next);
		free(tmpH);


		// Create parent node
		Node *parent = (Node*) malloc(sizeof(Node));
		parent->data = '&';
		parent->freq = first->freq + second->freq;
		parent->left = first;
		parent->right = second;
		parent->next = NULL;
	

//		// Add parent back to LL and sort afterwards.
//		parent->next = head->next->next;
//		head = parent;
//		insertionSort(&head, 1);

//		if (head->next->next != NULL) {
//		head = head->next->next;
		insertInOrder(parent, &head);

//		printf("---------- \n");
//		printLL(head);
	}


	char* order = NULL;
	order = (char*) malloc(100*100 * sizeof(char));

	// add Huffman tree
	memset(order, 0, 100*100);
	preorder(head, &order);


	char* currentSeq = (char*) malloc((257*2+1) * sizeof(char));
	memset(currentSeq, 0, 257*2 + 1);
//	printLL(head);
	preorderLR(head, currentSeq);


	// add EOF
	strcat(order, traversalBucket[0]);
//	printf("BEFORE: %d\n", digitCount);i
	digitCount += strlen(traversalBucket[0]);
//	printf("AFTER: %d\n", digitCount);
//	strcat(order, "\n");
//	printf("EOF order: %s\n", traversalBucket[0]);

	// Read file 2nd time
	FILE *fp2 = fopen(inputFile, "r");

        // Read file 2nd time.
        while (1) {
                char c = fgetc(fp2);

                // EOF has frequency of 1
                if (feof(fp2)) {
			strcat(order, traversalBucket[0]);
			digitCount += strlen(traversalBucket[0]);
//			strcat(order, "\n");
			break;
                }

		int index = ((int) c) + 1;

//printf("%c  -- %d  --- %s -- strlen: %d\n", c,  index, traversalBucket[index], strlen(traversalBucket[index]));
		strcat(order, traversalBucket[index]);
		digitCount += strlen(traversalBucket[index]);
//		strcat(order, " ");
        }
	fclose(fp2);


	// Add remainder 0 to get full padding
	while (digitCount % 8 != 0) {
		strcat(order, "0");
		digitCount++;
	}

	//printf("%s\n", order);	

	FILE *fp_huf;
	if (outputFile != NULL)
		fp_huf = fopen(outputFile, "wb");
	else
		fp_huf = stdout;

	// Encode binary to bitwise character
	bitwise(order, fp_huf);

	freeTree(head);
	free(order);

	

	// Free the traversal bucket
	for (int i = 0; i <= 256; i++) {
		if (traversalBucket[i] != NULL)
			free(traversalBucket[i]);
	}
	

	if (outputFile != NULL)
		fclose(fp_huf);
	
	return 0;
}
