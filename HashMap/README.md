*This file is not finished, that is why there isn't a test for it yet.

TODO:  (1) hashmap_insert() has to be able to handle duplicates 
         --if value is copy type need to just replace the value in the values array
         --if the value is non-copy (heap allocated) need to free
         
IDEAS: (1) -- maybe pass a function pointer to a free'ing function, pass in NULL if values are copy types
           -- create a heap allocated copy and return that to the caller who must free
           
