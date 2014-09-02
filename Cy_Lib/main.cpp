#include <iostream>
#include "Singleton.h"
using namespace std;

int main()
{
	Singleton* p = Singleton::Instance();
	p->message();
	Singleton* q = Singleton::Instance();
	if (p == q) cout << "确实是单例" << endl;
	system("pause");
}