#pragma once
#include <typeinfo>
#include <iostream>
using std::cout;
using std::endl;

class CCC {
public:
	void gao(){
		cout << "wow" << endl;
	}
};

void test() {
	cout << typeid(int).name() << endl;
	cout << typeid(1).name() << endl;
	cout << typeid(typeid(1)).name() << endl;
}
