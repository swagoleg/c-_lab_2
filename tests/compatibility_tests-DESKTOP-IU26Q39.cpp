#include <gtest/gtest.h>
#include "lib/number.h"


TEST(ConversionCompatibility, IntAndString42) {
    int2025_t from_int_42 = FromInt(42);
    int2025_t from_string_42 = FromString("42");
    EXPECT_TRUE(from_int_42 == from_string_42);
}

TEST(ConversionCompatibility, IntAndStringNegative100) {
    int2025_t from_int_neg = FromInt(-100);
    int2025_t from_string_neg = FromString("-100");
    EXPECT_TRUE(from_int_neg == from_string_neg);
}

TEST(ConversionCompatibility, IntAndStringZero) {
    int2025_t from_int_zero = FromInt(0);
    int2025_t from_string_zero = FromString("0");
    EXPECT_TRUE(from_int_zero == from_string_zero);
}

TEST(ConversionCompatibility, MixedArithmeticAddition) {
    int2025_t a = FromInt(10);
    int2025_t b = FromString("20");
    int2025_t expected = FromInt(30);
    int2025_t result = a + b;
    EXPECT_TRUE(result == expected);
}

TEST(ConversionCompatibility, MixedArithmeticSubtraction) {
    int2025_t c = FromString("100");
    int2025_t d = FromInt(50);
    int2025_t expected2 = FromInt(50);
    int2025_t result2 = c - d;
    EXPECT_TRUE(result2 == expected2);
}

TEST(ConversionCompatibility, LimitValueINT32_MAX) {
    int2025_t max_from_int = FromInt(2147483647);
    int2025_t max_from_string = FromString("2147483647");
    EXPECT_TRUE(max_from_int == max_from_string);
}

TEST(ConversionCompatibility, LimitValueINT32_MIN) {
    int2025_t min_from_int = FromInt(-2147483648);
    int2025_t min_from_string = FromString("-2147483648");
    EXPECT_TRUE(min_from_int == min_from_string);
}

TEST(ConversionCompatibility, MixedComparisonEqual) {
    int2025_t int_val = FromInt(123);
    int2025_t str_val = FromString("123");
    EXPECT_TRUE(int_val == str_val);
}

TEST(ConversionCompatibility, MixedComparisonNotEqual) {
    int2025_t int_val = FromInt(123);
    int2025_t different_str = FromString("124");
    EXPECT_FALSE(int_val == different_str);
    EXPECT_TRUE(int_val != different_str);
}

TEST(ConversionCompatibility, MixedComparisonNotEqualReverse) {
    int2025_t int_val = FromInt(123);
    int2025_t str_val = FromString("123");
    EXPECT_FALSE(int_val != str_val);
}

TEST(ConversionCompatibility, CrossTypeArithmeticChain) {
    int2025_t a = FromInt(10);
    int2025_t b = FromString("5");
    int2025_t c = FromInt(2);
    int2025_t d = FromString("3");

    // (10 + 5) * 2 - 3 = 15 * 2 - 3 = 30 - 3 = 27
    int2025_t result = ((a + b) * c) - d;
    int2025_t expected = FromInt(27);
    EXPECT_TRUE(result == expected);
}

TEST(ConversionCompatibility, StringFormatNormal) {
    int2025_t normal = FromString("42");
    int2025_t from_int_val = FromInt(42);
    EXPECT_TRUE(normal == from_int_val);
}

TEST(ConversionCompatibility, StringFormatWithPlus) {
    int2025_t with_plus = FromString("+42");
    int2025_t from_int_val = FromInt(42);
    EXPECT_TRUE(with_plus == from_int_val);
}

TEST(ConversionCompatibility, StringFormatWithLeadingZeros) {
    int2025_t with_zeros = FromString("000042");
    int2025_t from_int_val = FromInt(42);
    EXPECT_TRUE(with_zeros == from_int_val);
}

TEST(ConversionCompatibility, StringFormatCrossComparison) {
    int2025_t normal = FromString("42");
    int2025_t with_plus = FromString("+42");
    int2025_t with_zeros = FromString("000042");

    EXPECT_TRUE(normal == with_plus);
    EXPECT_TRUE(normal == with_zeros);
}

TEST(ConversionCompatibility, ZeroIntToString) {
    int2025_t zero_int = FromInt(0);
    int2025_t zero_string = FromString("0");
    EXPECT_TRUE(zero_int == zero_string);
}

TEST(ConversionCompatibility, ZeroNegativeString) {
    int2025_t zero_int = FromInt(0);
    int2025_t zero_negative_string = FromString("-0");
    EXPECT_TRUE(zero_int == zero_negative_string);
}

TEST(ConversionCompatibility, ZeroPlusString) {
    int2025_t zero_int = FromInt(0);
    int2025_t zero_with_plus = FromString("+0");
    EXPECT_TRUE(zero_int == zero_with_plus);
}

TEST(ConversionCompatibility, ZeroStringVariants) {
    int2025_t zero_string = FromString("0");
    int2025_t zero_negative_string = FromString("-0");
    int2025_t zero_with_plus = FromString("+0");

    EXPECT_TRUE(zero_string == zero_negative_string);
    EXPECT_TRUE(zero_string == zero_with_plus);
}