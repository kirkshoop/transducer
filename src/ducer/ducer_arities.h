#pragma once

namespace nsducer {

template<class Next, class Complete>
struct stateless_s
{
    Next next;
    Complete complete;

    stateless_s(Next next, Complete complete) : next(next), complete(complete) {}

    // complete
    template<class Acc>
    Acc operator()(Acc acc) const { return complete(acc);}
    // next
    template<class Acc, class T>
    Acc operator()(Acc acc, T v) const { return next(acc, v);}
};

auto stateless = [](auto next, auto complete){
    return stateless_s<decltype(next), decltype(complete)>{next, complete};
};

template<class State, class Next, class Complete>
struct stateful_s
{
    std::shared_ptr<State> state;
    Next next;
    Complete complete;

    stateful_s(State state, Next next, Complete complete) : state(std::make_shared<State>(state)), next(next), complete(complete) {}

    // complete
    template<class Acc>
    auto operator()(Acc acc) const { return complete(acc, *state);}
    // next
    template<class Acc, class T>
    Acc operator()(Acc acc, T v) const { return next(acc, *state, v);}
};

auto stateful = [](auto state, auto next, auto complete){
    return stateful_s<decltype(state), decltype(next), decltype(complete)>{state, next, complete};
};

}
