#include <gtest/gtest.h>
#include "RBT.cpp"

using namespace std;

TEST(InsertionTests, Test1) {
    RBTree<int> intBT;
    intBT.insert(6);
    string excepted_output = "{value:6 , color=0}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}

TEST(InsertionTests, Test2) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    string excepted_output = "{value:6 , color=0}, |6|->{value:3 , color=0}, |3|->{value:2 , color=1}, |3|->{value:5 , color=1}, |6|->{value:8 , color=1}, |8|->{value:7 , color=0}, |8|->{value:79 , color=0}, |79|->{value:8 , color=1}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}
TEST(InsertionTests, Test3) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8, 9};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    string excepted_output = "{value:6 , color=0}, |6|->{value:3 , color=0}, |3|->{value:2 , color=1}, |3|->{value:5 , color=1}, |6|->{value:8 , color=1}, |8|->{value:7 , color=0}, |8|->{value:9 , color=0}, |9|->{value:8 , color=1}, |9|->{value:79 , color=1}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}

TEST(RemovalTests, Test1) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.remove(7);
    string excepted_output = "{value:6 , color=0}, |6|->{value:3 , color=0}, |3|->{value:2 , color=1}, |3|->{value:5 , color=1}, |6|->{value:8 , color=1}, |8|->{value:8 , color=0}, |8|->{value:79 , color=0}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}

TEST(RemovalTests, Test2) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.remove(5);
    string excepted_output = "{value:6 , color=0}, |6|->{value:3 , color=0}, |3|->{value:2 , color=1}, |6|->{value:8 , color=1}, |8|->{value:7 , color=0}, |8|->{value:79 , color=0}, |79|->{value:8 , color=1}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}

TEST(RemovalTests, Test3) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.remove(8);
    string excepted_output = "{value:6 , color=0}, |6|->{value:3 , color=0}, |3|->{value:2 , color=1}, |3|->{value:5 , color=1}, |6|->{value:8 , color=1}, |8|->{value:7 , color=0}, |8|->{value:79 , color=0}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}

TEST(RemovalTests, Test4) {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.remove(6);
    string excepted_output = "{value:7 , color=0}, |7|->{value:3 , color=0}, |3|->{value:2 , color=1}, |3|->{value:5 , color=1}, |7|->{value:8 , color=1}, |8|->{value:79 , color=0}, |79|->{value:8 , color=1}, ";
    ASSERT_EQ(intBT.vectorize(), excepted_output);
}



int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}
//
// Created by zer-oRBT on 25.03.2023.
//
