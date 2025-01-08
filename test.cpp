#include "signed_ptr.h"
#include <iostream>
#include <Windows.h>

//test class with a few attributes
class sample
{
public:
	int ivalue;
	std::string svalue;

	//test constructor with a param
	sample(int x) {
		ivalue = x;
		svalue = "test";
	}
};

int main() {
	//nullptr assignment
	signed_ptr<decltype(MessageBoxA)> ptr = nullptr;

	//no value
	signed_ptr<decltype(MessageBoxA)> other;

	//value assignment
	ptr = MessageBoxA;

	//assign another signed ptr
	other = ptr;

	//deref ptr
	(*other)(0, "Hey", "", 0);

	//constructor with params
	signed_ptr<sample> s = make_signed<sample>(1337);

	//getting underlying values
	std::cout << s->ivalue << std::endl;
	std::cout << (*s).svalue << std::endl;

	//deallocation
	s.destroy();
}
