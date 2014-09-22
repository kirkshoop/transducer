#pragma once

namespace nsducer {

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

}