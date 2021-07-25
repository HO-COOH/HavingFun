# Reinvent STL
This project aims to reinvent the C++ STL library.

## Why
Because reinventing the wheel is sometimes the best way to learn.

## Goal
1. Use some of the foundamental part of standard library (like type_traits, dynamic memory allocation) to implement the main STL containers
2. Implement those foundamental part like type_traits, dynamic memory allocation with no dependency
3. Find and remove all `std` with my own namespace, and it should still works.

## "std::string's 11 Confusing Constructors"
Inspired by [this video by Jason Turner](https://youtu.be/3MOw1a9B7kc), I decided to give it a try to see whether we can "fix" it
using [NamedType Library](https://github.com/joboccara/NamedType).


