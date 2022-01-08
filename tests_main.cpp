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


SCENARIO("moving constructor"){
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

SCENARIO("moving assignment"){
  GIVEN("two list_pool class, then we will move one to the other") {
  
    list_pool<int, uint16_t> pool_1{};
    list_pool<int, uint16_t> pool_2{};
    auto l1 = pool_1.new_list();
    auto l2 = pool_2.new_list();
    l1 = pool_1.push_front(3, l1);
    l1 = pool_1.push_front(2, l1);
    l1 = pool_1.push_front(1, l1);
    REQUIRE(pool_1._size()==3);
    l2 = pool_2.push_front(7, l2);
    l2 = pool_2.push_front(6, l2);
    l2 = pool_2.push_front(5, l2);
    l2 = pool_2.push_front(4, l2);
    REQUIRE(pool_2._size()==4);
    pool_2 = std::move(pool_1);
    REQUIRE(pool_2._size()==3);
    REQUIRE(pool_1._size()==0);


   }
}

SCENARIO("copy constructor"){
  GIVEN("a list_pool class, then we will construct one similar without removing the original one") {
  
    list_pool<int, uint16_t> pool{};
    
    auto l1 = pool.new_list();
    l1 = pool.push_front(3, l1);
    l1 = pool.push_front(2, l1);
    l1 = pool.push_front(1, l1);
    list_pool<int, uint16_t> pool_new{pool};

    REQUIRE(pool_new._size()==pool._size());
    auto tmp = pool.last_node(l1);
    REQUIRE(pool_new.value(tmp)==pool.value(tmp));


   }
}

SCENARIO("copy assignment"){
  GIVEN("a list_pool class, then we will copy one similar discarding the previous one") {
  
    list_pool<int, uint16_t> pool_1{};
    
    auto l1 = pool_1.new_list();
    l1 = pool_1.push_front(3, l1);
    l1 = pool_1.push_front(2, l1);
    l1 = pool_1.push_front(1, l1);
    
    list_pool<int, uint16_t> pool_2{};
    auto l2 = pool_2.new_list();
    l2 = pool_2.push_front(7, l2);
    l2 = pool_2.push_front(6, l2);
    l2 = pool_2.push_front(5, l2);
    l2 = pool_2.push_front(4, l2);
    REQUIRE(pool_2.value(l2)==4);
    REQUIRE(pool_2._size()==4);
    pool_2 = pool_1;
    auto tmp = pool_1.last_node(l1);
    REQUIRE(pool_2._size()==pool_1._size());
    REQUIRE(pool_2.value(tmp)==pool_1.value(tmp));

   }
}
