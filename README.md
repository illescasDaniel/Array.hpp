# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.7.2--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `std::unique_ptr<>` to automatically manage the memory.

## Benchmarks

Insert & delete values **-O0**:

std::vector push_back: 0.821s  
std::vector emplace_back: 0.724s  
evt::**Array** Append: **0.215s**  
std::vector Removal: 0.596s  
evt::**Array** Removal: **0.122s**  
std::vector Insert at beginning: 1.61s  
evt::**Array** Insert at beginning: **1.612s**  
std::vector Insert at end: 1.285s  
evt::**Array** Insert at end: **0.471s**  
  
  
**-O2:**  
  
std::vector push_back: 0.178s  
std::vector emplace_back: 0.14s  
evt::**Array** Append: **0.077s**  
std::vector Removal: 0s  
evt::**Array** Removal: **0.074s**  
std::vector Insert at beginning: 1.577s  
evt::**Array** Insert at beginning: **1.589s**  
std::vector Insert at end: 0.184s  
evt::**Array** Insert at end: **0.181s**  
