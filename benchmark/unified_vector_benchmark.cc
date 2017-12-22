#include <unified_vector.h>

#include <functional>
#include <iostream>
#include <string>

class DataClassA {
public:
	void dumpContent() {
		std::cout << "This is class A" << std::endl;
	}
};

class DataClassB {
public:
	void dumpContent() {
		std::cout << "This is class B" << std::endl;
	}
};

class DataClassC {
public:
	void dumpContent() {
		std::cout << "This is class C" << std::endl;
	}
};

class Test {
public:

	void push_back(DataClassA) {
	}

	void push_back(DataClassB) {
	}

};

namespace {

template<class TUsedInLambda>
void genLambdaFunction(TUsedInLambda fn) {
	fn(23, 34);
}

}

int main() {

	unified_vector<DataClassA, DataClassB, DataClassC> uv;

	uv.push_back(DataClassA());
	uv.push_back(DataClassB());
	uv.push_back(DataClassB());
	uv.push_back(DataClassC());

	auto lmd = [](auto & obj ) -> void { obj.dumpContent();};
	uv.visit( lmd );

	return 0;
}

/*

 #include <memory>

 #include <cxxabi.h>

 using namespace std;

 // GCC demangling -- not required for functionality
 string demangle(const char* mangled) {
 int status;
 unique_ptr<char[], void (*)(void*)> result(
 abi::__cxa_demangle(mangled, 0, 0, &status), free);
 return result.get() ? string(result.get()) : "ERROR";
 }

 template<typename Param>
 struct BaseSingle {
 virtual void BaseFoo(Param) {
 cout << "Hello from BaseSingle<"
 << demangle(typeid(Param).name())
 << ">::BaseFoo" << endl;
 };
 };

 template<typename... Params>
 struct Base : public BaseSingle<Params>... {
 template<typename T> void Foo(T&& x) {
 this->BaseSingle<T>::BaseFoo(forward<T>(x));
 }
 };

 int main() {
 Base<string, int, bool> b;
 b.Foo(1);
 b.Foo(true);
 b.Foo(string("ab"));
 }

 */
