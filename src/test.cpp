#include <map>
#include <vector>
#include <iostream>

#include "Serializer.h"

class MyParent
{
public:
	int a, b;
	virtual int Op() = 0;
};

class Add : public MyParent
{
public:
	virtual int Op()
	{
		return a + b;
	};
};

class Mul: public MyParent
{
public:
	virtual int Op()
	{
		return flag ? -(a * b) : a * b;
	};
	bool flag;
};

int main(int argc, char* argv[])
{
	Add add; Mul mul;
	add.a = 5; add.b = 7;
	mul.a = 5; mul.b = 7;

	std::vector<Add> addons(2);
	addons[0] = add;
	add.b -= 1;
	addons[1] = add;
	
	std::map<char, Add> addons2;
	addons2['0'] = add;
	add.b += 1;
	addons2[' '] = add;

	auto new_obj = Serializer::Do(&add);
	std::cout << "single object: ";
	Serializer::print_buffer();

	new_obj = Serializer::Do(&addons);
	std::cout << "vector of two objects: ";
	Serializer::print_buffer();

	new_obj = Serializer::Do(&addons2);
	std::cout << "map of objects: ";
	Serializer::print_buffer();

	// watch that!
	std::cout << std::endl;
	const auto reconstructed = *((decltype(addons2)*)new_obj->data());
	for (auto x : addons2)
		std::cout << x.first << ": (" << x.second.a << ", " << x.second.b << ")" << std::endl;
	std::cout << std::endl;

	std::vector<std::vector<unsigned char>> m(2);
	m[0].resize(2); m[1].resize(5);
	m[0][0] = 1; m[0][1] = 2;
	m[1][0] = 3; m[1][1] = 4; m[1][2] = 5; m[1][3] = 6; m[1][4] = 7;

	new_obj = Serializer::Do(&m);
	std::cout << "vector of vector of bytes: ";
	Serializer::print_buffer();

	return 0;
}
