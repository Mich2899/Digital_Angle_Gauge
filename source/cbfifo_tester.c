/*
 * **********************************************************************************************
 * assignment2_tester.c - automated test cases for grading the
 * student cbfifo implementations
 *
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * **********************************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <UART.h>

#include "cbfifo.h"

#define max(x,y) ((x) > (y) ? (x) : (y))

int g_tests_passed = 0;
int g_tests_total = 0;

#define test_assert(value) {                                            \
  g_tests_total++;                                                      \
  if (value) {                                                          \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d\n\r", __LINE__);               \
  }                                                                     \
}

#define test_equal(value1, value2) {                                    \
  g_tests_total++;                                                      \
  long res1 = (long)(value1);                                           \
  long res2 = (long)(value2);                                           \
  if (res1 == res2) {                                                   \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d: %ld != %ld\n\r", __LINE__, res1, res2); \
  }                                                                     \
}

static void
test_Rx_cbfifo_one_iteration()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf_rx[2048];
  const int cap_rx = Rx_cbfifo_capacity();
  size_t ret = 0;

  // the asserts in the following 2 lines are there because they're
  // not testing the student, they're validating that the test is
  // correct
  assert(sizeof(buf_rx) > cap_rx);
  test_assert(cap_rx == 1024  || cap_rx == 1023);

  test_equal(Rx_cbfifo_length(), 0);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), 0);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 2), 0);

  // enqueue 10 bytes, then dequeue same amt
  test_equal(Rx_cbfifo_enqueue(str, 10), 10);
  test_equal(Rx_cbfifo_length(), 10);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 10), 10);
  test_equal(strncmp(buf_rx, str, 10), 0);
  test_equal(Rx_cbfifo_length(), 0);

  // enqueue 20 bytes;  dequeue 5, then another 20
  test_equal(Rx_cbfifo_enqueue(str, 20), 20);
  test_equal(Rx_cbfifo_length(), 20);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 5), 5);
  test_equal(Rx_cbfifo_length(), 15);
  test_equal(Rx_cbfifo_dequeue(buf_rx+5, 20), 15);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str, 20), 0);

  // fill buffer and then read it back out
  test_equal(Rx_cbfifo_enqueue(str, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), cap_rx);
  ret = Rx_cbfifo_enqueue(str, 1);
  test_assert(ret == 0 || (int)ret == -1);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str, cap_rx), 0);

  // Add 20 bytes and pull out 18
  test_equal(Rx_cbfifo_enqueue(str, 20), 20);
  test_equal(Rx_cbfifo_length(), 20);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 18), 18);
  test_equal(Rx_cbfifo_length(), 2);
  test_equal(strncmp(buf_rx, str, 18), 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap_rx-2) / 4;
  for (int i=0; i<4; i++) {
    test_equal(Rx_cbfifo_enqueue(str+i*chunk_size, chunk_size), chunk_size);
    test_equal(Rx_cbfifo_length(), (i+1)*chunk_size + 2);
  }
  test_equal(Rx_cbfifo_length(), 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  test_equal(Rx_cbfifo_dequeue(buf_rx, 2), 2);
  test_equal(strncmp(buf_rx, str+18, 2), 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    test_equal(Rx_cbfifo_dequeue(buf_rx+i, 1), 1);
    test_equal(Rx_cbfifo_length(), chunk_size*4 - i - 1);
  }
  test_equal(strncmp(buf_rx, str, chunk_size*4), 0);

  // write more than cap_rxacity
  test_equal(Rx_cbfifo_enqueue(str, 65), 65);
  test_equal(Rx_cbfifo_enqueue(str+65, cap_rx), cap_rx-65);
  test_equal(Rx_cbfifo_length(), cap_rx);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str, cap_rx), 0);

  // write zero bytes
  test_equal(Rx_cbfifo_enqueue(str, 0), 0);
  test_equal(Rx_cbfifo_length(), 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < cap_rx-write  (1)
  //        bytes exactly cap_rx-write  (2)
  //        bytes > cap_rx-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  test_equal(Rx_cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(Rx_cbfifo_length(), 32);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 16), 16);
  test_equal(Rx_cbfifo_length(), 16);
  test_equal(strncmp(buf_rx, str, 16), 0);

  test_equal(Rx_cbfifo_enqueue(str+32, 32), 32);  // (1)
  test_equal(Rx_cbfifo_length(), 48);
  test_equal(Rx_cbfifo_enqueue(str+64, cap_rx-64), cap_rx-64);  // (2)
  test_equal(Rx_cbfifo_length(), cap_rx-16);
  test_equal(Rx_cbfifo_dequeue(buf_rx+16, cap_rx-16), cap_rx-16);
  test_equal(strncmp(buf_rx, str, cap_rx), 0);

  test_equal(Rx_cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(Rx_cbfifo_length(), 32);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 16), 16);
  test_equal(Rx_cbfifo_length(), 16);
  test_equal(strncmp(buf_rx, str, 16), 0);

  test_equal(Rx_cbfifo_enqueue(str+32, cap_rx-20), cap_rx-20);  // (3)
  test_equal(Rx_cbfifo_length(), cap_rx-4);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx-8), cap_rx-8);
  test_equal(strncmp(buf_rx, str+16, cap_rx-8), 0);
  test_equal(Rx_cbfifo_length(), 4);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 8), 4);
  test_equal(strncmp(buf_rx, str+16+cap_rx-8, 4), 0);
  test_equal(Rx_cbfifo_length(), 0);

  test_equal(Rx_cbfifo_enqueue(str, 49), 49);  // advance so that read < write
  test_equal(Rx_cbfifo_length(), 49);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 16), 16);
  test_equal(Rx_cbfifo_length(), 33);
  test_equal(strncmp(buf_rx, str, 16), 0);

  test_equal(Rx_cbfifo_enqueue(str+49, cap_rx-33), cap_rx-33);  // (4)
  test_equal(Rx_cbfifo_length(), cap_rx);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str+16, cap_rx), 0);

  test_equal(Rx_cbfifo_enqueue(str, 32), 32);  // advance so that read < write
  test_equal(Rx_cbfifo_length(), 32);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 16), 16);
  test_equal(Rx_cbfifo_length(), 16);
  test_equal(strncmp(buf_rx, str, 16), 0);

  test_equal(Rx_cbfifo_enqueue(str+32, cap_rx), cap_rx-16);  // (5)
  test_equal(Rx_cbfifo_dequeue(buf_rx, 1), 1);
  test_equal(Rx_cbfifo_length(), cap_rx-1);
  test_equal(Rx_cbfifo_dequeue(buf_rx+1, cap_rx-1), cap_rx-1);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str+16, cap_rx), 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  test_equal(Rx_cbfifo_enqueue(str, cap_rx-4), cap_rx-4);
  wpos += cap_rx-4;
  test_equal(Rx_cbfifo_length(), cap_rx-4);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 32), 32);
  rpos += 32;
  test_equal(Rx_cbfifo_length(), cap_rx-36);
  test_equal(strncmp(buf_rx, str, 32), 0);
  test_equal(Rx_cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(Rx_cbfifo_length(), cap_rx-24);

  test_equal(Rx_cbfifo_enqueue(str+wpos, 16), 16);  // (6)
  test_equal(Rx_cbfifo_length(), cap_rx-8);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx-8);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str+rpos, cap_rx-8), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(Rx_cbfifo_enqueue(str, cap_rx-4), cap_rx-4);
  wpos += cap_rx-4;
  test_equal(Rx_cbfifo_length(), cap_rx-4);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 32), 32);
  rpos += 32;
  test_equal(Rx_cbfifo_length(), cap_rx-36);
  test_equal(strncmp(buf_rx, str, 32), 0);
  test_equal(Rx_cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(Rx_cbfifo_length(), cap_rx-24);

  test_equal(Rx_cbfifo_enqueue(str+wpos, 24), 24);  // (7)
  test_equal(Rx_cbfifo_length(), cap_rx);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str+rpos, cap_rx), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(Rx_cbfifo_enqueue(str, cap_rx-4), cap_rx-4);
  wpos += cap_rx-4;
  test_equal(Rx_cbfifo_length(), cap_rx-4);
  test_equal(Rx_cbfifo_dequeue(buf_rx, 32), 32);
  rpos += 32;
  test_equal(Rx_cbfifo_length(), cap_rx-36);
  test_equal(strncmp(buf_rx, str, 32), 0);
  test_equal(Rx_cbfifo_enqueue(str+wpos, 12), 12);
  wpos += 12;
  test_equal(Rx_cbfifo_length(), cap_rx-24);

  test_equal(Rx_cbfifo_enqueue(str+wpos, 64), 24);  // (8)
  test_equal(Rx_cbfifo_length(), cap_rx);
  test_equal(Rx_cbfifo_dequeue(buf_rx, cap_rx), cap_rx);
  test_equal(Rx_cbfifo_length(), 0);
  test_equal(strncmp(buf_rx, str+rpos, cap_rx), 0);
}

void
test_Rx_cbfifo()
{
  g_tests_passed = 0;
  g_tests_total = 0;

  test_Rx_cbfifo_one_iteration();

  printf("%s: passed test case percentage (%2.1f%%)\n\r", __FUNCTION__,
      100.0*g_tests_passed/g_tests_total);
}

