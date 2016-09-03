# tsp_impl

## how to use

- clone this repository.

- compile

```sh
$ cd tsp_impl
$ make
```

- call ```tsp_impl``` with option.

```
$ ./tsp_impl

Usage: tsp_impl [ -l ][ -a ] -t <tsplib_file>

  -t tsplib_file    test data that included tsplib
  -l                list of algorithms and tsplib files
  -a name           define target algorithm (default: brute force)
  -h                print this help

One of -l or -t must be specified.
```

## Test case
Download TSPLIB data.

[MP-TESTDATA - The TSPLIB Symmetric Traveling Salesman Problem Instances](http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/)
