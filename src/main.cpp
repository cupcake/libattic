
#include <gtest/gtest.h>
#include "chunker.h"


// Chunk a file
TEST(ChunkTest,ChunkFile) {
    Chunker c;
    std::string test;
    test.append("test.txt");
    ASSERT_EQ(c.ChunkFile(test),true);

}

// TODO :: De-Chunk a file

int main (int argc, char* argv[])
{
    // Init gtestframework
    testing::InitGoogleTest(&argc, argv);

    // run all tests
    return RUN_ALL_TESTS();
}
