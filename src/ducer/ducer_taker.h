#pragma once

namespace nsducer {

auto taker = [](long count){
    return [=](auto step) {
        return stateful(
            count,
            [=](auto s, long& count, auto v){
                if (--count) {return step(s, v);} return s;
            },
            [=](auto s, long& count){return s;});
    };
};

}