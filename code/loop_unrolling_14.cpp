// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral.hpp>
using namespace boost::hana;


// sample(loop_unrolling_14)
__attribute__((noinline)) void f() { }

int main() {
    int_<10>.times(f);
}
// end-sample
