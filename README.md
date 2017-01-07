# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.3--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers a little bit better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks (-O0)

Insert & delete values:

std::vector push_back: 0.847s
evt::**Array** Append: **0.526s**
std::vector Removal: 0.599s
evt::**Array** Removal: **0.308s**
std::vector Insert at beginning: 1.633s
evt::**Array** Insert at beginning: **1.619s**
std::vector Insert at end: 1.279s
evt::**Array** Insert at end: **0.975s**
