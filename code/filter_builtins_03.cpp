// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_fundamental.hpp>

#include <cassert>
#include <string>
using namespace boost::mpl;


// sample(filter_builtins_03)
template <typename Ts>
struct builtins
    : copy_if<Ts, boost::is_fundamental<_1> >
{ };

struct Foo { int* p; };

int main() {
    assert((equal<
        builtins<vector<int, float, Foo, char, double, std::string> >::type,
        vector<int, float, char, double>
    >::value));
}
// end-sample
