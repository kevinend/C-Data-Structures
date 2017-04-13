Unlike the previous files this does not use a void * to simulate generics. 

I tried creating a wrapper type that would simulate a generic intrusive container.

To pass compilation/linking I had to put the implementation within the header file
as well as have the user create the wrapper type before including s_linked_list.h.

Also, you can only have one instance a list per compilation unit, otherwise symbols will clash
and you will end up with linking issues.

Those three drawbacks may be enough to justify not doing something similar like that in the future, though it was fun to try.
