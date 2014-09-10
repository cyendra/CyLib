#include <cstdio>

class Adaptee {
public:
	void SpecificRequest(int cost) {
		printf("such adaptee so %d\n", cost);
	}

};

class Target {
public:
	virtual void Request() = 0;
};

class AdapterCls : public Target, private Adaptee {
public:
	void Request() {
		SpecificRequest(100);
	}
};

class AdapterImp : public Target {
private:
	Adaptee adaptee;
public:
	void Request() {
		adaptee.SpecificRequest(100);
	}
};

class Client
{
public:
	Client();
	~Client();
	void gaoCls() {
		Target *p = new AdapterCls();
		p->Request();
		delete p;
	}
	void gaoImp() {
		Target *p = new AdapterImp();
		p->Request();
		delete p;
	}
private:
};

Client::Client()
{
}

Client::~Client()
{
}