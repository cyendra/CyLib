#include <iostream>
#include <string>

class Prototype {
public:
	virtual ~Prototype() = 0;
	virtual Prototype* Clone() = 0;
	virtual void message() = 0;
};
Prototype::~Prototype(){}

class ConcretePrototype : public Prototype {
private:
	std::string str;
public:
	ConcretePrototype() { str = ""; }
	ConcretePrototype(std::string s) { str = s; }
	ConcretePrototype(const ConcretePrototype& r) {
		std::cout << "拷贝构造" << std::endl;
		str = r.str;
	}
	virtual void message() {
		std::cout << str << std::endl;
	}
	virtual ~ConcretePrototype() {}
	virtual Prototype* Clone()
	{
		std::cout << "克隆操作" << std::endl;
		return new ConcretePrototype(*this);
	}
};

/*
int main()
{
Prototype* p = new ConcretePrototype("秘密");
p->message();
Prototype* q = p->Clone();
q->message();
system("pause");
}

*/