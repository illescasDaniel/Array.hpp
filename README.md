# Array.hpp

[![Version](https://img.shields.io/badge/version-v1.3.1--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/print.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Also offers a little bit better performance (see [Benchmark](#Benchmark)).  
It uses a unique_ptr<> to automatically manage the memory.

## Benchmarks (-O0)

Insert & delete 30 million values:

```std::vector Insertion: 0.737s  

evt::**Array** Insertion: **0.485s**

std::vector Removal: 0.581s

evt::**Array** Removal: **0.305s**```
