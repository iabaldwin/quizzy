all:  entropy

# Entropy
# Compute an estimate of the discrete entropy of a given source of random
# information (e.g.  "/dev/random"). Ensure that the random source provides
# integers ranging from [0,128], and assume that the input probability space is
# divided into 16 events [0,...,8) -> 0, [8,...16) -> 1, etc. Report answer in
# bits.
entropy:	src/entropy.cc
					g++ src/entropy.cc -std=c++14 -o entropy


clean:
	rm entropy
