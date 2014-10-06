#pragma once
#include <cstdio>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

namespace Lambda {
	void EachList() {
		std::vector<int>  someList{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int total = 0;
		std::for_each(someList.begin(), someList.end(), [&total](int x) {
			total += x;
		});
		std::cout << total << std::endl;
	}
	void LambdaFunc() {
		auto func = [](int x)->std::function < int(int) > {
			return [x](int y) {
				return x*y;
			};
		};
		auto f = func(10);
		std::cout << f(20) << std::endl;
	}
	void UseLambda() {
		auto xyz = [](int x){return x * 10; }(10);
		
	}

	void Iterator() {
		using namespace std;
		istream_iterator<string> ii(cin);
		istream_iterator<string> eos;
		for_each(ii, eos, [](string s){
			cout << s << endl;
		});

	}
}