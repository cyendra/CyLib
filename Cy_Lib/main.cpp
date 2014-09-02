#include <iostream>
#include "Prototype.h"

using namespace std;

int main()
{
	Prototype* p = new ConcretePrototype("ÃØÃÜ");
	p->message();
	Prototype* q = p->Clone();
	q->message();
	system("pause");
}
