![v2019 001](../master/ivm.svg)

# IVM(R) Matrice Library ![v1.00 SZL](../master/version.svg)

A libary for numerical computation, includes basic linear algebra (vector, matrix, linear system) operations, auto-differentiation, and non-linear optimization, etc. The library contains two parts, the first part is a Fortran kernel library which supplies the basic linear arithmatic operations; the second part is a modern C++ library which aims to implement efficient and elegant numeric computation.

## Features
* Unified memory and basic type management.
* Automatic inference of optimal type according to the context.
* Expression system enhanced lazy evaluation.

## System Requirements
Matrice supports Intel 64 architecture and compatible architectures.
The library is optimized for the systems based on
* Intel(R) Core(TM) processor - 5th, 6th, 7th, and 8th generation
* Intel(R) Xeon(R) processor E5 v3 family (formerly Haswell)

and compatible processors.

The software dependencies are:
* C++ compiler with C++11 standart support (at least)
* Optinal dependencies:
  * Intel OpenMP
  * CUDA 8.0 or later
