compare is a cli utility that takes as parameters the names of two branches to be compared and runs the necessary functions of the shared library libcompbranches.so for analysis.

The libcompbranches.so library performs the following tasks:
1) gets lists of binary packages of 2 branches.
2) compares the received packet lists and output JSON, which displays:
          - all packages that are in the 1st, but not in the 2nd
          - all packages that are in 2nd but not in 1st
          - all packages whose version-release is greater in the 1st than in the 2nd

Package lists are displayed for each architecture supported by the branch.

The generated JSON will be named output.json and stored in the folder where the utility is launched.

Building and installation instructions:

1) You need to download the source codes from the repository.
2) You will need nlohmann-json-devel, libghc_filesystem-devel packages (these are the names of the packages in Alt Linux, in other OS they may differ), which can be installed using a package manager or downloaded from the corresponding repositories. You also need the g++ compiler.
3) Copy the header file "filesystem.hpp" from the include/ghc directory (when installed via the package manager, the full path will look like /usr/include/ghc, when installing libghc_filesystem-devel manually, the path to the directory with header file may differ) to the sourse codes directory.
4) Go to the directory with source codes.
5) Run make -f Makefile-1 in the source codes directory to build the shared library.
6) Copy the resulting shared library  from the source codes directory to the standard libraries directory /usr/lib (run the command with superuser privileges).
7) Run the «ldconfig» command (in superuser mode).
8) Remove the library from the source codes directory: rm -f ./libcompbranches.so.
9) Delete the resulting object file from the source codes directory: make -f Makefile-1 clean.
10) Run make -f Makefile-2 in the source directory to build the executable.
11) The resulting executable file «compare» is manually copied from the source codes directory to the /usr/local/bin directory (running the command with superuser privileges).
12) Delete the resulting executable file from the source code directory: rm -f ./compare.
13) To run the utility, simply use the «compare» command with the necessary parameters.


