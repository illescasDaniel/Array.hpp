# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.7--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks

Insert & delete values **-O0**:

std::vector push_back: 0.827s  
evt::**Array** Append: **0.23s**  
std::vector Removal: 0.588s  
evt::**Array** Removal: **0.126s**  
std::vector Insert at beginning: 1.591s  
evt::**Array** Insert at beginning: **1.588s**  
std::vector Insert at end: 1.285s  
evt::**Array** Insert at end: **0.443s**  
  
  
**-O2:**  
  
std::vector push_back: 0.179s  
evt::**Array** Append: **0.077s**  
std::vector Removal: 0s  
evt::**Array** Removal: **0.071s**  
std::vector Insert at beginning: 1.571s  
evt::**Array** Insert at beginning: **1.582s**  
std::vector Insert at end: 0.186s  
evt::**Array** Insert at end: **0.183s**  
