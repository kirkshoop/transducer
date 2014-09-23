#pragma once

namespace nsducer {

auto println = [](auto& stream){
    auto pointer = std::addressof(stream);
    return [=](auto step) {
        typedef decltype(pointer) Pointer;
        return stateful(
            pointer,
            [=](auto s, Pointer stream, auto v){
                *stream << v << std::endl; return step(s, v);
            },
            [=](auto s, Pointer){return s;});
    };
};

}
