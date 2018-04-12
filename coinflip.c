/*  Copyright (c) 2018, Philip Busch <vzxwco@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */


/*  This program computes the expected waiting time for all bitstrings of 
 *  the specified length.
 *
 *  You can adjust SEQ_LEN to accomodate for different lenghts. The theoretical
 *  upper bound is determined by your machine's word size. Unless you have 2^64
 *  words of RAM to spare, the actual upper bound may be much lower.
 * 
 *  You can increase NUM_ROUNDS to improve precision. 
 *
 *  Here's the output for SEQ_LEN = 3 and NUM_ROUNDS = 10000000:
 * 
 *       seq   average waiting time
 *       --------------------------
 *       000   14.00
 *       001   8.00
 *       010   10.00
 *       011   8.00
 *       100   8.00
 *       101   10.00
 *       110   8.00
 *       111   14.00
 *
 *  See goo.gl/dL7SZg for a theoretical discussion of these results.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>


#define NUM_ROUNDS 1000
#define SEQ_LEN 3
#define ARR_LEN (1<<SEQ_LEN)


/* Returns 0 or 1 with "equal" probability. */
int flip(void)
{
	return rand() % 2;
}

/* Checks if array contains a zero. */
int haszero(int *arr, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		if (arr[i] == 0)
			return 1;

	return 0;
}

/* Computes average waiting time for all bitstrings of length SEQ_LEN. */
int main(void)
{
	unsigned char seq;
	int i, j, mask;
	int hit[ARR_LEN];
	int sum[ARR_LEN];

	// initialize random number generator
	srand(time(NULL));

	// initialize seq to all zeroes
	seq = '\0';

	// initialize sum to all zeroes
	memset(sum, 0, sizeof sum);

	// initialize bitmask (set SEQ_LEN least significant bits to 1)
	mask = 0;
	for (i = 0; i < SEQ_LEN; i++)
		mask |= 1 << i;

	// main loop
	for (i = 0; i < NUM_ROUNDS; i++) {
		// initialize hit to all zeroes
		memset(hit, 0, sizeof hit);

		// keep flipping until all sequences of len SEQ_LEN are found
		for (j = 0; haszero(hit, ARR_LEN); j++) {
			// shift sequence to the left by 1
			seq <<= 1;
	
			// flip last bit
			seq |= flip();
	
			// isolate last SEQ_LEN bits
			seq &= mask;

			// save waiting time for current sequence
			if (j >= SEQ_LEN)	
				hit[seq] = (hit[seq] == 0 ? j : hit[seq]);
		}

		// add results of current round to overall sum
		for (j = 0; j < ARR_LEN; j++) {
			sum[j] += hit[j];
		}
	}

	// print results
	puts("seq   average waiting time");
	puts("--------------------------");
	for (i = 0; i < ARR_LEN; i++) {
		// output binary representation of bitstring
		for (j = 1; j <= SEQ_LEN; j++)
			putchar((i & (1 << (SEQ_LEN - j))) ? '1' : '0');

		// output waiting time
		printf("   %.2f\n", (double)sum[i] / NUM_ROUNDS);
	}

	// YOUR KUNG-FU IS OLD AND NOW YOU MUST DIE!!
	return 0;
}

