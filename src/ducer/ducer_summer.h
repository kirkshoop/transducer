#pragma once

namespace nsducer {

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

}