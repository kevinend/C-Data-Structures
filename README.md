# Generic-C-Data-Structures

This repository contains sample generic containers/data structures implemented in plain old C.

All examples were compiled with gcc on linux (ubuntu) with the following flags:
  gcc -Wall -Werror -fno-common -std=c99 
 
Included are valgrind outputs for a few of the data structures. I tried to test all the functionality included in each file and in sequences that would be prone to leaking memory (such as popping a stack then immediately pushing right after). This doesn't guarantee that the structures are leak-proof. I just wanted to ensure that they weren't leaking memory in obvious ways.

As always, the following code definitely contains bugs. 

