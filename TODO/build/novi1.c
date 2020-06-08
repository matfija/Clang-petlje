#include <stdio.h>

int main() {
  int x = 10;
  int l0 = 5;
 
  do{
       do{
         x--;
       }while(x>3);
    
   }while(x>0);

   int labela = 11;
goto labela;
  
do{
labela:
    printf("Cao zdravo");
    labela--;
}while(labela>0);


  return 0;
}
