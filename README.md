# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.7.2--beta-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax.  
Offers better performance than `std::vector` (see [Benchmark](#Benchmark)) and it uses `unique_ptr<>` to automatically manage the memory.

## Benchmarks

Insert & delete values **-O0**:

std::vector push_back: 0.815s
evt::**Array** Append: **0.235s**
std::vector Removal: 0.609s
evt::**Array** Removal: **0.119s**
std::vector Insert at beginning: 1.601s
evt::**Array** Insert at beginning: **1.594s**
std::vector Insert at end: 1.335s
evt::**Array** Insert at end: **0.457s**
  
  
**-O2:**  
  
std::vector push_back: 0.172s
evt::**Array** Append: **0.072s**
std::vector Removal: 0s
evt::**Array** Removal: **0.069s**
std::vector Insert at beginning: 1.638s
evt::**Array** Insert at beginning: **1.665s**
std::vector Insert at end: 0.177s
evt::**Array** Insert at end: **0.178s**
