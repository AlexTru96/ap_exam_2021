#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "list_pool.hpp"
#include <algorithm> // max_element, min_element
SCENARIO("some size checks"){
  list_pool<int, std::size_t> pool{4};
  auto l1 = pool.new_list();
  l1 = pool.push_front(42,l1);
  l1 = pool.push_front(84,l1);
  l1 = pool.push_front(420,l1);
  auto l2 = pool.new_list();
  l2 = pool.push_back(100,l2);
  l2 = pool.push_back(23,l2);
  REQUIRE( pool._size()==5);
}


