# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.6--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks (-O0)

Insert & delete values:

std::vector push_back: 0.824s  
evt::**Array** Append: **0.271s**  
std::vector Removal: 0.592s  
evt::**Array** Removal: **0.122s**  
std::vector Insert at beginning: 1.62s  
evt::**Array** Insert at beginning: **1.601s**  
std::vector Insert at end: 1.277s  
evt::**Array** Insert at end: **0.485s**  
