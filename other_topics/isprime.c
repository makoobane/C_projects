#include <stdio.h>
#include <math.h>

int main() {
    int n;
    scanf("%d", &n);

    int isprime=1;
    // Check if prime and print
    int v=(int)floor(sqrt((double)n));
    for(int i=2;i<v+1;i++){
      if(n%i==0){
        isprime=0;
        break;
      }
    }
    if(isprime){
      printf("number : %d is prime",n);
    }else{
        printf("it is not prime");
    }
    return 0;
}