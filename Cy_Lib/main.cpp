#include <iostream>
#include "SmartPoint.h"
using namespace std;

class B {
public:
	B() { cout << "构造了一个基类~" << endl; }
	virtual ~B() { cout << "基类被析构啦!" << endl; }
	virtual void message() { cout << "基基基基基基基基基基基基" << endl; }
};

class D : public B {
public:
	D() { cout << "构造了一个派生类~" << endl; }
	virtual ~D() { cout << "派生类被析构啦!" << endl; }
	virtual void message() { cout << "派派派派派派派派派派派派" << endl; }
};

void test1() { 
	cout << "构造演示:" << endl;
	cylib::SmartPtr<B> bp = new B();
	cylib::SmartPtr<B> bp2(new B());
}
void test2() { 
	cout << "比较演示:" << endl;
	cylib::SmartPtr<B> bp = new B();
	B* p = bp.get();
	cylib::SmartPtr<B> bp2 = bp;
	if (bp == p) cout << "相等" << endl;
	if (bp == bp2) cout << "相等" << endl;
}
void test3() {
	cout << "多态演示:" << endl;
	cylib::SmartPtr<B> bp;
	cylib::SmartPtr<D> dp = new D();
	bp = dp;
	bp->message();
}


int main()
{
	cout << "---------------" << endl;
	test1();
	cout << "---------------" << endl;
	test2();
	cout << "---------------" << endl;
	test3();
	cout << "---------------" << endl;
	system("pause");

}