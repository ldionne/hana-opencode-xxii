// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include <boost/fusion/include/for_each.hpp>
using namespace boost::fusion;


// sample(loop_unrolling_03)
__attribute__((noinline)) void f() { }

int main() {
    boost::array<int, 10> dummy;
    for_each(dummy, boost::bind(f));
}
// end-sample
