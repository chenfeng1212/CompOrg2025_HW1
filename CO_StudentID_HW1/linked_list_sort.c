#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Split the linked list into two parts
void splitList(Node *head, Node **firstHalf, Node **secondHalf)
{
    Node *fh = NULL, *sh = NULL;
    asm volatile(
        /*
        Block A (splitList), which splits the linked list into two halves
        */
        "mv t0, %[head]\n\t"
		"mv t1, %[head]\n\t"
		"beqz t0, 3f\n\t"
		"beqz t1, 3f\n\t"
		"ld t3, 8(t1)\n\t"
		"beqz t3, 3f\n\t"
		
		"1:\n\t"
		"lw t2, 4(t1)\n\t"
		"beqz t2, 2f\n\t"
		"ld t2, 8(t2)\n\t"
		"beqz t2, 2f\n\t"

		"ld t0, 8(t0)\n\t"
		"ld t1, 8(t1)\n\t"
		"ld t1, 8(t1)\n\t"
		"j 1b\n\t"

		"2:\n\t"
		"ld t2, 8(t0)\n\t"
		"sd zero, 8(t0)\n\t"
		"mv %[fh], %[head]\n\t"
		"mv %[fh], t2\n\t"

		"3:\n\t"
		: [fh] "=r" (fh), [sh] "=r" (sh)
		: [head] "r" (head)
		: "t0", "t1", "t2", "t3",  "memory"
	);
    *firstHalf = fh;
    *secondHalf = sh;
}

// Merge two sorted linked lists
Node *mergeSortedLists(Node *a, Node *b)
{
    Node *result = NULL;
    Node *tail = NULL;

    asm volatile(
        /*
        Block B (mergeSortedList), which merges two sorted lists into one
        */
        "mv %[res], zero\n\t"
		"mv %[tail], zero\n\t"
		
		"1:\n\t"
		"beqz %[a], 3f\n\t"
		"beqz %[b], 3f\n\t"

		"lw t0, 0(%[a])\n\t"
		"lw t1, 0(%[b])\n\t"

		"ble t0, t1, 5f\n\t"

		"mv t2, %[b]\n\t"
		"ld t3, 8(%[b])\n\t"
		"mv %[b], t3\n\t"
		"j 6f\n\t"

		"5:\n\t"
		"mv t2, %[a]\n\t"
		"ld t3, 8(%[a])\n\t"
		"mv %[a], t3\n\t"

		"6:\n\t"
		"sd zero, 8(t2)\n\t"
		"beqz %[res], 7f\n\t"
		"sd t2, 8(%[tail])\n\t"
		"j 8f\n\t"

		"7:\n\t"
		"mv %[res], t2\n\t"

		"8:\n\t"
		"mv %[tail], t2\n\t"
		"j 1b\n\t"

		"3:\n\t"
		"or t4, %[a], %[b]\n\t"
		"beqz t4, 2f\n\t"

		"beqz %[res], 4f\n\t"
		"sd t4, 8(%[tail])\n\t"
		"j 2f\n\t"

		"4:\n\t"
		"mv %[res], t4\n\t"

		"2:\n\t"
		: [res] "=r" (result), [tail] "=r" (tail), [a] "+r" (a), [b] "+r" (b)
		:
		: "t0", "t1", "t2", "t3", "t4", "memory"
	);

    return result;
}

// Merge Sort function for linked list
Node *mergeSort(Node *head)
{
    if (!head || !head->next)
        return head; // Return directly if there is only one node

    Node *firstHalf, *secondHalf;
    splitList(head, &firstHalf,
              &secondHalf); // Split the list into two sublists

    firstHalf = mergeSort(firstHalf);   // Recursively sort the left half
    secondHalf = mergeSort(secondHalf); // Recursively sort the right half

    return mergeSortedLists(firstHalf, secondHalf); // Merge the sorted sublists
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int list_size;
    fscanf(input, "%d", &list_size);
    Node *head = (list_size > 0) ? (Node *)malloc(sizeof(Node)) : NULL;
    Node *cur = head;
    for (int i = 0; i < list_size; i++) {
        fscanf(input, "%d", &(cur->data));
        if (i + 1 < list_size)
            cur->next = (Node *)malloc(sizeof(Node));
        cur = cur->next;
    }
    fclose(input);

    // Linked list sort
    head = mergeSort(head);

    cur = head;
    while (cur) {
        printf("%d ", cur->data);
        asm volatile(
            /*
            Block C (Move to the next node), which updates the pointer to
            traverse the linked list
            */
            "ld %[next], 8(%[cur])\n\t"
			: [next] "=r" (cur)
			: [cur] "r" (cur)
			: "memory" 
	    );
    }
    printf("\n");
    return 0;
}
