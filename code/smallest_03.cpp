// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/less.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include <cassert>
#include <string>
using namespace boost::mpl;


// sample(smallest_03)
template <typename Ts>
struct smallest
    : deref<
        typename min_element<
            Ts, less<sizeof_<_1>, sizeof_<_2> >
        >::type
    >
{ };

struct Foo { int* p; };

int main() {
    assert((boost::is_same<
        smallest<vector<int, float, char, double, std::string, Foo> >::type,
        char
    >::value));
}
// end-sample
