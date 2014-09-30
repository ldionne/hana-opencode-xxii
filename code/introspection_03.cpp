// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/define_assoc_struct.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <cassert>
#include <iostream>
#include <string>
using namespace boost::fusion;


// sample(introspection_03)
namespace keys { struct i; struct c; struct s; }
BOOST_FUSION_DEFINE_ASSOC_STRUCT((demo), Foo,
    (int, i, keys::i)
    (char, c, keys::c)
    (std::string, s, keys::s)
)

struct print {
    template <typename X>
    void operator()(X const& x) const { std::cout << x << std::endl; }
};

int main() {
    demo::Foo foo(1, 'x', "bonjour");
    for_each(foo, print());
    assert(at_key<keys::s>(foo) == "bonjour");
}
// end-sample
