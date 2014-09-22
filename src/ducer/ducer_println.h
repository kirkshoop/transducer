#pragma once

namespace nsducer {

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

}