# Sum of Absolute Differences

This example runs sum of absolute differences on a refernce matrix
and a frame matrix. Asuming parameters for the height and width of 
reference and frame matrixes, the result is a matrix with dimensions
(reference width - frame width + 1) x (reference height - frame height +1).

Element (x,y) in the result matrix, is the sum of absolute difference between
the frame, and the sub-matrix of the reference starting from position (x,y) with 
the same height and width as the frame. The purpose of this kernel is to find the 
porition of the reference matrix that has the most resemblence to the frame matrix.

The kernel code is located in the subdirectories of [kernel](kernel). The actual
sum of absolute differences code is in the header file
[kernel/include/sum_abs_diff.hpp](kernel/include/sum_abs_diff.hpp). 

# Makefile Targets

For a list of all Makefile targets, run `make help`.

## Versions

There are several different versions of this kernel. Each is a subdirectory in
the [kernel](kernel) directory.

### Version 0
In this version, each tile only performs one unit of work, i.e. calculating one
element of the result matrix. Based on the size of the result matrix, and the 
tile group dimensions, enough tile groups will be launched to populate the 
entire result matrix.


### Version 1

In this version, each tile performs multiple units of work. The workload share 
of each tile group is defined by block_size_x/y and passed to the kernel. Each 
tile then performs the kernel in a loop, with each iteration belonging to a 
unit of work (or vitual thread).


### Version 2

In this version, frame dimensions are fixed at 4 by 4.


### Version 3

In this version, the frame dimensions are templatized in the kernel, instead 
of being passed in as an input argument to the kernel. This gives the compiler
the opportunity to optimize based on frame dimensions known at compiler time.

