#include <iostream>
#include <string>

class Product {
public:
	virtual ~Product() {}
	virtual void gao() = 0;
};

class ConcreteProduct : public Product {
public:
	virtual ~ConcreteProduct() {}
	virtual void gao() {
		std::cout << "工厂方法" << std::endl;
	}
};

class Creator {
public:
	virtual ~Creator() {}
	virtual Product* FactoryMethod() = 0;
};

class ConcreteCreator : public Creator {
public:
	virtual ~ConcreteCreator() {}
	virtual Product* FactoryMethod() {
		return new ConcreteProduct();
	}
};