// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


// sample(filter_builtins)
template <typename ...T>
auto builtins = filter(tuple(type<T>...), trait<std::is_fundamental>);

struct Foo { int* p; };

int main() {
    assert((
        builtins<int, float, Foo, char, double, std::string>
        ==
        tuple(type<int>, type<float>, type<char>, type<double>)
    ));
}
// end-sample
