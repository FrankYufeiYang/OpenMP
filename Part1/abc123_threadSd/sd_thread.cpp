// author Yufei Yang
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sd_thread.h"
#include <omp.h>


using namespace std;

STDDEV_RESULT* calcSdThread(double *A, long N, int P)
{
    struct STDDEV_RESULT* res = new STDDEV_RESULT;
    
    double sd_temp, mean, min, max, sd;
    
    min = RAND_MAX;
    max = 0.0;
    sd = 0;
    sd_temp = 0;
    mean = 0;
	// // perform the summation for the mean

	omp_set_num_threads(P);
	long i=0;
	#pragma omp parallel
	{
	#pragma omp for private(i) reduction(+:mean) 
	for( i = 0; i < N; i++){
		mean = mean+A[i];
	}

	//use single since we only need calculate mean once
	#pragma omp single
	{
		mean /= (double) N;
	}
	

	// perform the summation for the std_dev
	#pragma omp for private(i) reduction(+:sd_temp)
	for( i = 0; i < N; i++)
	{
		sd_temp += (A[i] - mean) * (A[i] - mean);
	}	

	//use single since we only need calculate mean once
	#pragma omp single
	{
	sd=sqrt(sd_temp/(double)N);
	}
	
	// find min and max
	#pragma omp for reduction(max:max) reduction(min:min)
	for( i = 0; i < N; i++)
	{
		if(max < A[i])
		{
			max = A[i];
		}
		if(min > A[i])
		{
			min = A[i];
		}
	}
	}
	// store off the values to return 
	res->mean = mean;
	res->min = min;
	res->max = max;
	res->stddev = sd;
	
    return res;
}

THRESH_RESULT *findThreshValuesThread(double *A, long N, double T, int P)
{
	THRESH_RESULT *p_tmpResult = new THRESH_RESULT;
	
	// traverse the list once to find the count of values over threshold
	long c = 0;
	omp_set_num_threads(P);
	#pragma omp parallel
	{
		// long tmp = 0;
		#pragma omp for reduction(+:c)
		for (long i=0; i < N; i++)
		{
			if (A[i] > T)
				c++;
		}
	
	// store the count and allocate an array to store the results
	#pragma omp single
	{
	p_tmpResult->li_threshCount = c;
	p_tmpResult->pli_list = new long[c];
	c = 0;
	}
	

	// store the index locations of the values over threshold
	#pragma omp for
	for (long i=0; i < N; i++){
		if (A[i] > T){
			#pragma omp critical
			{
			p_tmpResult->pli_list[c] = i;
			c++;
			}
		}
	}
	}
	return p_tmpResult;
}
