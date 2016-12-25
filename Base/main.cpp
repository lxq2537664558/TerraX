#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[])
{
	std::cout << "base lib" << std::endl;
	getchar();
	return 0;
}
/*
using FunCB1 = std::function<void(int)>;
using FunCB2 = std::function<void(int, int)>;
class Child1
{
public:
	Child1() {
	}
	void ChildFunc1(int n1) { std::cout << __FUNCTION__ << "(" << n1 << ")" << std::endl; }
};

class Child2
{
public:
	Child2() {
	}
	void ChildFunc2(int n1, int n2) { std::cout << __FUNCTION__ << "(" << n1 << ", " << n2 << ")" << std::endl; }
};

class TestAllEvents
{
public:
	void DoEvents() {
		for (auto var : m_evs) {
			(*var)();
		}
	}

	std::set<events_params*> m_evs;
};

int main()
{
	Child1 c1;
	Child2 c2;
	TestAllEvents tae;
	auto pf = FunCB1(std::bind(&Child1::ChildFunc1, &c1, std::placeholders::_1));
	tae.m_evs.insert(new events_params(pf, 10));
	tae.m_evs.insert(new events_params(FunCB2(std::bind(&Child2::ChildFunc2, &c2, std::placeholders::_1, std::placeholders::_2)), 10, 20));

	tae.DoEvents();
	return 0;
	// template argument deduction/substitution fails
	// std::cout << std::apply(add_generic, std::make_tuple(2.0f,3.0f)) << '\n'; 
}
*/