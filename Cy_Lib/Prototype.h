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
		std::cout << "��������" << std::endl;
		str = r.str;
	}
	virtual void message() {
		std::cout << str << std::endl;
	}
	virtual ~ConcretePrototype() {}
	virtual Prototype* Clone()
	{
		std::cout << "��¡����" << std::endl;
		return new ConcretePrototype(*this);
	}
};

/*
int main()
{
Prototype* p = new ConcretePrototype("����");
p->message();
Prototype* q = p->Clone();
q->message();
system("pause");
}

*/