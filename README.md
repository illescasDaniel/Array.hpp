# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.4--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers a bit better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks (-O0)

Insert & delete values:

std::vector push_back: 0.805s  
evt::**Array** Append: **0.268s**  
std::vector Removal: 0.589s  
evt::**Array** Removal: **0.222s**  
std::vector Insert at beginning: 1.616s  
evt::**Array** Insert at beginning: **1.595s**  
std::vector Insert at end: 1.423s  
evt::**Array** Insert at end: **0.533s**  
