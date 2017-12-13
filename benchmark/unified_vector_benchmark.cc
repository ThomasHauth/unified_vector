
#include <unified_vector.h>

class DataClassA {};

class DataClassB {};

class DataClassC {};


class Test{ 
public:

    void push_back(DataClassA ) {};

    void push_back(DataClassB ) {};

};

int main() {

    unified_vector<DataClassA, DataClassB, DataClassC> uv;

    uv.push_back(DataClassA());
    uv.push_back(DataClassB());
    uv.push_back(DataClassB());
    uv.push_back(DataClassC());

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
