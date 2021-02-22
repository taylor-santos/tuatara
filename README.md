# tuatara
[![Build Status](https://travis-ci.com/taylor-santos/tuatara.svg?branch=trunk)](https://travis-ci.com/taylor-santos/tuatara)
[![Coverage Status](https://coveralls.io/repos/github/taylor-santos/tuatara/badge.svg?branch=trunk&kill_cache=1)](https://coveralls.io/github/taylor-santos/tuatara?branch=trunk)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/21962/badge.svg)](https://scan.coverity.com/projects/taylor-santos-tuatara)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/6581787e6f1f47de88382f1300dea83b)](https://app.codacy.com/gh/taylor-santos/tuatara?utm_source=github.com&utm_medium=referral&utm_content=taylor-santos/tuatara&utm_campaign=Badge_Grade)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/taylor-santos/tuatara.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/taylor-santos/tuatara/context:cpp)

Compiler for the Tuatara programming language

## Dependencies
*   CMake >= 3.15
*   Bison >= 3.6.1 
*   Flex >= 2.6

## Installation
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```