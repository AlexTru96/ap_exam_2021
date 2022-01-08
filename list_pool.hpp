#pragma once
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <utility>

//ITERATOR CLASS: useful method to traverse a blazingly fast list_pool

template <typename I, typename X, typename T>
class _iterator {
  I current;
  X pool;
  public:
  using value_type = T;
  using reference = value_type&;

  _iterator(I x, X pool_x) noexcept : current{x}, pool{pool_x}{}
  // constructor used by methods of the list_pool class

  reference operator*() const {
	return (*pool)[current-1].value;
  	} //access to the value of the current node 

  _iterator& operator++() {  // pre-increment
  	current = (*pool)[current-1].next;
    	return *this;
	}

  _iterator operator++(int) {  // post-increment
        auto tmp = *this;
        ++(*this); 
    	return tmp;
 	}

  friend bool operator==(const _iterator& x, const _iterator& y) {
    return (*x.pool)[x.current-1].value == (*y.pool)[y.current-1].value;
  } // if the values are equal we considered both equal

  friend bool operator!=(const _iterator& x, const _iterator& y) {
    return !(x == y);
  }
};

// LIST POOL CLASS: The heart of the code

template <typename T, typename N = std::size_t>
class list_pool{

  struct node_t{
    T value;
    N next;

    node_t(const T& x, N p) noexcept
        : value{x},  // copy constructor, noexcept allowed, just two integers
          next{p} {}

    node_t(T&& x, N p) noexcept
        : value{std::move(x)},  // move constructor
          next{p} {} 
  };

  std::vector<node_t> pool;
  using list_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  list_type free_node_list {end()}; // at the beginning, it is empty
  
  node_t& node(list_type x) noexcept { return pool[x-1]; } // given an index it will return the node associated in the vector of nodes.
  const node_t& node(list_type x) const noexcept { return pool[x-1]; }

  public:
  list_pool() noexcept = default; //default constructor
  list_pool(list_pool&&) noexcept = default; //move constructor
  list_pool& operator=(list_pool&&) = default; // move assignment

  list_pool(const list_pool& that_list) noexcept {
	pool = that_list.pool;
  }  // copy constructor

  list_pool& operator=(const list_pool& that_list) {
  	pool.clear();
	pool = that_list.pool;
	return *this;	
  } //copy assignment

  explicit list_pool(size_type n){
	pool.reserve(n);
  } // reserve n nodes in the pool,constructor that acquires resources (noexcept not recommended)
    
  using iterator = _iterator<list_type,std::vector<node_t>*, T>;
  using const_iterator = _iterator<list_type,std::vector<node_t>*, const T>;
  iterator begin(list_type x) noexcept { 
	  return iterator{x,&pool};}

  iterator end(list_type ) noexcept{ 
	return iterator{end(),&pool};} // this is not a typo
    
  const_iterator begin(list_type x) const noexcept{
	 return const_iterator{x,&pool};}

  const_iterator end(list_type ) const noexcept{
  	 return const_iterator{end(),&pool};} 
  
  const_iterator cbegin(list_type x) const noexcept{
	  return (*this).begin(x);}

  const_iterator cend(list_type ) const noexcept{
  	  return (*this).end(list_type(0));}
    
  list_type new_list() noexcept {
	return end();	 
  } // return an empty list

  void reserve(size_type n){
	  pool.reserve(n);  
  }; // reserve n nodes in the pool, acquires resources

  size_type capacity() const noexcept{
		return  pool.capacity();
  } // the capacity of the pool
  
  list_type _size() const noexcept{
  		return pool.size();}

  bool is_empty(list_type x) const noexcept {return x==end();}

  list_type end() const noexcept { return list_type(0); }
  
  T& value(list_type x) noexcept{
	return node(x).value;
  }
  const T& value(list_type x) const noexcept{
	return node(x).value;
  }

  list_type& next(list_type x) noexcept {
	return node(x).next;
  }
  const list_type& next(list_type x) const noexcept{
	return node(x).next;
  }
 
  list_type using_free_node(T&& val,list_type index) { 
	auto aux = free_node_list;
	node(free_node_list).value=std::forward<T>(val);
	free_node_list=node(aux).next;
	node(aux).next=index;
	return aux;
  } // helpful function which uses one node from free_node_list, setting the user value and the next index, returns the index of the used node.
  
  // luckily std::vector<node_t> will take care of the capacity :D
  //
  list_type _push_front(T&& val, list_type head){
	if(free_node_list){
		head= using_free_node(std::forward<T>(val), head);
		}
	else{
	  	pool.emplace_back(std::forward<T>(val),head);
	  	head=_size();
	}
	return head;
  }// returns the modified head of the list. This function could acquire resources by using emplace back so noexcept is not recommended.

  list_type push_front(const T& val, list_type head){
	return _push_front(val,head);
  } // Push front method for lhs values

  list_type push_front(T&& val, list_type head) {
	return _push_front(std::move(val),head);
  } // Push front method for rhs values 

  list_type last_node(list_type tmp) noexcept {
  	if(tmp){
    	    while (node(tmp).next){tmp = node(tmp).next;}}
  	return tmp;} 
  // returns the index of the last node in the actual list 

  list_type check_last_node(list_type head, list_type last) noexcept{
	if(head){
		node(last_node(head)).next = last;
		return head;}
	else{return last;}
  } // helpful function to concatenate the last node to the list when using push_back methods

  list_type _push_back(T&& val, list_type head){
	if(free_node_list){
		auto last = using_free_node(std::forward<T>(val),end());
		return check_last_node(head,last);
	}
	else{
		pool.emplace_back(std::forward<T>(val),end());
		return check_last_node(head, _size());
	}	
  } // returns the original head of the list. This function could acquire resources by using emplace back so noexcept is not recommended.

  list_type push_back(const T& val, list_type head){
	return _push_back(val,head);
  } // Push back method for lhs values

  list_type push_back(T&& val, list_type head){
	return _push_back(std::move(val),head); 
  } // Push back method for rhs values  

  list_type free(list_type x) noexcept {
	if(x){
		auto tmp =node(x).next;
		node(x).next=free_node_list;
		free_node_list = x;
	 	return tmp;}
	else{
      		return end();}	       
  } // delete first node concatenates with the current free list and returns the next in the actual list

  list_type free_list(list_type x) noexcept{
	if(free_node_list){
		node(last_node(free_node_list)).next=x;}
	else{free_node_list=x;}
	return end();
  } // free entire list, concatenates with the current free list and returns 0

  const list_type show_free_list() const noexcept{
  	return free_node_list;} // returns the head of the free node list

  list_type merge_list(list_type& l1, list_type& l2) noexcept{
	node(last_node(l1)).next = l2;
	l2 = end();
  	return l1;} // merges list l1 and l2, the head will be l1's head
};
