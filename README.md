# Generic-C-Data-Structures

This repository contains sample generic containers/data structures implemented in plain old C.

All examples were compiled with gcc on linux (ubuntu) with the following flags:
  gcc -Wall -Werror -fno-common -std=c99 
 
Included are valgrind outputs for a few of the data structures. I tried to test all the functionality included in each file and in sequences that would be prone to leaking memory (such as popping a stack then immediately pushing right after). Testing doesn't guarantee that the structures are leak-proof, just that they aren't leaking memory in obvious ways.

I am sure there are bugs in the following code...just par for the course.

