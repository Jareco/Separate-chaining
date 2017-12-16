#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename Key, size_t N = 7 /* implementation-defined */>
class ADS_set {
public:
  class Iterator;
  using value_type = Key;
  using key_type = Key;
  using reference = key_type&;
  using const_reference = const key_type&;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = Iterator;
  using const_iterator = Iterator;
  using key_compare = std::less<key_type>;   // B+-Tree
  using key_equal = std::equal_to<key_type>; // Hashing
  using hasher = std::hash<key_type>;        // Hashing
  private:

  struct Bucket{
    Key value;
    Bucket *next=nullptr;
    ~Bucket(){
      delete next;
    }
  };

  struct Table{
    Bucket* head=nullptr;
    ~Table(){
      delete head;
    }
  };


  size_t sz=0; 
size_t tablesize=0;
size_t max_sz{N};
  Table* table{nullptr};
  size_type hash_idx(const key_type& k) const { return hasher{}(k) % max_sz; }
  void insert_unchecked(const key_type& k){
    size_type hashwert=hash_idx(k);
    Bucket *help;
    help=new Bucket();
    help->next=table[hashwert].head;
    help->value=k;
    table[hashwert].head=help;
    sz++;
    help=nullptr;
    delete help;
    if(sz>max_sz*10) rehash();
  }
  
  bool find_pos(const key_type& k)const{
    bool result=false;
    size_type hashwert=hash_idx(k);
        if(table[hashwert].head!=nullptr){
 		auto current=table[hashwert].head;
         do{
      if(key_equal{}(current->value, k))
   		 result=true;
  		current=current->next;
	 }while(current!=nullptr&&(result==false));
      current=nullptr;
      delete current;
      }
   

    return result;
  }

    Bucket* find_pos2(const key_type& k)const{
    Bucket* pos=nullptr;
    size_type hashwert=hash_idx(k);
        if(table[hashwert].head!=nullptr){
 		auto current=table[hashwert].head;
         do{
      if(key_equal{}(current->value, k)){
   		 pos=current;
        return pos;
        }
  		current=current->next;
	 }while(current!=nullptr);
      }
   

    return pos;
  }

  void rehash(){
    std::vector<Key>temp;
     for(int i=0; i<max_sz; i++){
        if(table[i].head!=nullptr){
 		auto current=table[i].head;
         do{
   		 temp.push_back(current->value);
  		current=current->next;
	 }while(current!=nullptr);
   current=nullptr;
   delete current;
      }

    }
    delete[] table;
    sz=0;
    max_sz=max_sz*3;
    table=new Table[max_sz];
   for(const auto& k: temp)
      insert_unchecked(k);
  }

  public:


  ADS_set(){
    if(tablesize==0){
    table=new Table[N];
    tablesize=N;
    }
  }
  ADS_set(std::initializer_list<key_type> ilist){
      if(tablesize==0){
   table=new Table[N];
    tablesize=N;
    }
    insert(ilist);
  }
  template<typename InputIt> ADS_set(InputIt first, InputIt last){
      if(tablesize==0){
    table=new Table[N];
    tablesize=N;
    }
    insert(first, last);
  }
  ADS_set(const ADS_set& other){
      if(tablesize==0){
    table=new Table[N];
    tablesize=N;
    }
    for (const auto& k: other) insert_unchecked(k);
  }
   ~ADS_set(){delete[] table;}

  ADS_set& operator=(const ADS_set& other) { 
    if (this == &other) return *this;
    clear();
   for (const auto& k: other) insert_unchecked(k);
    return *this;
  }
  ADS_set& operator=(std::initializer_list<key_type> ilist) { 
    clear();
    insert(ilist);
    return *this;
   }

  size_type size() const{
    return sz;
  }
  bool empty() const{
    return !sz;
  }

  size_type count(const key_type& key) const{
    return find_pos(key);
  }
   iterator find(const key_type& key) const { 
         if (Bucket *pos {find_pos2(key)}) return const_iterator{pos,(int)hash_idx(key),table,max_sz};
    return end();
   }

 void clear()  { 
   delete [] table;
   sz=0;
   max_sz=N;
   table=new Table[max_sz];
  }
  
  void swap(ADS_set& other) {     
    using std::swap;
    swap(sz, other.sz);
    swap(max_sz, other.max_sz);
    swap(tablesize, other.tablesize);
    swap(table, other.table); }

