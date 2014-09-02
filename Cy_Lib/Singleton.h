#include <iostream>

class Singleton {
public:
	static Singleton* Instance();
	void message();
protected:
	Singleton() {};
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);	
};

Singleton* Singleton::Instance() {
	static Singleton _instance;
	return &_instance;
}
void Singleton::message() {
	std::cout << "单例模式" << std::endl;
}
/*
int main()
{
Singleton* p = Singleton::Instance();
p->message();
Singleton* q = Singleton::Instance();
if (p == q) cout << "确实是单例" << endl;
system("pause");
}
*/