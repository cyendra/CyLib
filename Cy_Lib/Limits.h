#pragma once

#include <limits>
#include <cstdio>

class Gao{
public:
	Gao(){}
	Gao(const Gao&) {
		puts("Cons");
	}
	Gao& operator=(const Gao&){
		puts("===");
		return *this;
	}
};

void SomeLimits() {
	Gao g1;
	Gao g2 = g1;
	Gao g3(g2);
	g1 = g3;
}