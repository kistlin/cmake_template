#include "MyClass.h"

#include <iostream>

MyClass::MyClass() {
   std::cout << "MyClass constructor\n";
}

MyClass::MyClass(const MyClass& rhs) {
    std::cout << "MyClass copy constructor\n";
}

MyClass::MyClass(MyClass&& rhs)  noexcept {
    std::cout << "MyClass move constructor\n";
}
