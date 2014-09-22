#pragma once

namespace nsducer {

template<class Ducer>
auto compose(Ducer ducer) {
    return ducer;
}

template<class Ducer0, class... DucerN>
auto compose(Ducer0 ducer0, DucerN... ducerN) {
    return [=](auto step) {return ducer0(compose(ducerN...)(step));};
}

}
