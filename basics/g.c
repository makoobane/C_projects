#include <stdio.h>


/**
 * A completely generic add function.
 * It does not need to know what the types are.
 */
void generic_add(const void *a, const void *b, void *result, void (*add_func)(const void*,const void* ,void*)) {
    if (a && b && result && add_func) {
        add_func(a, b, result); // Delegate the work to the caller's custom function
    }
}

// ============================================================================
// Caller's side: You define the types and their specific math functions
// ============================================================================

// 1. Example with a built-in type (int)
void add_integers(const void *a, const void *b, void *result) {
    *(int *)result = *(const int *)a + *(const int *)b;
}

// 2. Example with a completely custom struct (Vector2D)
typedef struct {
    float x;
    float y;
} Vector2D;

void add_vectors(const void *a, const void *b, void *result) {
    const Vector2D *v1 = (const Vector2D *)a;
    const Vector2D *v2 = (const Vector2D *)b;
    Vector2D *res = (Vector2D *)result;
    
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
}

int main() {
    // Testing with standard integers
    int num1 = 10, num2 = 20, int_res = 0;
    generic_add(&num1, &num2, &int_res, add_integers);
    printf("Integer Result: %d\n", int_res);

    // Testing with a completely custom type the generic function never heard of
    Vector2D vec1 = {1.0f, 2.0f};
    Vector2D vec2 = {3.0f, 4.0f};
    Vector2D vec_res = {0};
    
    generic_add(&vec1, &vec2, &vec_res, add_vectors);
    printf("Vector Result: (%.1f, %.1f)\n", vec_res.x, vec_res.y);

    return 0;
}
