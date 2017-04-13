Unlike the previous files this does not use a void *. 

I tried creating a wrapper type that would simulate a generic intrusive container.

It seems to work fine, though to pass compilation/linking I had to put the implementation within the header file
as well as have the user create the wrapper type before including s_linked_list.h.

Those two drawbacks, may be enough to justify not doing something similar like that in the future.
