#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<stdbool.h>

#include "share.c"


// GLOBAL VARIABLES

/*
 * Print linked list (testing)
 */
void printLL(Node *p) {
	Node *current = p;
	while (current != NULL) {
		printf("%c -> ", current->data);
                current = current->next;
        }
        printf("END\n");
}


/*
 * Convert binary string to LL;
 */
Node* toLL(char* order) {
	char* t;
	Node *binLL = NULL;
	Node *dumb = NULL;
	// add to binary LL
	for (t = order; *t != '\0'; t++) {
		if (binLL == NULL) {
			binLL = (Node*) malloc(sizeof(Node));
			binLL->data = *t;
			binLL->next = NULL;
			dumb = binLL;	
		}
		else {
			Node *nextBin = (Node*) malloc(sizeof(Node));
			nextBin->data = *t;
			nextBin->next = NULL;
			dumb->next = nextBin;
			dumb = dumb->next;
		}
	}

	return binLL;
}


/*
 * Convert to preorder traversal
 */
char* get8bit(char orderArray[], int p) {
	char* bw = malloc(9 * sizeof(char));
	for (int i = p; i < p + CHAR_BIT; i++) {
		bw[i-p] = orderArray[i];
	}
	bw[8] = '\0';
	return bw;
}


/*
 * Convert binary string to int
 */
int binaryToInt(char* bin) {
	int result = 0;
	for (int i = 0; i < CHAR_BIT; i++) {
		result = result * 2 + (bin[i] - '0');
	}

	return result;
}


int p = 0;
Node* readPreorder(char orderArray[]) {
	if (orderArray[p] == '\0')
		return NULL;

	if (orderArray[p] == '0') {
		Node* dumb = (Node*) malloc(sizeof(Node));
		dumb->data = '&';
		dumb->next = NULL;
		dumb->left = NULL;
		dumb->right = NULL;
		// Read left
		p++;
		dumb->left = readPreorder(orderArray);
		// Read right
		p++;
		dumb->right = readPreorder(orderArray);

		return dumb;
	}

	else {
		char* next = get8bit(orderArray, p + 1);
		p += 8;
		Node *leaf = (Node*) malloc(sizeof(Node));
		leaf->data = binaryToInt(next);
		leaf->left = NULL;
		leaf->right = NULL;
		free(next);
	
//		printf("leafffff -  %c\n", leaf->data);
		return leaf;
	}
	
}


/*
 * Find EOF marker
 */
void searchEOF(char orderArray[], Node* head) {
	if (head->left == NULL && head->right == NULL) {
		head->data = EOF;	
		return;
	}

	int step = orderArray[++p];
	if (step == '0') {
		searchEOF(orderArray, head->left);
	}
	else {
		searchEOF(orderArray, head->right);
	}
}


/*
 * Final decoder
 */
void decodeFile(char orderArray[], Node** head, Node* dumb, FILE *fp_decoded) {
	// Reach leaf node
	if (dumb->left == NULL && dumb->right == NULL) {
		char ch = dumb->data;


		if (ch == EOF) {
			return;
		}


		fputc(ch, fp_decoded);
		Node* resetDumb = *head;
		decodeFile(orderArray, head, resetDumb, fp_decoded);
	}
	else {
		char direction = orderArray[p];
		p++;
		if (direction == '0')
			decodeFile(orderArray, head, dumb->left, fp_decoded);
		else
			decodeFile(orderArray, head, dumb->right, fp_decoded);
	}
}



int main(int argc, char **argv) {
	if (argc == 1) {
		fprintf(stderr, "Program needs at least one argument!\n");
		exit(1);
	}

	
	char* order = NULL;
	order = (char*) malloc(100*100 * sizeof(char));
        memset(order, 0, 100*100);

	// Read input
	char* inputFile = argv[1];
	char* outputFile = NULL;
	if (argc == 3) {
		outputFile = argv[2];
	}

	FILE *fp = fopen(inputFile, "r");
	while (1) {
		char c = fgetc(fp);
		// EOF has frequency of 1
		if (feof(fp)) {
			break;
		}

		
		int dec = c;
		if (dec < 0)
			dec += 256;
		
		char* bin = decToBinary(dec);
		strcat(order, bin);
		
		free(bin);
	}

	fclose(fp);
	
	char orderArray[100*100];
	strcpy(orderArray, order);
	
	// Huffman tree
	Node *head = NULL;
	head = readPreorder(orderArray);

	searchEOF(orderArray, head);
	
	// NOTE: can read at this index p right away without incrementing by 1	
	Node* dumbHead = head;
	p++;
	FILE *fp_decoded;
        if (outputFile == NULL) {
                fp_decoded  = stdout;
        }
	else 
		fp_decoded = fopen(outputFile, "wb");


	
//	printf("%d\n", p);
//	printf("%s\n", orderArray);
		
	// Final decode
	decodeFile(orderArray, &head, dumbHead, fp_decoded);

//	fputc('\n', fp_decoded);
	

	// FREE MEMORY
	free(order);
	freeTree(head);

	if (outputFile != NULL)
		fclose(fp_decoded);

	return 0;
}
