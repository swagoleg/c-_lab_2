#include <gtest/gtest.h>
#include "lib/number.h"


TEST(Equality, SameSmallNumbers) {
    int2025_t a = FromInt(42);
    int2025_t b = FromInt(42);
    EXPECT_TRUE(a == b);
}

TEST(Equality, DifferentSmallNumbers) {
    int2025_t a = FromInt(42);
    int2025_t c = FromInt(43);
    EXPECT_FALSE(a == c);
}

TEST(Equality, SameLargeNumbers) {
    int2025_t d = FromString("12345678901234567890");
    int2025_t e = FromString("12345678901234567890");
    EXPECT_TRUE(d == e);
}

TEST(Equality, DifferentLargeNumbers) {
    int2025_t d = FromString("12345678901234567890");
    int2025_t f = FromString("12345678901234567891");
    EXPECT_FALSE(d == f);
}


TEST(Inequality, SameSmallNumbers) {
    int2025_t a = FromInt(42);
    int2025_t b = FromInt(42);
    EXPECT_FALSE(a != b);
}

TEST(Inequality, DifferentSmallNumbers) {
    int2025_t a = FromInt(42);
    int2025_t c = FromInt(43);
    EXPECT_TRUE(a != c);
}

TEST(Inequality, SameLargeNumbers) {
    int2025_t d = FromString("12345678901234567890");
    int2025_t e = FromString("12345678901234567890");
    EXPECT_FALSE(d != e);
}

TEST(Inequality, DifferentLargeNumbers) {
    int2025_t d = FromString("12345678901234567890");
    int2025_t f = FromString("12345678901234567891");
    EXPECT_TRUE(d != f);
}


TEST(ZeroComparison, IntAndStringZero) {
    int2025_t zero1 = FromInt(0);
    int2025_t zero2 = FromString("0");
    EXPECT_TRUE(zero1 == zero2);
}

TEST(ZeroComparison, IntAndNegativeStringZero) {
    int2025_t zero1 = FromInt(0);
    int2025_t zero3 = FromString("-0");
    EXPECT_TRUE(zero1 == zero3);
}

TEST(ZeroComparison, StringAndNegativeStringZero) {
    int2025_t zero2 = FromString("0");
    int2025_t zero3 = FromString("-0");
    EXPECT_TRUE(zero2 == zero3);
}


TEST(SignComparison, PositiveVsNegative) {
    int2025_t positive = FromInt(42);
    int2025_t negative = FromInt(-42);
    EXPECT_TRUE(positive != negative);
}

TEST(SignComparison, PositiveVsZero) {
    int2025_t positive = FromInt(42);
    int2025_t zero = FromInt(0);
    EXPECT_TRUE(positive != zero);
}

TEST(SignComparison, NegativeVsZero) {
    int2025_t negative = FromInt(-42);
    int2025_t zero = FromInt(0);
    EXPECT_TRUE(negative != zero);
}


TEST(LargeNumberComparison, SameVeryLargeNumbers) {
    int2025_t large1 = FromString("999999999999999999999999999999");
    int2025_t large2 = FromString("999999999999999999999999999999");
    EXPECT_TRUE(large1 == large2);
}

TEST(LargeNumberComparison, DifferentVeryLargeNumbers) {
    int2025_t large1 = FromString("999999999999999999999999999999");
    int2025_t large3 = FromString("999999999999999999999999999998");
    EXPECT_TRUE(large1 != large3);
}

TEST(LargeNumberComparison, DifferentVeryLargeNumbersNotEqual) {
    int2025_t large1 = FromString("999999999999999999999999999999");
    int2025_t large3 = FromString("999999999999999999999999999998");
    EXPECT_FALSE(large1 == large3);
}