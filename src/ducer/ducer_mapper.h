#pragma once

namespace nsducer {

auto mapper = [](auto map){
    return [=](auto step) {
        return stateless(
            [=](auto s, auto v){
                return step(s, map(v));
            },
            [=](auto s){return step(s);});
    };
};


}