#pragma once

namespace nsducer {

auto transduce = [](auto ducer, auto reduce, auto init, auto collection) {
    auto step = ducer(reduce);
    for(auto& v : collection) {
        init = step(init, v);
    }
    return step(init);
};

}
