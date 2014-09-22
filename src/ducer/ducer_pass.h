#pragma once

namespace nsducer {

auto pass = [](){
    return [=](auto step) {
        return step;
    };
};

}