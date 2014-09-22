transducer experiments in c++
=============================

Transducers ignore the represenation of a sequence and instead build algorithm composition around the operations performed on each value.
The core pattern is based on std::accumulate. std::transform, std::copy_if and many others can be implemented in terms of std::accumulate.
```C++
template<class InputIterator, class OutputIterator, class UnaryFunction>
OutputIterator acc_transform(
  InputIterator first1,
  InputIterator last1,
  OutputIterator result,
  UnaryFunction func){
    return std::accumulate(first1, last1, result, [=](auto result, auto v){*result = func(v); return ++result;});
}
```

References:
- Rich Hickey @ StrangeLoop [YouTube](https://www.youtube.com/watch?v=6mTbuzafcII)
- Rich Hickey [Blog](http://blog.cognitect.com/blog/2014/8/6/transducers-are-coming)
- James Long [Blog](http://jlongster.com/Transducers.js--A-JavaScript-Library-for-Transformation-of-Data)

Status
======
- uses C++14 features.
- implemented transduce, into and compose
- filterer, mapper, summer and println
- the transducers are implementing two arities, next and complete. the arities are implemented with overloading.

ToDo
====
- add early-termination
- show merge, concat and map+merge

Builds
======

XCode
-----
```
mkdir projects/build
cd projects/build
cmake -G"Xcode" ../CMake -B.
```

OSX
---
```
mkdir projects/build
cd projects/build
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -B. ../CMake
make
```

Usage examples
==============

Functional style. As shown in Rich Hickey's talk.
```C++
    auto even = [](auto v){return v % 2 == 0;};

    auto inc = [](auto v){return ++v;};

    auto source = std::array<int, 4>({1,2,3,4});

    auto sum = into(
        compose(
            filterer(even),
            mapper(inc),
            summer(),
            println(std::cout)),
        0L,
        source);
```

Object style. Similar to the pattern used by Rx (Reactive Extentions) libraries.
```C++
    auto even = [](auto v){return v % 2 == 0;};

    auto inc = [](auto v){return ++v;};

    auto source = std::array<int, 4>({1,2,3,4});

    auto sum_of_inc_even = ducer<>().
        filter(even).
        map(inc).
        sum();

    auto sum = sum_of_inc_even.
            println(std::cout).
            into(0L, source);
```

Filterer
========

With C++14 auto lambdas, the filterer can look much like the impl in functional languages.
```C++
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
```

Stateless
=========

Uses overloading to implement 2 arity step function
```C++
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
```
