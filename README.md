Implementation of HotRod client in C++. Client can comunicate with intelligence 0x01, 0x02, 0x03.
==============

Start server
--------------
1 server
```bash
make startserver  #to run infinispan server
```

or

Distribution servers
```bash
make startservers  #to run distribution servers

make kill  #to kill all java programs (servers)
```


Run client´s tests
--------------
```bash
make tests
```

Examples
--------------
See /examples directory
You can run them via makefile

```bash
make basic_example
make bulk_example
make rmmap_example
make advanced_example
make rmmap_advanced_example
make not_thread_safe_example
```
