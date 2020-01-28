#include <iostream>
using namespace std;

char wot() {
  return '\0';
}
int main() {
  if (wot()) {
    cout << "wow" << endl;
  }
  cout << wot() << endl;
  return 0;
}