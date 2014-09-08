#include <process.h>
#include <Windows.h>
#include <cstdio>

namespace Thread {
	
	unsigned WINAPI Read(void *arg);
	unsigned WINAPI Accu(void *arg);
	static HANDLE semOne;
	static HANDLE semTwo;
	static int cnt;

	int Test3() {
		HANDLE hThread1, hThread2;
		semOne = CreateSemaphore(NULL, 0, 1, NULL);
		semTwo = CreateSemaphore(NULL, 1, 1, NULL);

		hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
		hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

		WaitForSingleObject(hThread1, INFINITE);
		WaitForSingleObject(hThread2, INFINITE);
		CloseHandle(semOne);
		CloseHandle(semTwo);
		return 0;
	}

	unsigned WINAPI Read(void *arg) {
		for (int i = 0; i < 5; i++) {
			fputs("Input num: ", stdout);
			WaitForSingleObject(semTwo, INFINITE);
			scanf_s("%d", &cnt);
			ReleaseSemaphore(semOne, 1, NULL);
		}
		return 0;
	}
	
	unsigned WINAPI Accu(void *arg) {
		int sum = 0;
		for (int i = 0; i < 5; i++) {
			WaitForSingleObject(semOne, INFINITE);
			sum += cnt;
			ReleaseSemaphore(semTwo, 1, NULL);
		}
		printf("Result: %d\n", sum);
		return 0;
	}
	
	const int NUM_THREAD = 50;

	unsigned WINAPI ThreadFunc(void *arg);

	unsigned WINAPI ThreadInc(void *arg);
	unsigned WINAPI ThreadDec(void *arg);
	long long num = 0;
	CRITICAL_SECTION cs;

	int Test2() {
		HANDLE tHandles[NUM_THREAD];
		printf("sizeof long long: %d\n", sizeof(long long));
		InitializeCriticalSection(&cs);
		for (int i = 0; i < NUM_THREAD; i++) {
			if (i % 2) tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadInc, NULL, 0, NULL);
			else tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadDec, NULL, 0, NULL);
		}
		WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
		DeleteCriticalSection(&cs);
		printf("result: %I64d\n", num);
		return 0;
	}

	unsigned WINAPI ThreadInc(void *arg) {
		EnterCriticalSection(&cs);
		for (int i = 0; i < 50000000; i++) num += 1;
		LeaveCriticalSection(&cs);
		return 0;
	}
	unsigned WINAPI ThreadDec(void *arg) {
		EnterCriticalSection(&cs);
		for (int i = 0; i < 50000000; i++) num -= 1;
		LeaveCriticalSection(&cs);
		return 0;
	}

	int Test() {
		HANDLE hThread;
		DWORD wr;
		unsigned threadID;
		int param = 5;
		
		hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
		if (hThread == 0) {
			puts("_beginthreadex error");
			return -1;
		}
		if ((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED) {
			puts("thread wait error");
			return -1;
		}
		printf("wait result: %s\n", (wr == WAIT_OBJECT_0) ? "signaled" : "time-out");
		puts("end of test");
		return 0;
	}
	unsigned WINAPI ThreadFunc(void *arg) {
		int cnt = *((int*)arg);
		for (int i = 0; i < cnt; i++) {
			Sleep(1000);
			puts("running thread");
		}
		return 0;
	}
}