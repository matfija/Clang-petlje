#include <stdio.h>

int main() {
  int x = 10;
  while (x--)
    printf("%d", x);

  while (x--) {
    printf("%d", x);
    x += 2;
  }

  putchar('\n');

  {
    x = 10;
    while (x--) {
      continue;
      x--;
    }
    {
      x--;
      continue;
    }
  }

  while (1) {
    x = 10;
    break;
  }

  printf("%d", x);
  putchar('\n');

  while (1) {
    while (1)
      break;
    x = 5;
    break;
  }

  printf("%d", x);
  putchar('\n');

  while (0) {
  }

  while (0)
    ;

  return 0;
}
