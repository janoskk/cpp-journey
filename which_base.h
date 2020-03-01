#ifndef which_base_h
#define which_base_h

struct Base1 {
    virtual void foo() = 0;
};

struct Base2 {
    virtual void foo() = 0;
};

struct Base11 : Base1 {
    void foo() override {
        // std::cout << "Base1\n";
    }
};

struct Base22 : Base2 {
    void foo() override {
        // std::cout << "Base2\n";
    };
};

struct Derived : Base11, Base22
{ };

void test_which_base()
{
    Derived d{};
    Base1* b1 = &d;
    Base2* b2 = &d;
    b1->foo(); // prints "Base1"
    b2->foo(); // prints "Base2"
}

#endif /* which_base_h */
