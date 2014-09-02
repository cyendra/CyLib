#include <iostream>
#include "Abstract_Factory.h"

using namespace std;

int main()
{
	auto fac = ConcreteFactory::Instance();
	auto pro = fac->CreateProduct();
	auto obj = fac->CreateObject();
	pro->message();
	obj->gao();

	auto facEx = ExtraFactory::Instance();
	auto proEx = facEx->CreateProduct();
	auto objEx = facEx->CreateObject();
	proEx->message();
	objEx->gao();

	system("pause");
}
