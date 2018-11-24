# Short coding puzzles

## Strings
Write a program that takes a path to a file containing a list of words (e.g.
"/usr/share/dict) and count the number of palindromic words in the list.
A palindromic word (for this problem) is a word that reads the same in the
forwards and reverse order, and is case-sensitive (i.e. Z != z). Report total
count of palindromic words

## Entropy
Compute an estimate of the discrete entropy of a given source of random
information (e.g.  "/dev/random"). Ensure that the random source provides
integers ranging from [0,128), and assume that the input probability space is
divided into 16 events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in
bits.

## Matching
You are given three sets of image patches, corresponding to portions of
a normalized grayscale image (i.e. all values are from 0.0-1.0, as opposed to
0-255). For the reference patch, which is the closest candidate patch? (For
your definition of closeness).

## Modeling
You are given a list of input data points corresponding to
a independent/depenent variable pair. Estimate the relationship between the
dependent and independent variable with a polynomial model of your choice, and
report how well that model fits (the residual error).

## Build instructions
1. `cd quizzy`
1. `mkdir build && cd build`
1. `conan install ..`
1. `cmake ..`
1. `make`
1. `make test` for testing Matrix library