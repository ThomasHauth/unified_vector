# unified_vector
C++ class to hold various classes in one vector-like class and allow virtual dispatch free method calls.

```bash
mkdir build
cmake ../
make
```

Here is a short example of how the unified_vector class can be used:
```C++
struct TestClassA {
	int value = 23;
};

struct TestClassB {
	float value = 5.5;
};

unified_vector<TestClassA, TestClassB> uvec;

uvec.push_back(TestClassA());
uvec.push_back(TestClassB());
uvec.push_back(TestClassB());

// compute sum of entries
float sum = 0.0f;
uvec.visit([&sum]( auto const& e ) {sum+=e.value;});
```
