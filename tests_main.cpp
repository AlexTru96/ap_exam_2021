#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "list_pool.hpp"
#include <algorithm> // max_element, min_element
SCENARIO("a size check"){
  list_pool<int, std::size_t> pool{4};
  auto l1 = pool.new_list();
  l1 = pool.push_front(1,l1);
  l1 = pool.push_front(2,l1);
  auto l2 = pool.new_list();
  l2 = pool.push_back(3,l2);
  l2 = pool.push_back(4,l2);
  REQUIRE( pool._size()==4);
}

SCENARIO("checking merge list function"){
  GIVEN("two lists, l1 with 3 elements and l2 with 2") {
   list_pool<int, uint16_t> pool{5};
    auto l1 = pool.new_list();
    l1 = pool.push_front(3, l1);
    l1 = pool.push_front(2, l1);
    l1 = pool.push_front(1, l1);

    auto l2 = pool.new_list();

    l2 = pool.push_back(4, l2);
    l2 = pool.push_back(5, l2);
    
    l1 = pool.merge_list(l1,l2);
    REQUIRE(pool.is_empty(l2));
    auto tmp = pool.last_node(l1);
    
   REQUIRE(pool.value(tmp) == 5);
   }
}

SCENARIO("checking heads for the free node list"){
  GIVEN("a list of size 6 \n index l1 -> 5 -> 3 -> 1 -> 2 -> 4 -> 6 \n value l1 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6") {
   list_pool<int, uint16_t> pool{6};
    auto l1 = pool.new_list();
    l1 = pool.push_front(3, l1);
    l1 = pool.push_back(4, l1);
    l1 = pool.push_front(2, l1);
    l1 = pool.push_back(5, l1);
    l1 = pool.push_front(1, l1);
    l1 = pool.push_back(6, l1);
    
    l1 = pool.free(l1);
    auto head_free_nl = pool.show_free_list();
    REQUIRE(head_free_nl==5);
    REQUIRE(pool.value(l1)==2);
    
    l1 = pool.free(l1);
    l1 = pool.free(l1);
    head_free_nl = pool.show_free_list();
    REQUIRE(head_free_nl==1);
    REQUIRE(pool.value(l1) == 4);
   }
}


SCENARIO("moving pools"){
  GIVEN("a list_pool class, we will use move constructor") {
    list_pool<int, uint16_t> pool_moved{};
    auto l1 = pool_moved.new_list();
    l1 = pool_moved.push_front(3, l1);
    l1 = pool_moved.push_front(2, l1);
    l1 = pool_moved.push_front(1, l1);
    list_pool<int, uint16_t> pool_new{std::move(pool_moved)};
    REQUIRE(pool_new._size()==3);
    REQUIRE(pool_new.value(l1)==1);
    REQUIRE(pool_moved._size() == 0);

   }
}
