#include <iostream>
#include "ADS_set.h"
using namespace std;

int main(){
  cout << "Testing program for ADS_Set" << endl;

  ADS_set<double> set1={1,5,34,34.54,2312.5,34535.5};
  set1.dump();

}


