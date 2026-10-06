#include <stdio.h>
int main(){
    FILE *fptr;
    //----------------------writing file by making file writing it first thing, appending 
    // fptr=fopen("f.txt","w");// open to write or overwrite existing things
    // fprintf(fptr,"bla bla");
    ////Or append something on existing file and existing content;
    // fptr=fopen("f.txt","a");//open in add text on it mode or append mode
    // fprintf(fptr,"\n another  new bla bla");
    //....................................Reading
    // Open a file in read mode
fptr = fopen("f.txt", "r");

// Store the content of the file
char myString[100];

// If the file exist
if(fptr != NULL) {

  // Read the content and print it
  char t='t';
  char* tk=&t;
  while(tk) {
     tk=fgets(myString, 100, fptr);
    // printf("fgets finds : %p\n",tk);// when there is no texxt available in file it will return nil and loop stops
    printf("value is: %s\n", myString);
  }
// If the file does not exist
} else {
  printf("Not able to open the file.");
}

// Close the file
fclose(fptr);
    return 0;
}