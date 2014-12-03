ROmics
======

High-performance genomic analysis tools for R.

General description
======
ROmics is intended to be a set of efficient routines for the analysis of large genomic datasets. The software relies on C backend tunned routines called from R.

On its first version, the software is extended to multicore CPU multicore machines based on the shared memory paradigm. In other words, you can exploit your 4-core Intel i7 machines out of the box.

Projected schedule
======
The version (V1.0) will be released with support for CCA, RA and GCCA algorithms on multicore machines.

The next major release (V2.0) it will be focused on the use of GPU accelerators using CUDA. Probably the use of multi-GPU configurations will be also targeted.

Next versions will be probably extended with MPI support, but there are also another tracks to follow... 

Requisites
=======
For the parallel flavour of version V1.0:
Linux-based operating system (Mac or Linux)
C compiler, OpenMP capable. (see C compilers section)
OpenBLAS libraries. (see Installing OpenBLAS section)

Installing OpenBLAS
=======
The OpenBLAS installation is pretty straightforward. You can install the sotfware using your system package browser (yum, apt, yast..) but I recommend you to compile it (it will give you better performance)

If you're used to install and compile this type of libraries, well, just read the README file and tune the Makefile properly. If not, just type 'make' and then 'sudo make install'. This will create a directory tree under /opt/OpenBLAS, and you're done.







