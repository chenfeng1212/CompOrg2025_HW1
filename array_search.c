#include <stdio.h>

int arraySearch(int *p_a, int arr_size, int target)
{
    int result = -1;

    asm volatile(
        // Your code
        "li t2, 0\n\t"
	"1:\n\t"
	
	"bge t2, %[size], 2f\n\t"

	"slli t0, t2, 2\n\t"
	"add t0, %[p_a], t0\n\t"

	"lw t1, 0(t0)\n\t"
	
	"bne t1, %[target], 3f\n\t"
	"mv %[res], t2\n\t"
	"j 2f\n\t"
	
	"3:\n\t"
	"addi t2, t2, 1\n\t"
	"j 1b\n\t"

	"2:\n\t"

	: [res] "=r" (result)
	: [p_a] "r" (p_a), [target] "r" (target), [size] "r" (arr_size)
	:"t0", "t1", "t2", "memory"
	);

    return result;
}

// Main function to test the implementation
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
    int arr_size;
    fscanf(input, "%d", &arr_size);
    int arr[arr_size];

    // Read integers from input file into the array
    for (int i = 0; i < arr_size; i++) {
        int data;
        fscanf(input, "%d", &data);
        arr[i] = data;
    }
    int target;
    fscanf(input, "%d", &target);
    fclose(input);

    int *p_a = &arr[0];

    int index = arraySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}
