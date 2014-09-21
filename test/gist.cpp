#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <cassert>

template<class Next, class Complete>
struct stateless_s
{
    Next next;
    Complete complete;

    stateless_s(Next next, Complete complete) : next(next), complete(complete) {}

    // complete
    template<class Acc>
    Acc operator()(Acc acc) const { return complete(acc);}
    // next
    template<class Acc, class T>
    Acc operator()(Acc acc, T v) const { return next(acc, v);}
};

auto stateless = [](auto next, auto complete){
    return stateless_s<decltype(next), decltype(complete)>{next, complete};
};


template<class State, class Next, class Complete>
struct stateful_s
{
    std::shared_ptr<State> state;
    Next next;
    Complete complete;

    stateful_s(State state, Next next, Complete complete) : state(std::make_shared<State>(state)), next(next), complete(complete) {}

    // complete
    template<class Acc>
    Acc operator()(Acc acc) const { return complete(acc, *state);}
    // next
    template<class Acc, class T>
    Acc operator()(Acc acc, T v) const { return next(acc, *state, v);}
};

auto stateful = [](auto state, auto next, auto complete){
    return stateful_s<decltype(state), decltype(next), decltype(complete)>{state, next, complete};
};


auto filterer = [](auto pred){
    return [=](auto step) {
        return stateless(
            [=](auto s, auto v){
                if (pred(v)) {
                    return step(s, v);
                }
                return s;
            },
            [=](auto s){return step(s);});
    };
};

auto mapper = [](auto map){
    return [=](auto step) {
        return stateless(
            [=](auto s, auto v){
                return step(s, map(v));
            },
            [=](auto s){return step(s);});
    };
};

auto summer = [](){
    return [=](auto step) {
        return stateful(
            0L,
            [=](auto s, long& sum, auto v){
                sum += v; return s;
            },
            [=](auto s, long& sum){return step(s, sum);});
    };
};

auto println = [](auto& stream){
    auto pointer = std::addressof(stream);
    typedef decltype(pointer) Pointer;
    return [=](auto step) {
        return stateful(
            pointer,
            [=](auto s, Pointer stream, auto v){
                *stream << v << std::endl; return step(s, v);
            },
            [=](auto s, Pointer stream){return s;});
    };
};

auto pass = [](){
    return [=](auto step) {
        return step;
    };
};

auto transduce = [](auto ducer, auto reduce, auto init, auto collection) {
    auto step = ducer(reduce);
    for(auto& v : collection) {
        init = step(init, v);
    }
    return step(init);
};

template<class T, class V>
auto into_for(std::vector<T> acc, V v, int) {
    acc.push_back(v);
    return acc;
}

template<class Stream, class V, class C = decltype(*(Stream)nullptr << *(V*)nullptr)>
auto into_for(Stream acc, V v, int) {
    *acc << v << std::endl;
    return acc;
}

template<class T, class V, class C = decltype(*(T*)nullptr = *(V*)nullptr)>
auto into_for(T acc, V v, ...) {
    acc = v;
    return acc;
}

struct into_next
{
    template<class LHS, class RHS>
    auto operator()(LHS lhs, RHS rhs) const {
        return into_for(lhs, rhs, 0);
    }
};

auto into = [=](auto ducer, auto out, auto in){
    return transduce(
        ducer,
        stateless(
            into_next(),
            [](decltype(out) acc){return acc;}),
        out,
        in);
};

template<class Ducer>
auto compose(Ducer ducer) {
    return ducer;
}

template<class Ducer0, class... DucerN>
auto compose(Ducer0 ducer0, DucerN... ducerN) {
    return [=](auto step) {return ducer0(compose(ducerN...)(step));};
}

template<class Ducer = decltype(pass())>
struct ducer : public Ducer
{
    ducer() : Ducer(pass()) {}
    ducer(Ducer d) : Ducer(d) {}

    template<class Pred>
    auto filter(Pred pred) {
        auto composed = compose(*this, filterer(pred));
        return ducer<decltype(composed)>(composed);
    }

    template<class Map>
    auto map(Map map) {
        auto composed = compose(*this, mapper(map));
        return ducer<decltype(composed)>(composed);
    }

    auto sum() {
        auto composed = compose(*this, summer());
        return ducer<decltype(composed)>(composed);
    }

    template<class Stream>
    auto println(Stream& stream) {
        auto composed = compose(*this, ::println(stream));
        return ducer<decltype(composed)>(composed);
    }

    template<class To, class Collection>
    auto into(To to, Collection collection) {
        return ::into(*this, to, collection);
    }

};

auto even = [](auto v){return v % 2 == 0;};

auto inc = [](auto v){return ++v;};

template<class InputIterator, class OutputIterator, class UnaryFunction>
OutputIterator acc_transform(
  InputIterator first1,
  InputIterator last1,
  OutputIterator result,
  UnaryFunction func){
    return std::accumulate(first1, last1, result, [=](auto result, auto v){*result = func(v); return ++result;});
}

int main() {

    long actual;
    const long required = 8;

    auto source = std::array<int, 4>({1,2,3,4});

    auto out = std::vector<long>();
    acc_transform(std::begin(source), std::end(source), std::back_inserter(out), [](auto v){return v + 1;});
    for(auto v : out) {
        std::cout << v << std::endl;
    }

    auto sum_of_inc_even = ducer<>().
        filter(even).
        map(inc).
        sum();

    {
        auto ss = std::stringstream();
        std::cout << transduce(sum_of_inc_even,
            stateless(
                [](auto acc, auto v){*acc << v << std::endl; return acc;},
                [](auto acc){return acc;}),
            std::addressof(ss), source)->str();
    }

    {
        actual = into(
            compose(
                filterer(even),
                mapper(inc),
                summer(),
                println(std::cout)),
            0L,
            source);
        assert(actual == required);
    }

    {
        actual = sum_of_inc_even.
            println(std::cout).
            into(0L, source);
        assert(actual == required);
    }

    {
        actual = into(
            compose(
                sum_of_inc_even,
                println(std::cout)),
            0L,
            source);
        assert(actual == required);
    }

    {
        auto ss = std::stringstream();
        std::cout << sum_of_inc_even.
            into(std::addressof(ss), source)->str();
        auto actual = ss.str();
        auto required = std::string("8\n");
        assert(actual == required);
    }

}