  void insert(std::initializer_list<key_type> ilist){
   if(tablesize==0){
   table=new Table[N];
    tablesize=N;
    }
    for (const auto& k: ilist) 
      if (!find_pos(k)) insert_unchecked(k);
  }

 std::pair<iterator,bool> insert(const key_type& key)  {
    if (find_pos2(key))
      return std::make_pair(const_iterator{find_pos2(key),(int)hash_idx(key),table,max_sz}, false);
    
    insert_unchecked(key);
    return std::make_pair(const_iterator{find_pos2(key),(int)hash_idx(key),table,max_sz}, true);
  }


  template<typename InputIt> void insert(InputIt first, InputIt last){
  if(tablesize==0){
  table=new Table[N];
    tablesize=N;
    }
    for (auto it = first; it != last; ++it) {
      if (!find_pos(*it)) {
        insert_unchecked(*it);
      }
    }
  }

  size_type erase(const key_type& key) { 
    if(find_pos(key)){
        size_type hashwert=hash_idx(key);
        if(table[hashwert].head!=nullptr){
 		Bucket* current=table[hashwert].head;
     Bucket* beforcurrent=table[hashwert].head;
     int counter=0;
         do{
      if(key_equal{}(current->value, key)){
   		 if(counter==0){
        table[hashwert].head=current->next;
        current->next=nullptr;
        delete current;
        beforcurrent=nullptr;
        delete beforcurrent;
        sz--;
        return 1;

	    }else{
        beforcurrent->next=current->next;
        current->next=nullptr;
        beforcurrent=nullptr;
        delete beforcurrent;
        delete current;
        sz--;
        return 1;
      }

        }
      counter++;
      beforcurrent=current;
      current=current->next;
	 }while(current!=nullptr);
      }
    }
      return 0;
    }
  
  
  int findfirst()const {
    int firstelement=0;
    for(size_type i=0; i<max_sz; i++){
      if(table[i].head!=nullptr){firstelement=i;return firstelement;}
    }
    return firstelement;
  } 

/**
    int findlast()const {
    int lastelement=0;
    for(size_type i=0; i<max_sz; i++){
      if(table[i].head!=nullptr){lastelement=i;}
    }
    return lastelement;
  } 
**/
  const_iterator begin() const { return const_iterator{table[this->findfirst()].head,findfirst(),table, max_sz};}
  const_iterator end() const { return const_iterator{nullptr}; }

  void dump(std::ostream& o = std::cerr) const{
     for(int i=0; i<max_sz; i++){
        if(table[i].head!=nullptr){
 		auto current=table[i].head;
         do{
   		 o<<i<<": value: "<<current->value<<std::endl;
  		current=current->next;
	 }while(current!=nullptr);
   current =nullptr;
    delete current;
      }
     }
    
  }
  friend bool operator==(const ADS_set& lhs, const ADS_set& rhs) { 
    if (lhs.sz != rhs.sz) return false;
     for (const auto& k: lhs) 
      if (!rhs.find_pos(k)) return false;
    return true;
   }
  friend bool operator!=(const ADS_set& lhs, const ADS_set& rhs) { return !(lhs==rhs); }
};

template <typename Key, size_t N>
class ADS_set<Key,N>::Iterator {
  Bucket* pos;
  int element;
  Table* table;
  size_type max_sz;
public:
  using value_type = Key;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type&;
  using pointer = const value_type*;
  using iterator_category = std::forward_iterator_tag;


  explicit Iterator(Bucket * pos=nullptr, int element=0, Table* table=nullptr, size_type max_sz=0):pos{pos}, element{element}, 
                    table{table}, max_sz{max_sz}{
  }


  
  ~Iterator(){
    table=nullptr;
    pos=nullptr;
    delete pos;
    delete table;
  }

  reference operator*() const { return pos->value; }
  pointer operator->() const { return &pos->value; }
  
   void skip(){
    while(table[element].head==nullptr && element<max_sz){element++;}
  }

  Iterator& operator++() {
        if(pos->next!=nullptr&&pos!=nullptr){pos=pos->next;}
        else{
          element++;
            skip();
            if(element==max_sz){
              pos=nullptr;
              return *this;
            }         
            pos=table[element].head;
        }

    return *this;
  }
  Iterator operator++(int) { 
   auto rc = *this;
    ++*this;
    return rc;
   }
  friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.pos == rhs.pos; }
  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return lhs.pos != rhs.pos; }
};

template <typename Key, size_t N> void swap(ADS_set<Key,N>& lhs, ADS_set<Key,N>& rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H
