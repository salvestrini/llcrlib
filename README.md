
Overview
--------

The <b>L</b>ow-<b>L</b>evel(ANSI C) <b>C</b>ommon Distributed Application Protocol(CDAP) and <b>R</b>esource Information Base (RIB) library, 
or `llcrlib`, provides  (or better, _should provide_) a basic implementation of the CDAP/RIB library in ANSI C.

Current version
---------------

The current status is in its very initial design, draft-0, (i.e. RFC). In short, it is a bunch of (buggy) header files to discuss the design and goals of the libraries.

### Current status (draft-0)

* CDAP connection stablishment missing, and in particular CACEP necessary input (e.g. authentication) => requires work in CDAP sub-lib.
* RIB association to an AE is not yet clear => requires work in CDAP sub-lib.
* Basic built-in types are _very_ incomplete.
* 'Multiple inheritance', or multiple objects with different `class_name`s in the same position of the tree is tricky and requires further study. However APIs support it.
* `rib/rib_remote.h` operations are incomplete, yet basic `` 
* `Scope` is not drafted yet
* `Filter` is intentionally not implemented.
* Adaptation to the terminology of the spec. Try to be as close as possible
* Interceptor(s) prototype needs to be drafted
* Necessary built-in 'register to events' functionality not yet drafted
* Revise and simplify APIs
* Many many other things...

Architecture
------------
```
            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
            x                                                                         x
          | x  +-----------+     +-----------+                          |             x
          | x  |           |     |           +-------------------------->             x
          | x  |           | (b) |           |               (d)        |             x
     RINA <----+           +----->           <--------------------------+ Application x
Operating | x  |           |     |           |                          | logic       x
   system |    |           |     |           |    ..................    |             x
     APIs |(a) |   CDAP    |     |    RIB    |    .                .    |             x
          |    |  library  |     |  library  +----> * Higher level +---->             x
          | x  |           | (c) |           |    .   frameworks   .    |             x
          +---->           <-----+           |(d1).                .(d2)|             x
          | x  |           |     |           |    . * Other lang   .    |             x
          | x  |           |     |           <----+   bindings     <----+             x
          | x  |           |     |           |    .                .    |             x
          | x  +-----------+     +-----^-----+    ..................    |             x
            x                          |                                |             x
            x                          | (o1)                                         x
            x                   ...............                                       x
            x                   . Interceptor .                                       x
            x                   ...............                                       x
            x                                                                         x
            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

     ....  Optional modules
     ----  Libraries
     +-->  APIs
     xxxx  OS application boundary 
```

The llcr library consists of two main sub-libraries that can be used together or separately:

* `llcr-cdap`: CDAP library. Provides basic CDAP encoding/decoding of messages. The library expects a RIB provider to link against, see `cdap/rib_provider.h`.
* `llcr-rib`: RIB library. This library is meant to provide an easy to use (hopefully) API to manage CDAP RIBs. This library expects a cdap provider behind, see `rib/cdap_provider.h` for more details.

When using the llcr-rib, one can build higher level framworks or other language bindings, to simplify the application logic.

The `Interceptor`(s) are modules that can intercept incoming remote operations before being passed to the application - (d) or (d1) depending on the approach-. Some examples of interceptors can be ACLs or debug loggers. 

Copyright
---------

TODO

PRISTINE FP7
