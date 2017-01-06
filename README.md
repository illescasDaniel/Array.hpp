# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.2.1--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers a little bit better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks (-O0)

Insert & delete 30 million values:

std::vector Insertion: 0.737s  
evt::**Array** Insertion: **0.485s**  
std::vector Removal: 0.581s  
evt::**Array** Removal: **0.305s**  
