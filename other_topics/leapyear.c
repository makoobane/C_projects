#include <stdio.h>

int main() {
    int year;
    scanf("%d", &year);

    // Check and print
   if(year%4==0&&year%100!=0){
      printf("year:%d is leap year\n",year);
   }else if((year%100==0 && year%400==0) && year%4==0){
     printf("year %d is leap year\n",year);
   }else{
    printf("year %d is not leap\n",year);
   }
    return 0;
}

