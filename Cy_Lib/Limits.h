#pragma once

#include <limits>
#include <cstdio>
#include <functional>


class Base {
public:
	virtual void gao1() = 0;
	virtual void gao2() = 0;
	void gao3() {
		printf("wwwww Gao3\n");
	}
	void gao4() {
		printf("wwwww Gao4\n");
	}
};

class Use : public Base {
public:
	virtual void gao1() {
		printf("Test Gao1\n");
	}	
	void gao2() {
		printf("Test Gao2\n");
	}
	void gao3() {
		printf("Test Gao3\n");
	}
	virtual void gao4() {
		printf("Test Gao4\n");
	}
};

class Use2 : public Use {
public:
	void gao1() {
		printf("Use Gao1\n");
	}
	void gao2() {
		printf("Use Gao2\n");
	}
	void gao3() {
		printf("Use Gao3\n");
	}
	void gao4() {
		printf("Use Gao4\n");
	}
};

void SomeLimits() {
	Use s1;
	Use2 s2;
	Base *p = &s1;
	p->gao1();
	p->gao2();
	p->gao3();
	p->gao4();
	p = &s2;
	p->gao1();
	p->gao2();
	p->gao3();
	p->gao4();
	Use* q = &s1;
	q->gao1();
	q->gao2();
	q->gao3();
	q->gao4();
	q = &s2;
	q->gao1();
	q->gao2();
	q->gao3();
	q->gao4();
}

void funtion() {


}