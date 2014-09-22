#pragma once

namespace nsducer {

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

}