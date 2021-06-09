#ifndef TESTSUITE
#define TESTSUITE

void testCond(bool ok, char *testname);
void testInt8(uint8_t got, uint8_t expected, char *testname);
void testInt32(uint32_t got, uint32_t expected, char *testname);
void testInt(int got, int expected, char *testname);
void testManyInt32(uint32_t got[], uint32_t expected[], int size, char *testname);


#endif