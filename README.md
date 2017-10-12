# **Array**.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.24.1-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax and performance.  
```diff
- Note: if the Array store objects of a Class, that class needs to have a default constructor.
```

## Benchmarks

**-O0**:

std::vector push_back: 0.688659s<br>
std::vector emplace_back: 0.716044s<br>
evt::**Array** Append: **0.20434s**<br>
std::vector Removal: 0.545444s<br>
evt::**Array** Removal: **0.225306s**<br>
std::vector Insert at beginning: 1.58625s<br>
evt::Array Insert at beginning: 1.6258s<br>
std::vector Insert in the middle: 2.13889s<br>
evt::Array Insert in the middle: 2.14317s<br>
std::vector Insert at end: 1.26026s<br>
evt::**Array** Insert at end: **0.421712s**<br>

- TEST Class Benchmark (int, double, string)

std::vector push_back: 7.47206s<br>
std::vector emplace_back: 7.25572s<br>
evt::**Array** Append: **7.00413s**<br>

**-Ofast:**

std::vector push_back: 0.155405s<br>
std::vector emplace_back: 0.147155s<br>
evt::**Array** Append: **0.0795583s**<br>
std::vector Removal: 4.4e-08s<br>
evt::Array Removal: 0.12843s<br>
std::vector Insert at beginning: 1.61507s<br>
evt::**Array** Insert at beginning: **1.5651s**<br>
std::vector Insert in the middle: 2.10976s<br>
evt::Array Insert in the middle: 2.203s<br>
std::vector Insert at end: 0.209195s<br>
evt::**Array** Insert at end: **0.181815s**<br>

- TEST Class Benchmark (int, double, string)

std::vector push_back: 4.9516s<br>
std::vector emplace_back: 4.81552s<br>
evt::**Array** Append: **4.40035s**<br>


## [API Reference](https://illescasdaniel.github.io/Array.hpp/)

<img src="Array.png" width="800">
