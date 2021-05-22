#ifndef TESTSUITE
#define TESTSUITE

void testCond(bool ok, char *testname);
void testInt8(int8_t got, int8_t expected, char *testname);
void testInt32(int32_t got, int32_t expected, char *testname);
void testInt(int got, int expected, char *testname);
void testManyInt32(int32_t got[], int32_t expected[], int size, char *testname);


#endif