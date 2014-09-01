#include <iostream>
#include "SmartPoint.h"
using namespace std;

class B {
public:
	B() { cout << "������һ������~" << endl; }
	virtual ~B() { cout << "���౻������!" << endl; }
	virtual void message() { cout << "������������������������" << endl; }
};

class D : public B {
public:
	D() { cout << "������һ��������~" << endl; }
	virtual ~D() { cout << "�����౻������!" << endl; }
	virtual void message() { cout << "������������������������" << endl; }
};

void test1() { 
	cout << "������ʾ:" << endl;
	cylib::SmartPtr<B> bp = new B();
	cylib::SmartPtr<B> bp2(new B());
}
void test2() { 
	cout << "�Ƚ���ʾ:" << endl;
	cylib::SmartPtr<B> bp = new B();
	B* p = bp.get();
	cylib::SmartPtr<B> bp2 = bp;
	if (bp == p) cout << "���" << endl;
	if (bp == bp2) cout << "���" << endl;
}
void test3() {
	cout << "��̬��ʾ:" << endl;
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