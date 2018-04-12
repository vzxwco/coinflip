# coinflip

This program computes the expected waiting time for all bitstrings of
the specified length.

You can adjust SEQ_LEN to accomodate for different lenghts. The theoretical
upper bound is determined by your machine's word size. Unless you have 2^64
words of RAM to spare, the actual upper bound may be much lower.

You can increase NUM_ROUNDS to improve precision.

Here's the output for SEQ_LEN = 3 and NUM_ROUNDS = 10000000:

      seq   average waiting time
      --------------------------
      000   14.00
      001   8.00
      010   10.00
      011   8.00
      100   8.00
      101   10.00
      110   8.00
      111   14.00

This means that e.g. when tossing a fair coin, head-tail-tail (011) is most
likely to occur after 8 tosses, while head-tail-head (010) is most likely to
occur after 10 tosses.

See http://goo.gl/dL7SZg for a theoretical discussion of these results.

