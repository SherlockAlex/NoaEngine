#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class B;
class A;
class A {
public:
	B* b = nullptr;
};

class B {
public:
	A* a = nullptr;
};

class C {
public:
	A* a = nullptr;
};

int main(int argc,char * argv)
{
	A* a = new A();
	B* b = new B();
	C* c = new C();

	a->b = b;
	b->a = a;
	c->a = a;

	delete a;

	cout << "6" << endl;

	return 0;

}
