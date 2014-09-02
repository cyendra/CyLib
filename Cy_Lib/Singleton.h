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
	std::cout << "����ģʽ" << std::endl;
}
/*
int main()
{
Singleton* p = Singleton::Instance();
p->message();
Singleton* q = Singleton::Instance();
if (p == q) cout << "ȷʵ�ǵ���" << endl;
system("pause");
}
*/