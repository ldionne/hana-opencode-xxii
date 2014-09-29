// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


// sample(smallest)
template <typename ...T>
auto smallest = minimum_by(ordering(sizeof_), tuple(type<T>...));

struct Foo { int* p; };

int main() {
    assert((
        smallest<int, float, char, double, std::string, Foo> == type<char>
    ));
}
// end-sample
