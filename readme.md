transducer experiments in c++
=============================

References:
- Rich Hickey @ StrangeLoop [YouTube](https://www.youtube.com/watch?v=6mTbuzafcII)
- Rich Hickey [Blog](http://blog.cognitect.com/blog/2014/8/6/transducers-are-coming)
- James Long [Blog](http://jlongster.com/Transducers.js--A-JavaScript-Library-for-Transformation-of-Data)

Example
=======
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
