#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <array>

#include "ducer/ducer.h"
namespace d = nsducer;

auto even = [](auto v){return v % 2 == 0;};
auto odd = [](auto v){return v % 2 != 0;};
auto inc = [](auto v){return ++v;};

SCENARIO("ducer", "[ducer][operators]"){
    GIVEN("a test"){

        WHEN("sum of even number in 1-4 each incremented by 1"){

            long actual;
            const long required = 8;

            auto source = std::array<int, 4>({1,2,3,4});

            auto sum_of_inc_even = d::ducer.
                filter(even).
                map(inc).
                sum();

            THEN("the output must be 8"){
                auto ss = std::stringstream();
                std::cout << sum_of_inc_even.
                    into(std::addressof(ss), source)->str();
                auto actual = ss.str();
                auto required = std::string("8\n");
                REQUIRE(actual == required);
            }

            THEN("the output must be 8"){
                actual = d::into(
                    d::compose(
                        d::filterer(even),
                        d::mapper(inc),
                        d::summer(),
                        d::println(std::cout)),
                    0L,
                    source);
                REQUIRE(actual == required);
            }

            THEN("the output must be 8"){
                actual = sum_of_inc_even.
                    println(std::cout).
                    into(0L, source);
                REQUIRE(actual == required);
            }

            THEN("the output must be 8"){
                actual = d::into(
                    d::compose(
                        sum_of_inc_even,
                        d::println(std::cout)),
                    0L,
                    source);
                REQUIRE(actual == required);
            }
        }
    }
}
