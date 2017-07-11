# **Array**.hpp

[![Build Status](https://travis-ci.org/illescasDaniel/Array.hpp.svg?branch=master)](https://travis-ci.org/illescasDaniel/Array.hpp)
[![Version](https://img.shields.io/badge/version-v1.21-green.svg)](https://github.com/illescasDaniel/Array.hpp/releases)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/illescasDaniel/Array.hpp/blob/master/LICENCE) 

Container to store data, similar to `std::vector` but with better syntax and performance.  
```diff
- Note: if the Array store objects of a Class, that class needs to have a default constructor.
```

## Benchmarks

**-O0**:

std::vector push_back: 0.688659s
std::vector emplace_back: 0.716044s
evt::**Array** Append: **0.20434s**
std::vector Removal: 0.545444s
evt::**Array** Removal: **0.225306s**
std::vector Insert at beginning: 1.58625s
evt::Array Insert at beginning: 1.6258s
std::vector Insert in the middle: 2.13889s
evt::Array Insert in the middle: 2.14317s
std::vector Insert at end: 1.26026s
evt::**Array** Insert at end: **0.421712s**

- TEST Class Benchmark (int,double,string)

std::vector push_back: 7.47206s
std::vector emplace_back: 7.25572s
evt::**Array** Append: **7.00413s**

**-Ofast:**

std::vector push_back: 0.155405s
std::vector emplace_back: 0.147155s
evt::**Array** Append: **0.0795583s**
std::vector Removal: 4.4e-08s
evt::Array Removal: 0.12843s
std::vector Insert at beginning: 1.61507s
evt::**Array** Insert at beginning: **1.5651s**
std::vector Insert in the middle: 2.10976s
evt::Array Insert in the middle: 2.203s
std::vector Insert at end: 0.209195s
evt::**Array** Insert at end: **0.181815s**

- TEST Class Benchmark (int,double,string)

std::vector push_back: 4.9516s
std::vector emplace_back: 4.81552s
evt::**Array** Append: **4.40035s**


## API Reference

<img src="Array.png" width="800">
