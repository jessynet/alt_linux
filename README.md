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
2) You will need libcurl-devel, librpm-devel,nlohmann-json-devel, libghc_filesystem-devel packages (these are the names of the packages in Alt Linux, in other OS they may differ), which can be installed using a package manager or downloaded from the corresponding repositories. You also need the g++ compiler.
3) Go to the directory with source codes.
4) Run make -f Makefile in the source codes directory to build the shared library  and an executable file that uses that library.
5) Delete the resulting object file from the source codes directory: make -f Makefile clean.
6) To run the utility, simply use the «./compare» command with the necessary parameters. For help use "./compare -h" or "./compare --help"


