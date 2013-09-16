ELF Reader and Loader
=====================
Project link: http://www.cs.purdue.edu/homes/grr/cs354/lab1-elfinfo/

* elfinfo.c: an ELF executable file reader.
* loader.c: a simple loader to load the program in memory and run it.
* compare.sh: compare the output of the file reader elfinfo with elfdump.
* runprog.sh: load programs with loader.


Setup:
------
Add the path to the gcc version 3 compiler and make files:
```
export PATH=/opt/csw/gcc3/bin:$PATH
make
```


Run the programs:
-----------------
```
compare.sh
runprog.sh
```

