#include <iostream>

#ifdef WINSOCK
#include "Winsock.h"
#include "WebServ.h"
#include "Thread.h"  
#endif // WINSOCK

#include "Adapter.h"


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

#ifdef THREAD
	//Thread::Test();
	//Thread::Test2();
	Thread::Test3();
#endif // THREAD

	Client clt;
	clt.gaoCls();
	clt.gaoImp();

	system("pause");
}
