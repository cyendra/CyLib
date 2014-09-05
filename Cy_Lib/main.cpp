#include <iostream>
#include "Winsock.h"

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

#define HOST
#ifdef HOST
	char s[100];
	while (cin >> s) {
		WinSock::GetHostByName(s);
		cin >> s;
		WinSock::GetHostByAddr(s);
	}
#endif // HOST
	
	system("pause");
}
