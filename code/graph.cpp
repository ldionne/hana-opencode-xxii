// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <future>
#include <iostream>
using namespace boost::hana;


struct Computation { };

template <typename F, typename Dependencies>
struct computation_impl {
    F f;
    Dependencies dependencies;
    using hana_datatype = Computation;
};

auto computation = [](auto f, auto ...dependencies) {
    auto deps = tuple(dependencies...);
    return computation_impl<decltype(f), decltype(deps)>{f, deps};
};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Computation, Computation> : equal_mcd {
        template <typename C1, typename C2>
        static constexpr auto equal_impl(C1 c1, C2 c2) {
            return decltype_(c1.f) == decltype_(c2.f);
        }
    };
}}

auto depends_on = infix(fix([](auto depends_on, auto a, auto b) {
    return eval_if(b ^in^ a.dependencies,
        always(true_),
        [=](auto _) {
            return any(_(a).dependencies, [=](auto d) {
                return depends_on(d, b);
            });
        }
    );
}));

auto independent = [](auto a, auto b) {
    return !(a ^depends_on^ b) && !(b ^depends_on^ a);
};

auto parallelized = [](auto ...computations) {
    auto ordered = sort_by(flip(depends_on), tuple(computations...));
    auto grouped = group_by(independent, ordered);
    return grouped;

};

auto parallelize = [](auto ...computations) {
    for_each(parallelized(computations...), [](auto group) {
        auto futures = fmap(group, [](auto c) {
            return std::async(std::launch::async, c.f);
        });

        for_each(std::move(futures), [](auto&& f) {
            std::forward<decltype(f)>(f).wait();
        });
    });
};

// sample(graph)
int main() {
    auto f = computation([] { std::cout << "f" << std::endl; });
    auto g = computation([] { std::cout << "g" << std::endl; }, f);
    auto h = computation([] { std::cout << "h" << std::endl; }, g);
    auto i = computation([] { std::cout << "i" << std::endl; }, g);

    BOOST_HANA_CONSTANT_ASSERT(g ^depends_on^ f);
    BOOST_HANA_CONSTANT_ASSERT(!(f ^depends_on^ g));

    BOOST_HANA_CONSTANT_ASSERT(h ^depends_on^ g);
    BOOST_HANA_CONSTANT_ASSERT(h ^depends_on^ f);
    BOOST_HANA_CONSTANT_ASSERT(!(g ^depends_on^ h));
    BOOST_HANA_CONSTANT_ASSERT(!(f ^depends_on^ h));

    BOOST_HANA_CONSTANT_ASSERT(
        parallelized(f, g, h, i) == tuple(
            tuple(f),
            tuple(g),
            tuple(h, i)
        )
    );

    // parallelize(f, g, h, i);
}
// end-sample
