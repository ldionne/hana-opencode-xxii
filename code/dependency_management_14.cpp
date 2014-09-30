// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace boost::hana;


#if 0
auto unique = [](auto xs) {
    using L = datatype_t<decltype(xs)>;
    return foldl(xs, nil<L>, [](auto xs, auto x) {
        return eval_if(x ^in^ xs,
            always(xs),
            [=](auto _) { return cons(_(x), xs); }
        );
    });
};
#endif

#if 0
// sample(dependency_management_14-computation)
auto computation = [](auto f, auto ...dependencies) {
    return ...;
};

// par exemple
auto hello = computation([] {
    std::cout << "hello";
});

auto world = computation([] {
    std::cout << "world!" << std::endl;
}, hello);

assert(world.dependencies == tuple(hello));
// end-sample
#endif

struct Computation { };

template <typename F, typename Dependencies>
struct computation_impl : F {
    constexpr computation_impl(F f, Dependencies deps)
        : F(f), dependencies(deps)
    { }

    Dependencies dependencies;
    using hana_datatype = Computation;
};

auto computation = [](auto f, auto ...dependencies) {
    auto deps = tuple(dependencies...);
    return computation_impl<decltype(f), decltype(deps)>(f, deps);
};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Computation, Computation> : equal_mcd {
        template <typename C1, typename C2>
        static constexpr auto equal_impl(C1 c1, C2 c2) {
            return decltype_(c1) == decltype_(c2);
        }
    };
}}

// sample(dependency_management_14-depends_on)
auto depends_on = infix(fix([](auto depends_on, auto f, auto g) {
    return eval_if(g ^in^ f.dependencies,
        always(true_),
        [=](auto _) {
            return any(_(f).dependencies, [=](auto dep) {
                return depends_on(dep, g);
            });
        }
    );
}));
// end-sample

// sample(dependency_management_14-linearized)
auto linearized = [](auto fs) {
    return sort_by(flip(depends_on), fs);
};
// end-sample

auto all_dependencies = fix([](auto all_dependencies, auto f) {
    auto other_deps = flatten(fmap(f.dependencies, all_dependencies));
    return unique(concat(f.dependencies, other_deps));
});

auto independent = [](auto f, auto g) {
    return !(f ^depends_on^ g) && !(g ^depends_on^ f);
};

auto is_circular = [](auto f, auto g) {
    return f ^depends_on^ g && g ^depends_on^ f;
};

#if 0
auto parallelize = [](auto f) {
    auto fs = linearized(cons(f, all_dependencies(f)));

    for_each(group_by(independent, fs), [](auto work_step) {
#if 0
        auto futures = fmap(work_step, [&](auto f) {
            return std::async(f);
        });

        for_each(std::move(futures), [](auto&& f) {
            std::forward<decltype(f)>(f).wait();
        });
#endif

        // hana::tuple can't contain move-only types right now.
        std::vector<std::future<void>> futures;
        for_each(work_step, [&](auto f) {
            futures.push_back(std::async(std::launch::async, f));
        });

        for (auto& result: futures)
            result.wait();
    });
};
#endif

std::mutex io_lock;
auto print = [](auto s) {
    std::lock_guard<std::mutex> guard{io_lock};
    std::cout << s << std::endl;
};


int main() {

    {
// sample(dependency_management_14-depends_on_example)
// par exemple
auto hello = computation([] {
    std::cout << "hello";
});

auto world = computation([] {
    std::cout << "world!" << std::endl;
}, hello);

assert(world ^depends_on^ hello);
// end-sample
    }

    {
// sample(dependency_management_14-linearized_example)
// par exemple
auto hello = computation([] {
    std::cout << "hello";
});

auto world = computation([] {
    std::cout << "world!" << std::endl;
}, hello);

assert(linearized(tuple(world, hello)) == tuple(hello, world));
// end-sample
    }


    (void)independent;
    (void)all_dependencies;
    (void)is_circular;














































    //////////////////////////////////////////////////////////////////////////
    // Étapes pour la pâte
    //////////////////////////////////////////////////////////////////////////
    auto prechauffer = computation([] {
        print("On préchauffe le four à 180°C.");
    });

    auto tapisser_moules = computation([] {
        print("On tapisse les moules à gateau avec du papier parchemin.");
    });

    auto ingredients_secs = computation([] {
        print("On mélange la farine et la poudre à pâte dans un bol.");
    });

    auto melange_pate = computation([] {
        print("On fouette les oeufs et on incorpore les ingrédients secs.");
    }, ingredients_secs);

    auto cuire_pate = computation([] {
        print("On fait cuire au four environ 55 minutes.");

        // j'ai un bon four
        std::this_thread::sleep_for(std::chrono::milliseconds(55));
    }, melange_pate, prechauffer, tapisser_moules);


















































    //////////////////////////////////////////////////////////////////////////
    // Étapes pour le glaçage
    //////////////////////////////////////////////////////////////////////////
    auto chauffer_creme = computation([] {
        print("On porte la crème à ébullition.");
    });

    auto fouetter_glacage = computation([] {
        print("On fouette le chocolat et la crème chaude.");
    }, chauffer_creme);

    auto etendre_glacage = computation([] {
        print("On étend le glaçage sur le gâteau.");
    }, cuire_pate, fouetter_glacage);
















































    //////////////////////////////////////////////////////////////////////////
    // L'étape qu'on aime tous
    //////////////////////////////////////////////////////////////////////////
    auto delice = computation([] {
        print("Miam! Merci Ricardo!");
    }, etendre_glacage);

    auto recette = tuple(
        delice,
        chauffer_creme,
        fouetter_glacage,
        tapisser_moules,
        etendre_glacage,
        cuire_pate,
        prechauffer,
        melange_pate,
        ingredients_secs
    );
    for_each(linearized(recette), [](auto etape) {
        etape();
    });

















































    // Other tests
    {
        {
            auto hello = computation([] { std::cout << "hello"; });

            auto world = computation([] {
                std::cout << "world!" << std::endl;
            }, hello);

            assert(world.dependencies == tuple(hello));
        }

        BOOST_HANA_CONSTANT_ASSERT(delice ^depends_on^ melange_pate);
        BOOST_HANA_CONSTANT_ASSERT(!(melange_pate ^depends_on^ delice));

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
            linearized(tuple(h, g, f, i)) == tuple(f,g,h,i)
        );
    }
}
