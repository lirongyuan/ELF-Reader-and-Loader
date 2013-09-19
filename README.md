ELF Reader and Loader
=====================

There are several formats of executable files. The most common ones are a.out and elf. a.out is the oldest one and it is used mainly in BSD Unix.
``elfinfo`` is an ELF executable file reader that displays the header information and the symbol table contained in an elf file.

``loader`` is a loader to load an ELF executable file in memory and run it.

http://www.cs.purdue.edu/homes/grr/cs354/lab1-elfinfo/

Download
--------
```
$ git clone https://github.com/lirongyuan/ELF-Reader-and-Loader.git
```

Setup
-----
```
$ export PATH=/opt/csw/gcc3/bin:$PATH
$ make
```

Usage
-----
```
$ ./elfinfo prog-name
```

```
$ ./loader prog-name
```


Tests
-----
Compare the output of the file reader elfinfo with elfdump.
```
$ compare.sh
```

Load programs with loader.
```
$ runprog.sh
```



