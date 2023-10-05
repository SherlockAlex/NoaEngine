#include <iostream>

using namespace std;

class Base 
{
public:
	Base() {
		Debug();
	}

	virtual void Debug() {
		cout << "Hello Base" << endl;
	}
};

class Child:public Base 
{
public:
	Child() :Base() {

	}

	void Debug() override {
		cout << "Hello Child" << endl;
	}

};

int main(int argc,char * argv[])
{
	Child c;

	return 0;
}