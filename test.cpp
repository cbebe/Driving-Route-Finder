#include <iostream>

int main() {
  int x = 12, y = 24;
  char c[50];
  sprintf(c, "W %d %d\n", x, y);
  std::cout << c;

  return 0;
}