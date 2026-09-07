#include <stdio.h>
void addInt(void* x, void* y, void* result);
void addFloat(void* x, void* y, void* result);
int main()
{
    // int a = 10;
    // char b = 'x';

    // // Void pointer stores address of int
    // void* ptr = &a;

    // // Void pointer stores address of char
    // // ptr = &b;
    // int a = 10;
    // void* ptr = &a;
    // // The void pointer 'ptr' is cast to an integer pointer
    // // using '(int*)ptr' Then, the value is dereferenced
    // // with `*(int*)ptr` to get the value at that memory
    // // location
    // printf("%d\n", *(int*)ptr);
    int x=10,y=12;
    int result=0;
    addInt(&x,&y,&result);
    printf("sum is %d\n",result);
    float xf=10.2,yf=13.4;
    float resultf=0.0;
    addFloat(&xf,&yf,&resultf);
    printf("float sum is %.3f\n",resultf);
    return 0;
}
void addInt(void* x, void* y, void* result){
    *(int*)result=*(int*)x +*(int*) y;
}
void addFloat(void* x, void* y, void* result){
    *(float*)result=*(float*)x +*(float*) y;
}