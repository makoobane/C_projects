#include <stdio.h>
int contains(int arr[],int length,int value);
int main() {
    int day, month, year;
    scanf("%d", &day);
    scanf("%d", &month);
    scanf("%d", &year);

    // Check and print
    int VALIDNESS=0;
    const int FEBRUARY=2;
    int leapyear=(year%4==0)&&(year%100!=0|| year%400==0);
    int monthvalid=month<13&&month>0;
    int abovethirdy[7]={1,3,5,7,8,10,12};
    if(monthvalid){
       if(contains(abovethirdy,7,month)){
       VALIDNESS =day<32?1:0;
       }else if(month==FEBRUARY){
        VALIDNESS=leapyear==0?(day<29?1:0):(day<30?1:0);
       }else{
        VALIDNESS=day<31?1:0;
       }
    }
    if(VALIDNESS){
    printf("this date is valid: %d/%d/%d",day,month,year);

    }else{
    printf("this date is not valid: %d/%d/%d",day,month,year);
    }

    return 0;
}

int contains(int arr[],int length, int value)
{
    for(int m=0;m<length;m++ ){
        if(arr[m]==value) return 1;
    }
    return 0;
}
