#include "json.hpp"

#include <iostream>

using namespace vastina;

int main()
{
  j_data<j_type::array> aa;
  std::cout << (aa.getType()==j_type::array) << '\n';

  const auto v {aa.getType()};
  j_data<v> vv;
  std::cout << (vv.getType()==j_type::array) << '\n';

  return 0;
}