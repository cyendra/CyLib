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
	std::cout << "µ¥ÀýÄ£Ê½" << std::endl;
}
