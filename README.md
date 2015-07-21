# project fuzzyjs
This is a fully embeddable javascript interpreter written in c++. It is not ECMAScript compliant, is not efficient, probably shouldn't be used in a real situation. The benefit, however, is that this project has _absolutely no dependencies._ You do not even have to include stdio.h! The only requirement is that you implement **malloc** and support of the **new** keyword.

## Why is this a thing?
I want to utilize javascript in a baremetal project of mine. As such, I don't have access to any of the standard libraries which is an odd restriction. I couldn't find any project that was completely standalone, so I decided to try my hand at rolling my own. The scope of fuzzyjs is not overly ambitious and I do not expect to ever support full ECMAScript compliance. But who knows?

## How do I use this?
Look at fjs.cpp for a small demonstration, but essentially just include "fjs.h" and then you have access to the JSParser object. Instantiate it and call the _parse_ method which takes a char* of code and away you go! There are also methods to register c++ functions for use in javascript (per the demo).

## What is supported?
The following language features are currently supported (this list will be updated as I add more stuff).

- variable assignment
- function declaration
- c++ interop "delegates"
- delegate invocation
- evaluate simple expressions (pretty much only == is currently supported)
- string concatenation
- javascript functions
- the return keyword!
- object creation via: prototypes
- object instantiation (minus the constructor stuff)
- anonymous functions as parameters
- basic math: plus, minus, multiply (division is a different beast that I will implement later)

More to come!


