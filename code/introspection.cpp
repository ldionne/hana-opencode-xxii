// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#define BOOST_PP_VARIADICS 1
#include <boost/hana/foldable/record_mcd.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/record/macros.hpp>
#include <boost/hana/searchable/record_mcd.hpp>
#include <boost/hana/string.hpp>

#include <cassert>
#include <iostream>
#include <string>
using namespace boost::hana;


// sample(introspection-declare)
struct Foo {
    BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Foo,
        (int, i),
        (char, c),
        (std::string, s)
    );
};
// end-sample

// sample(introspection-use)
int main() {
    Foo foo{1, 'x', "bonjour"};
    for_each(foo, [](auto member) {
        std::cout << member << std::endl;
    });

    assert(
        lookup(foo, BOOST_HANA_STRING("s")) == just("bonjour")
    );
}
// end-sample
