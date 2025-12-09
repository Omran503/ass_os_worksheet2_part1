extern int sum_of_three(int a, int b, int c);
extern int sum_array(int* arr, int length);
extern int find_max(int* arr, int length);

/* Test array used by the kernel when invoking C functions. */
int test_array[5] = {5, 12, 7, 25, 3};

/* -------------------------------------------------------------------------
 * main()
 * Entry point invoked by the kernel after control is transferred from
 * loader.asm. The function exercises three C routines to validate that
 * the kernel can correctly call into C code using the cdecl convention.
 * ------------------------------------------------------------------------- */
void main() {
    int r1 = sum_of_three(10, 20, 30);        // Test summation of three integers
    int r2 = sum_array(test_array, 5);        // Compute sum of array elements
    int r3 = find_max(test_array, 5);         // Determine maximum element

    while (1);   // Maintain control; prevents returning to loader code
}

/* -------------------------------------------------------------------------
 * sum_of_three()
 * Returns the arithmetic sum of three integer arguments.
 * Serves as the minimal demonstration function described in the OS text.
 * ------------------------------------------------------------------------- */
int sum_of_three(int a, int b, int c) {
    return a + b + c;
}

/* -------------------------------------------------------------------------
 * sum_array()
 * Computes the total of all elements in an integer array.
 * Demonstrates pointer arithmetic and iteration in C.
 * ------------------------------------------------------------------------- */
int sum_array(int* arr, int length) {
    int total = 0;
    for (int i = 0; i < length; i++)
        total += arr[i];
    return total;
}

/* -------------------------------------------------------------------------
 * find_max()
 * Scans an integer array and returns the largest value.
 * Illustrates array traversal and conditional comparisons.
 * ------------------------------------------------------------------------- */
int find_max(int* arr, int length) {
    int max = arr[0];
    for (int i = 1; i < length; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}
