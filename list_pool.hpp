#pragma once
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <utility>

template <typename I, typename X, typename T>
class _iterator {
  I current;
  X pool;
  public:
  using value_type = T;
  using reference = value_type&;

  _iterator(I x, X pool_x) : current{x}, pool{pool_x}{}

  reference operator*() const {
	return (*pool)[current-1].value;
  	}

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
  }

  friend bool operator!=(const _iterator& x, const _iterator& y) {
    return !(x == y);
  }
};



template <typename T, typename N = std::size_t>
class list_pool{

  struct node_t{
    T value;
    N next;

    node_t(const T& x, N p) noexcept
        : value{x},  // copy ctor
          next{p} {}

    node_t(T&& x, N p) noexcept
        : value{std::move(x)},  // move ctor
          next{p} {} 
  };

  std::vector<node_t> pool;
  using list_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  list_type free_node_list = list_type(0); // at the beginning, it is empty
  
  node_t& node(list_type x) noexcept { return pool[x-1]; }
  const node_t& node(list_type x) const noexcept { return pool[x-1]; }

  public:
  list_pool() noexcept = default;
  list_pool(list_pool&&) noexcept = default;

  explicit list_pool(size_type n){
	pool.reserve(n);
  } // reserve n nodes in the pool
    
  using iterator = _iterator<list_type,std::vector<node_t>*, T>;
  using const_iterator = _iterator<list_type,std::vector<node_t>*, const T>;
  iterator begin(list_type x) { 
	  return iterator{x,&pool};}

  iterator end(list_type ){ 
	return iterator{end(),&pool};} // this is not a typo
    
  const_iterator begin(list_type x) const{
	 return const_iterator{x,&pool};}

  const_iterator end(list_type ) const{
  	 return const_iterator{end(),&pool};} 
  
  const_iterator cbegin(list_type x) const{
	  return const_iterator{x,&pool};}

  const_iterator cend(list_type ) const{
  	  return const_iterator{end(),&pool};}
    
  list_type new_list() noexcept {
	return end();	 
  } // return an empty list

  void reserve(size_type n){
	  pool.reserve(n);  
  }; // reserve n nodes in the pool

  size_type capacity() const{
		return  pool.capacity();
  } // the capacity of the pool
  
  list_type _size() const{
  		return pool.size();}

  bool is_empty(list_type x) const noexcept {return x==end();}

  list_type end() const noexcept { return list_type(0); }
  
  T& value(list_type x){
	return node(x).value;
  }
  const T& value(list_type x) const {
	return node(x).value;
  }

  list_type& next(list_type x){
	return node(x).next;
  }
  const list_type& next(list_type x) const{
	return node(x).next;
  }
 
  template<typename X>
  list_type using_free_node(X&& val,list_type index){ 
	auto aux = free_node_list;
	node(free_node_list).value=std::forward<X>(val);
	free_node_list=node(aux).next;
	node(aux).next=index;
	return aux;
  }


  list_type push_front(const T& val, list_type head){
	  if(free_node_list){
		head= using_free_node(val, head);
		}
	  else{
	  	pool.emplace_back(val,head);
	  	head=_size();
	  }
	  return head;

  }

  list_type push_front(T&& val, list_type head) noexcept {
	  if(free_node_list){
		head = using_free_node(std::move(val), head);
		}
	  else{
	  	pool.emplace_back(std::move(val),head);
	  	head=_size();
	  }
	  return head;
  }

  list_type last_node(list_type tmp) noexcept {
  	if(tmp){
    	    while (node(tmp).next){tmp = node(tmp).next;}}
  	return tmp;}


  list_type check_last_node(list_type head, list_type last){
	if(head){
		node(last_node(head)).next = last;
		return head;}
	else{return last;}

  }

  list_type push_back(const T& val, list_type head){
	  if(free_node_list){
		auto last = using_free_node(val,end());
		return check_last_node(head,last);
	  }
	
 	 else{
		pool.emplace_back(val,end());
		return check_last_node(head, _size());
	 }	
  }

  list_type push_back(T&& val, list_type head){
	  if(free_node_list){
		auto last = using_free_node(std::move(val),end());
		return check_last_node(head,last);
	  }
	
 	 else{
		pool.emplace_back(std::move(val),end());
		return check_last_node(head, _size());
	 }		 

  }
  
  list_type free(list_type x){
	if(x){
		auto tmp =node(x).next;
		node(x).next=free_node_list;
		free_node_list = x;
	 	return tmp;}
	else{
      		return end();}	       
  } // delete first node

  list_type free_list(list_type x){
	if(free_node_list){
		node(last_node(free_node_list)).next=x;}
	else{free_node_list=x;}
	return end();
  } // free entire list
};
