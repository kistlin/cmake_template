#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass {
public:
    MyClass();
    MyClass(const MyClass& rhs);
    MyClass(MyClass&& rhs) noexcept ;
};

#endif //  MYCLASS_H
