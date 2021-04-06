# OpenMP
Parallel with OpenMP

### Part 1 includes calculations of certain values from an array with OpenMP
### Part 2 includes a matrix matrix multiplication algorithm with OpenMP

## Introduction:
This project mainly focuses on parallelization using Openmp in C++. Openmp is a very convenient and very user-friendly paralleling tool compared to std::thread. The goal here is to use Openmp, instead of the Thread library, to solve the same two tasks of the Thread project.

## System Model:
Every task is run on the RHEL7 interactive desktop. Unless specified, the core number is set to 20 by default for each test run. L1, L2, and L3 cache sizes are 32KB, 256KB, 30720KB respectively. Memory size is set separately to 1GB in the pbs file. 

We also need to include the omp.h header file and add -fopenmp flag to makefile.

## Problem Statement:
Same as the Thread Project.

## OpenMP:
OpenMP uses a portable model to give programmers a simple interface of parallelization. It is much easier to use OpenMP for parallelizing compared to the Thread library, because OpenMP will take care of all the troublesome threads management and data consistency.

OpenMP is also friendly to programmers who need to understand and maintain other people’s code. Since what we do is only adding OpenMP directives directly to serial implementation, the code readability is unchanged. In fact, when we turn off the openmp switch, the compiler will compile the same code perfectly as serial implementation. 

## Part 1:
### _A. Non-associativity of floating-point calculation_
When calculating two floating-point numbers A and B, we don’t always get the same result. For example,  and  don’t give the same result as expected from regular arithmetic principles. If we look into the two results, the difference often doesn’t show up until after many digits after the decimal point. This means the two results effectively have the same value. However, because binary encoding for floating-point numbers is so different from decimal representation, the computer will always recognize them as two different numbers. This requires us to implement a comparison function for floating-points, so that within a certain range of error, ε, the function will give a positive answer. We implemented such a function in main.cpp.
### _B. Paralleling_
Part 1 has 3 subroutines, that is,  finding mean, finding sum, finding top values. We parallel the code base on the original serial code by adding OpenMP directives directly. Unlike the Thread project, we don’t need to address the issue when N is not a multiple of P, because OpenMP automatically deals with it. 
### _C. OpenMP and Thread_
![Table1](https://github.com/FrankYufeiYang/OpenMP/blob/main/table1.jpg)

But all the automatization came with a cost. When data size is relatively small, namely, in the range of L1 cache and L2 cache, the execution time of OpenMP is much longer than the execution time of serial code. Recall that from the Thread project, the execution time for threaded part 1 and part 2 are 0.001580 and 0.001195. The result from Table 1 shows that the OpenMP execution time for part 1 is about 10 times longer than part 1 from the Thread project; part 2 is about 5 times longer than part 2 from the Thread project.

This shows that, although OpenMP has all the advantages over the Thread library, when data size is small and threading overhead doesn’t outweigh memory latency, OpenMP has a big disadvantage in terms of performance compared to the Thread library. This makes sense, because the inventors of OpenMP would need to make sure their module works in different environments and situations, and behind the scene, there must be a lot of extra effort to ensure that. 
### _D. Scalability_
Let’s see whether OpenMP gets worse by testing part 1 again with 4 threads. The result is presented in Figure 1.
![Figure 1](https://github.com/FrankYufeiYang/OpenMP/blob/main/figure1.jpg) 

We can see that when data size is small, OpenMP spends so much time on parallelization, and execution time of serial code is only a small fraction of it.

Let’s also run other tests with different P and N. From the Thread project, we learned that having 20 threads with our 20 cores system should result in the best performance. However, that is not the case with OpenMP. Observing from Figure 2, we find that, with OpenMP, a smaller number of threads actually results in better performance. What is more interesting is that, when P is 20 and above, the data size doesn’t even affect the performance so much. In fact, their execution time stays on the same level. 

![Figure 2](https://github.com/FrankYufeiYang/OpenMP/blob/main/figure2.jpg)

So, after running more tests and observing from the results, my conclusion is, unlike the Thread library, the Openmp module doesn’t have the best performance when the number of threads is the same as the number of cores. Instead, when P and N are at their middle ground, that is, they are both not too relatively small and relatively large, OpenMP gives the best performance. We will use this phenomenon on task 2 below.  

## Part 2:
Parallelizing task 2 is also straightforward. One thing different is that we can try to add the collapse directive in hope of better improvement on the nested for loops.

![Figure 3](https://github.com/FrankYufeiYang/OpenMP/blob/main/figure3.jpg)

Applying what we just observed, we choose a middle-ground number for P and N, that is, we test the performance of different matrix multiply algorithms with N as 1000 and P as 10. We find that, although serial optimization with parallelization still results in the best performance, paralleling the original code directly with OpenMP doesn’t result in second best performance anymore. The serially optimized code comes in the second place. This phenomenon aligns with what we have been discovering about OpenMP.

## Conclusion:
Although OpenMP is very easy to use, it may not give us the best performance of parallelization. In certain cases, especially when we are chasing the extremely optimized performance, we might need to use the Thread library instead. But I would imagine OpenMP vs. Thread library as Auto-threading vs. Manually-threading, that’s saying, under most situations OpenMP is still our best choice. 
