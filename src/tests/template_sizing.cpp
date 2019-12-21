#include <iostream>
#include <iomanip>
#include <vector>

template <class T>
class foo {
    public:
        foo(T b) : a(b) {
            std::cout << sizeof(a) << "\n";
        }
    private:
        T a;
};

int main() {
    std::cout << "Finding size of types:\n";

    std::cout << "Int: ";
    foo<int> a(5);
    std::cout << "uint8_t: ";
    foo<uint8_t> b(5);
    std::cout << "uint32_t: ";
    foo<uint32_t> c(5);
    std::cout << "Double: ";
    foo<double> d(5);
}
