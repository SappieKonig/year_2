#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <span>
#include <tuple>
#include <vector>
#include <iostream>

// ==================================
// ========    Exercise 1    ========
// ==================================
std::pair<float, float> statistics(std::span<const float> values)
{
    // Your solution goes here
    float mean = 0.f;
    float variance = 0.f;
    for (auto& value : values) {
            mean += value;
    }
    mean /= (float) values.size();
    for (auto& value : values) {
            variance += (value - mean) * (value - mean);
    }
    variance /= (float) values.size();
    float std = std::sqrt(variance);
    return { mean, std };
}

// ==================================
// ========    Exercise 2    ========
// ==================================
struct Tree {
    float value;
    std::vector<Tree> children;
};

float _countTree(const Tree& tree, int level = 0, bool countOnlyEvenLevels = false)
{
    float sum = tree.value;
    if (countOnlyEvenLevels && level % 2 == 1) {
        sum -= tree.value;
    }
    for (auto& child : tree.children) {
            sum += _countTree(child, level + 1, countOnlyEvenLevels);
    }
    return sum;
}


// Your implementation goes here. Feel free to define helper functions/structs/classes in this
// file if you need to. Make sure to put them above this function to prevent linker warnings.
float countTree(const Tree& tree, bool countOnlyEvenLevels)
{
    return _countTree(tree, 0, countOnlyEvenLevels);
}

// =================================================
// ========    Vector & Matrix Exercises    ========
// =================================================
// 
// !!! DO NOT MODIFY THIS STRUCT !!!
struct Vector3 {
    float x, y, z;
};

// Basic operations of a 3D vector (addition, subtraction, multiplication and division)
Vector3 add(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
Vector3 sub(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
Vector3 mul(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}
Vector3 mul(const Vector3& lhs, float rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}
Vector3 div(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
}

// Dot product, cross product and vector length.
// If you need a Linear Algebra refresher, please check out:
// https://www.tudelft.nl/en/eemcs/study/online-education/math-explained/vectors/
float dot(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3 {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
}
float length(const Vector3& lhs)
{
        return std::sqrt(lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z);
}

// The matrix is defined as:
// | m00  m01  m02 |
// | m10  m11  m12 |
// | m20  m21  m22 |
//
// The columns are stored as Vector3's, where:
// col1 = (m00, m10, m20)
// col2 = (m01, m11, m21)
// col3 = (m02, m12, m22)
//
// !!! DO NOT MODIFY THIS STRUCT !!!
struct Matrix3 {
    Vector3 col1, col2, col3;
};

// Element-wise operations of a 3x3 matrix (addition and subtraction)
Matrix3 add(const Matrix3& lhs, const Matrix3& rhs)
{
    // Your solution goes here
    return Matrix3 {add(lhs.col1, rhs.col1), add(lhs.col2, rhs.col2), add(lhs.col3, rhs.col3)};
}
Matrix3 sub(const Matrix3& lhs, const Matrix3& rhs)
{
    return Matrix3 {sub(lhs.col1, rhs.col1), sub(lhs.col2, rhs.col2), sub(lhs.col3, rhs.col3)};
}

// Matrix multiplication with a scalar, a vector, and another matrix.
Matrix3 mul(const Matrix3& lhs, float rhs)
{
    return Matrix3{mul(lhs.col1, rhs), mul(lhs.col2, rhs), mul(lhs.col3, rhs)};
}
Vector3 mul(const Matrix3& lhs, const Vector3& rhs)
{
    auto row1 = Vector3 {lhs.col1.x, lhs.col2.x, lhs.col3.x};
    auto row2 = Vector3 {lhs.col1.y, lhs.col2.y, lhs.col3.y};
    auto row3 = Vector3 {lhs.col1.z, lhs.col2.z, lhs.col3.z};
    return Vector3 {dot(row1, rhs), dot(row2, rhs), dot(row3, rhs)};
}
Matrix3 mul(const Matrix3& lhs, const Matrix3& rhs)
{
    return Matrix3{mul(lhs, rhs.col1), mul(lhs, rhs.col2), mul(lhs, rhs.col3)};
}

// Taking the transpose of a matrix means changing it's columns into rows and vice versa.
// Following our previous definition, the transpose of our matrix should look like:
// | m00  m10  m20 |
// | m01  m11  m21 |
// | m02  m12  m22 |
Matrix3 transpose(const Matrix3& m)
{
    auto row1 = Vector3 {m.col1.x, m.col2.x, m.col3.x};
    auto row2 = Vector3 {m.col1.y, m.col2.y, m.col3.y};
    auto row3 = Vector3 {m.col1.z, m.col2.z, m.col3.z};
    return Matrix3 {row1, row2, row3};
}

// The determinant is needed to compute the inverse of a matrix.
// If you need a Linear Algebra refresher, please check out:
// https://www.tudelft.nl/en/eemcs/study/online-education/math-explained/linear-algebra/#c144161
float determinant(const Matrix3& m)
{
    float determinant = 0.f;
    determinant += m.col1.x * (m.col2.y * m.col3.z - m.col2.z * m.col3.y);
    determinant -= m.col1.y * (m.col2.x * m.col3.z - m.col2.z * m.col3.x);
    determinant += m.col1.z * (m.col2.x * m.col3.y - m.col2.y * m.col3.x);
    return determinant;
}

float index(const Matrix3& m, int row, int col)
{
    Vector3 _col;
    if (col == 0) {
        _col = m.col1;
    }
    else if (col == 1) {
        _col = m.col2;
    }
    else {
        _col = m.col3;
    }
    if (row == 0) {
            return _col.x;
    }
    else if (row == 1) {
            return _col.y;
    }
    else {
            return _col.z;
    }
}

Matrix3 matrix_of_cofactors(const Matrix3& m)
{
    std::vector<float> minors;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::vector<float> minor;
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    if (k != i && l != j) {
                        // This is actually transposed, but throw your hands in the air like a determinant that just doesn't care ;)
                        minor.push_back(index(m, k, l));
                    }
                }
            }
            minors.push_back(minor[0] * minor[3] - minor[1] * minor[2]);
        }
    }
    return Matrix3 {
        Vector3 {minors[0], -minors[3], minors[6]},
        Vector3 {-minors[1], minors[4], -minors[7]},
        Vector3 {minors[2], -minors[5], minors[8]}
    };
}

// Computing the inverse of the given matrix. If you implemented it correctly then matrix M multiplied
// by its inverse should give the identity matrix). More information on how to compute the inverse of a
// 3x3 matrix can be found here:
// https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
Matrix3 inverse(const Matrix3& matrix)
{
    float det = determinant(matrix);
    Matrix3 cofactors = matrix_of_cofactors(matrix);
    Matrix3 adjugate = transpose(cofactors);
    return mul(adjugate, 1.f / det);

}

void print_matrix(const Matrix3& m)
{
    std::cout << m.col1.x << " " << m.col2.x << " " << m.col3.x << std::endl;
    std::cout << m.col1.y << " " << m.col2.y << " " << m.col3.y << std::endl;
    std::cout << m.col1.z << " " << m.col2.z << " " << m.col3.z << std::endl << std::endl;
}
