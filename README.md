# About

Computes the longest common substring of two strings.

# Compiling

```
git submodule update --init
cd libdivsufsort
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="Release" -DBUILD_DIVSUFSORT64=1 ..
make
cd ../..
make
```

# Running

```
./LCSS example/file1.txt example/file2.txt
```

Prints two lines. On the first line, three space-separated integers: the length of the longest match, starting position in file1.txt, starting position in file2.txt. On the next line, prints the longest match as a string.

# Restrictions

The files may not contain the characters '$' or '#', which are used internally as separator characters.