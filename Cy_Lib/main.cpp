#include <iostream>
#include "Winsock.h"
#include "WebServ.h"
#include "Thread.h"
using namespace std;

int main()
{
#ifdef SERVER
	int tp;
	cout << "1-服务器 2-客户端" << endl;
	cin >> tp;
	if (tp == 1) WinSock::Server(9190);
	if (tp == 2) WinSock::Client("127.0.0.1", 9190);
#endif // SERVER

#ifdef HOST
	char s[100];
	while (cin >> s) {
		WinSock::GetHostByName(s);
		cin >> s;
		WinSock::GetHostByAddr(s);
	}
#endif // HOST

#ifdef WEBSERV
	WebServ::Start("9190");
#endif // WEBSERV

	//Thread::Test();
	//Thread::Test2();
	Thread::Test3();
	system("pause");
}
