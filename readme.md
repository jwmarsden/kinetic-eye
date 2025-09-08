# Kinetic-Eye

This a personal repository of learnings. It currently includes; 

# Introduction

## Requirements

* C++23 (https://en.cppreference.com/w/cpp/23)
* CMake (https://cmake.org/)

## Internal Depdendencies

* spdlog for logging (https://github.com/gabime/spdlog) referenced from GitHub (https://github.com/gabime/spdlog.git)

# Build

## Get the Source

All of the required packages have been configured as CMake subdirectories which are managed as Git submodules. This makes our lives easy in many ways but it means that each of the dependencies needs to also be cloned. 

```
git clone --recursive https://github.com/jwmarsden/Kinetic-Eye.git
```

### Whoops, I didn't do recursive

```
➜  cd Kinetic-Eye
➜  Kinetic-Eye git:(main) ✗ git submodule update --init --recursive
```