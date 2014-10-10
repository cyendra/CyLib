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
	/*
	template<typename R, typename... V>
	struct fixpoint {
		typedef std::function<R(V...)> func_t;
		typedef std::function<func_t(func_t)> tfunc_t;
		class loopfunc_t {
		public:
			func_t operator()(loopfunc_t v) const { return func(v); }
			template<typename L>
			loopfunc_t(const L &l) : func(l) { }
			loopfunc_t() { }
		private:
			std::function<func_t(loopfunc_t)> func;
		};
		static func_t Fix(tfunc_t f) {
			return [](loopfunc_t x) -> func_t { return x(x); }([f](loopfunc_t x) { return [f, x](V... v){return f(x(x))(v...); }; });
		}
	};
	template<typename T>
	struct getfixpoint {
		typedef typename getfixpoint<decltype(&T::operator())>::fp fp;
	};
	template<typename R, typename T, typename... V>
	struct getfixpoint<std::function<R(V...)>(T::*)(std::function<R(V...)>)> {
		typedef fixpoint<R, V...> fp;
	};
	template<typename R, typename T, typename... V>
	struct getfixpoint<std::function<R(V...)>(T::*)(std::function<R(V...)>) const> {
		typedef fixpoint<R, V...> fp;
	};
	template<typename R, typename... V>
	struct getfixpoint<std::function<R(V...)>(*)(std::function<R(V...)>)> {
		typedef fixpoint<R, V...>fp;
	};
	template<typename T>
	auto getFix(T f) -> typename getfixpoint<T>::fp::func_t {
		return getfixpoint<T>::fp::Fix(f);
	}
	void loopFunc() {
		auto fac = getFix(
			[](std::function<int(int)> f) -> std::function<int(int)> {
				return[f](int n)->int {
					if (n == 0) return 1;
					else return n * f(n - 1);
				};
			}
		);
		std::cout << fac(10) << std::endl;
	}
	*/
}