#include <iostream>
#include <string>

class AbstractProduct {
public:
	virtual ~AbstractProduct() {}
	virtual void message() = 0;
};
class Product : public AbstractProduct {
	virtual ~Product() {}
	virtual void message() {
		std::cout << "我是一个普通的Product" << std::endl;
	}
};
class ProductEx : public AbstractProduct {
	virtual ~ProductEx() {}
	virtual void message() {
		std::cout << "我是加强后的Product" << std::endl;
	}
};

class AbstractObject {
public:
	virtual ~AbstractObject() {}
	virtual void gao() = 0;
};
class Object : public AbstractObject {
	virtual ~Object() {}
	virtual void gao() {
		std::cout << "虽然叫做Object，但是只是一个普通的Object" << std::endl;
	}
};
class ObjectEx : public AbstractObject {
	virtual ~ObjectEx() {}
	virtual void gao() {
		std::cout << "虽然叫做ObjectEx好像很厉害的样子，其实还是一个普通的Object" << std::endl;
	}
};

class AbstractFactory {
public:
	virtual AbstractProduct* CreateProduct() = 0;
	virtual AbstractObject* CreateObject() = 0;
	virtual ~AbstractFactory() {}
protected:
	AbstractFactory(){}
	AbstractFactory(const AbstractFactory&);
	AbstractFactory& operator=(const AbstractFactory&);
};

class ConcreteFactory : public AbstractFactory {
public:
	virtual AbstractProduct* CreateProduct() {
		return new Product();
	}
	virtual AbstractObject* CreateObject() {
		return new Object();
	}
	static ConcreteFactory* Instance() {
		static ConcreteFactory _instance;
		return &_instance;
	}
};

class ExtraFactory : public AbstractFactory {
public:
	virtual AbstractProduct* CreateProduct() {
		return new ProductEx();
	}
	virtual AbstractObject* CreateObject() {
		return new ObjectEx();
	}
	static ExtraFactory* Instance() {
		static ExtraFactory _instance;
		return &_instance;
	}
};
/*
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

*/