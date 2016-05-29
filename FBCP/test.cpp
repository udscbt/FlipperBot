#include "fbcp.string.h"
#include <map>
#include <iostream>

int main()
{
  fbcp::string a("Hello");
  fbcp::string b(", World!");
  std::map<fbcp::string, int> c;
  if (a < b) std::cout << "Ciao";
  else std::cout << "Hello";
}
