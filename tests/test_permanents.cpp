// MIT License
//
// Copyright (c) 2022 Quandela
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <complex>
#include <catch2/catch.hpp>
#include "../src/permanent.h"
#include <iostream>

static std::vector<std::complex<double>> genSquaredMatrixComplex(int squaredMatrixSize)
{
    std::vector<std::complex<double>> mat(squaredMatrixSize*squaredMatrixSize);
    auto increment = 1.0 / (squaredMatrixSize * squaredMatrixSize);

    auto generateComplexSuite = [&increment] {
        static std::complex<double> base(0, 0);
        base += std::complex<double>(increment, increment/2);
        return base;
    };

    std::generate(mat.begin(), mat.end(), generateComplexSuite);
    return mat;
}

static std::vector<double> genSquaredMatrixDouble(int squaredMatrixSize)
{
    auto increment = 1.0 / (squaredMatrixSize * squaredMatrixSize);
    auto generateSuite = [&increment]() {
        static double base = 0;
        base += increment;
        return base;
    };

    std::vector<double> mat(squaredMatrixSize*squaredMatrixSize);
    std::generate(mat.begin(), mat.end(), generateSuite);
    return mat;
}

static bool isApproximatelyEqual(double a, double b, double tolerance = std::numeric_limits<double>::epsilon() * 5)
{
    double diff = std::abs(a - b);
    if (diff <= tolerance)
        return true;

    return false;
}

static bool isApproximatelyEqual(std::complex<double> a, std::complex<double> b, double tolerance = std::numeric_limits<double>::epsilon() * 5)
{
    double diff = std::abs(a.real() - b.real());
    if (diff > tolerance)
        return false;

    diff = std::abs(a.imag() - b.imag());
    if (diff > tolerance)
        return false;

    return true;
}

SCENARIO("C++ Testing Permanents") {
    GIVEN("the glynn algorythm") {
        WHEN("computing a double matrix of size 2") {
            std::vector<double> matrixSize2 = {1, 2, 3, 4};

            THEN("build Succeed !") {
                auto res = permanent_glynn(matrixSize2.data(), 2);
                REQUIRE(isApproximatelyEqual(res, 10.));
            }
        }
        WHEN("computing a double matrix of size 5") {
            std::vector<double> matrix = genSquaredMatrixDouble(5);
            THEN("build Succeed !") {
                auto res = permanent_glynn(matrix.data(), 5);
                REQUIRE(isApproximatelyEqual(res, 1.4828236800000005));
            }
        }
        WHEN("computing a complex<double> matrix of size 5") {
            std::vector<std::complex<double>> matrix = genSquaredMatrixComplex(5);
            THEN("build Succeed !") {
                auto res = permanent_glynn(matrix.data(), 5);
                REQUIRE(isApproximatelyEqual(res, std::complex<double>(-1.760853120000001, 1.8998678400000022)));
            }
        }
    }
}
