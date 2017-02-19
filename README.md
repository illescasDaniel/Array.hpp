# Array.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=dev)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.13.5--dev-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENSE) 

Container to store data, similar to `std::vector` but with better syntax and performance.  
```diff
+ Note: this versions should be compatible with all c++ versions 
```

## Benchmarks

Insert & delete values **-O0**:

std::vector push_back: 0.798s  
std::vector emplace_back: 0.783s  
evt::**Array** Append: **0.153s**  
std::vector Removal: 0.62s  
evt::**Array** Removal: **0.112s**  
std::vector Insert at beginning: 1.627s  
evt::**Array** Insert at beginning: **1.648s**  
std::vector Insert at end: 1.426s  
evt::**Array** Insert at end: **0.295s**  

**-O2:**  

std::vector push_back: 0.175s  
std::vector emplace_back: 0.134s  
evt::**Array** Append: **0.076s**  
std::vector Removal: 0s  
evt::**Array** Removal: **0.067s**  
std::vector Insert at beginning: 1.578s  
evt::**Array** Insert at beginning: **1.578s**  
std::vector Insert at end: 0.197s  
evt::**Array** Insert at end: **0.163s**  
