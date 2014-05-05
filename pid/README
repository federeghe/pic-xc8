PID Library
===========

This library contains a standard ISA PID with 2 degrees of freedom.

How to use
----------
Using this library is very simple, there are 2 function:
- pid_init
- pid_data

Also if you want a tracking feature, uncomment the relative costant in the header.
Tracking feature adds also 2 functions:
- pid_tracking_mode
- pid_tracking_data

See _pid.h_ for details.

### How to tuning ###
To set correctly K, Ti, Td, N, w_prop, w_deriv, etc. you need to *tune* your PID. This can be
hard. Methods to tune PID are a lot and depend on application. On web you can find a lot of resources
about that.


Testing
-------
Successfully tested on this devices:
- PIC18F2550
- * Probably it works in entire PIC18 family with sufficient memory, because that does not use peripherals 
  or other specific PIC option.

Thanks
------
The algorithm is derived from an exercise done by prof. Alberto Leva (http://home.deib.polimi.it/leva/) in 2013. 
In addition, the reference book is: "Fondamenti di controlli automatici",Bolzern,McGraw-Hill,978-88-386-6434-2

