//#include "gtest/gtest.h"
#include "Relay.h"

/*TEST(RelayTest, BasicFunctionality) {

	
	EXPECT_EQ(0, 0);
	EXPECT_EQ(1, 0);
}


GTEST_API_ int main(int argc, char **argv) {
	printf("Running main() from gtest_main.cc\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/

int main(int argc, char **argv) {
	new Relay(0);
	return 0;
}