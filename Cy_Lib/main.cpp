#include <iostream>
#include "Winsock.h"

using namespace std;

int main()
{
	int tp;
	cout << "1-服务器 2-客户端" << endl;
	cin >> tp;
	if (tp == 1) WinSock::Server(9190);
	if (tp == 2) WinSock::Client("127.0.0.1", 9190);
	system("pause");
}
