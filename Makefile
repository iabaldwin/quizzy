all: strings entropy matching modeling

# Strings
# Write a program that takes a path to a file containing a list of words (e.g.
# "/usr/share/dict) and count the number of palindromic words in the list.
# A palindromic word (for this problem) is a word that reads the same in the
# forwards and reverse order, and is case-sensitive (i.e. Z != z). Report total
# count of palindromic words
strings:	strings.cc
					g++ strings.cc -std=c++14 -o strings

# Entropy
# Compute an estimate of the discrete entropy of a given source of random
# information (e.g.  "/dev/random"). Ensure that the random source provides
# integers ranging from [0,128], and assume that the input probability space is
# divided into 16 events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in
# bits.
entropy:	entropy.cc
					g++ entropy.cc -std=c++14 -o entropy

# Matching
# You are given three sets of image patches, corresponding to portions of
# a normalized grayscale image (i.e. all values are from 0.0-1.0, as opposed to
# 0-255). For the reference patch, which is the closest candidate patch? (For
# your definition of closeness).
matching: matching.cc
					g++ matching.cc -std=c++14 -o matching `pkg-config opencv --cflags --libs`

# Modeling
# You are given a list of input data points corresponding to
# a independent/depenent variable pair. Estimate the relationship between the
# dependent and independent variable with a polynomial model of your choice, and
# report how well that model fits (the residual error).
modeling:	modeling.cc
					g++ modeling.cc -std=c++14 -o modeling -g `pkg-config opencv --cflags --libs`

clean:
	rm strings entropy matching modeling
