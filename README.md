Implementation of HotRod client in C++.
==============
Client can comunicate with intelligence 0x01, 0x02, 0x03.

Start cache server
--------------
1 server
```bash
make startserver  #to run infinispan server
```

or

Distributed cache
```bash
make startservers  #to run Infinispan in distributed mode

make kill  #to kill all java programs (servers)
```


Run client´s tests
--------------
```bash
make tests
```

Examples
--------------
See /examples directory.
You can run them via makefile.

```bash
make basic_example
make bulk_example
make rmmap_example
make advanced_example
make rmmap_advanced_example
make not_thread_safe_example
```

Documentation
--------------
To generate doxygen documentation
```bash
make doc
```
