#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <cassert>

#include "ducer_arities.h"
#include "ducer_filterer.h"
#include "ducer_mapper.h"
#include "ducer_summer.h"
#include "ducer_taker.h"
#include "ducer_println.h"
#include "ducer_pass.h"
#include "ducer_transduce.h"
#include "ducer_into.h"
#include "ducer_compose.h"

namespace nsducer {

template<class Ducer = decltype(pass())>
struct ducer_type : public Ducer
{
    ducer_type() : Ducer(pass()) {}
    ducer_type(Ducer d) : Ducer(d) {}

    template<class Pred>
    auto filter(Pred pred) {
        auto composed = compose(*this, filterer(pred));
        return ducer_type<decltype(composed)>(composed);
    }

    template<class Map>
    auto map(Map map) {
        auto composed = compose(*this, mapper(map));
        return ducer_type<decltype(composed)>(composed);
    }

    auto sum() {
        auto composed = compose(*this, summer());
        return ducer_type<decltype(composed)>(composed);
    }

    template<class Stream>
    auto println(Stream& stream) {
        auto composed = compose(*this, nsducer::println(stream));
        return ducer_type<decltype(composed)>(composed);
    }

    template<class T, class Collection>
    auto to_vector(Collection collection) {
        return into(*this, std::vector<T>(), collection);
    }

    template<class To, class Collection>
    auto into(To to, Collection collection) {
        return nsducer::into(*this, to, collection);
    }
};

auto ducer = ducer_type<>();

}

