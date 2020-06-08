#include <stdio.h>

int main() {
  int x = 10;
  int l0 = 5;

  {
    goto l0;
    for (; x > 0;) {
    l0 : {
      {
        goto l2;
        for (; x > 3;) {
        l2 : { x--; }
        }
      }
    }
    }
  }

  int labela = 11;

  {
    goto l1;
    for (; labela > 0;) {
    l1 : {
      printf("Cao zdravo");
      labela--;
    }
    }
  }

  return 0;
}

