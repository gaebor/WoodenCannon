#include <iostream>

#include "wc.h"
#include "wc_vector.h"

int main()
{
	wc::VectorHelper<std::vector<int>>::Hacker::Custom([](void* x, size_t, const char*)
	{
		std::cout << x << " ";
	}, nullptr);
	std::cout << std::endl;

	std::cout << &(((std::vector<int>*)nullptr)->_Myfirst()) << " ";
	std::cout << &(((std::vector<int>*)nullptr)->_Mylast()) << " ";
	std::cout << &(((std::vector<int>*)nullptr)->_Myend()) << " ";
	std::cout << std::endl;
	return 0;
}