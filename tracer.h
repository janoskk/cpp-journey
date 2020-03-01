#ifndef tracer_h
#define tracer_h

struct Tracer
{
    Tracer() {
        std::cout << this << " default ctor\n";
    }
    Tracer(Tracer const& t) {
        std::cout << this << " copy ctor\n";
    }
    Tracer(Tracer&& t) {
        std::cout << this << " move ctor\n";
    }
    Tracer& operator=(Tracer const&) {
        std::cout << this << " operator=&\n";
        return *this;
    }
    Tracer& operator=(Tracer&&) {
        std::cout << this << " operator=&&\n";
        return *this;
    }
};

#endif /* tracer_h */
