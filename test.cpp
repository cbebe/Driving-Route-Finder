#include <iostream>

void printTest() {
  int x = 12, y = 24;
  char c[50];
  sprintf(c, "W %d %d\n", x, y);
  std::cout << c;
}

int main() {
  std::cout << atoi("12\n") << std::endl;
  return 0;
}