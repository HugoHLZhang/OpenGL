#pragma once 		 		 	 
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include < cstdio >
#include "Vertex.h"
template <int M, int N>
class Matrix {

    private:
        unsigned int m_Rows = M;
        unsigned int m_Cols = N;
        float* m_pTab;
    public:
	
        Matrix(const float& default_value = 0.0f) {
            m_pTab = new float[m_Rows * m_Cols];
            std::fill_n(m_pTab, m_Rows * m_Cols, default_value);
        }

        Matrix(const Matrix& b) {
            m_Rows = b.GetRows();
            m_Cols = b.GetCols();
            m_pTab = new float[m_Rows * m_Cols];
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < m_Cols; j++) {
                    m_pTab[i * m_Cols + j] = b.Get(i, j);
                }
            }
        }

        ~Matrix() {
            delete[] m_pTab;
        }

        inline float GetCols() const { return m_Cols; }
        inline float GetRows() const { return m_Rows; }
        inline float* GetData() { return m_pTab; }

        Matrix& operator= (Matrix b) {
            if (this != &b) {
                Swap(b);
            }
            return *this;
        }

        void Swap(Matrix& b) {
            std::swap(m_Rows, b.m_Rows);
            std::swap(m_Cols, b.m_Cols);
            std::swap(m_pTab, b.m_pTab);
        }

        void Set(int i, int j, const float val) {
            _ASSERT(i >= 0 && i < m_Rows&& j >= 0 && j < m_Cols);
            m_pTab[i * m_Cols + j] = val;
        }

        float Get(int i, int j) const {
            _ASSERT(i >= 0 && i < m_Rows&& j >= 0 && j < m_Cols);
            return m_pTab[i * m_Cols + j];
        };
	
        float& operator()(int i, int j) {
            _ASSERT(i >= 0 && i < m_Rows&& j >= 0 && j < m_Cols);
            return m_pTab[i * m_Cols + j];
        }

        const float& operator()(int i, int j) const {
            _ASSERT(i >= 0 && i < m_Rows&& j >= 0 && j < m_Cols);
            return m_pTab[i * m_Cols + j];
        }

        Matrix& operator+=(const Matrix& b) {
            _ASSERT(m_Rows == b.GetRows() && m_Cols == b.GetCols());
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < m_Cols; j++) {
                    Set(i, j, Get(i, j) + b.Get(i, j));
                }
            }
            return *this;
        }

        Matrix& operator-=(const Matrix& b) {
            _ASSERT(m_Rows == b.GetRows() && m_Cols == b.GetCols());
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < m_Cols; j++) {
                    Set(i, j, Get(i, j) - b.Get(i, j));
                }
            }
            return *this;
        }

	    template<int M2, int N2>
        Matrix& operator*=(const Matrix<M2, N2>& b) {
            _ASSERT(m_Cols == b.GetRows());
            Matrix<M, N2> c;
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < b.GetCols(); j++) {
                    float sum = 0.0f;
                    for (int k = 0; k < m_Cols; k++) {
                        sum += Get(i, k) * b.Get(k, j);
                    }
                    c.Set(i, j, sum);
                }
            }
            Swap(c);
            return *this;
        }

        Matrix& operator*=(const float& b) {
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < m_Cols; j++) {
                    Set(i, j, Get(i, j) * b);
                }
            }
            return *this;
        }

        Matrix operator+(Matrix b) const{
            return b += *this;
        }

        Matrix operator-(Matrix b) const {
            return b -= *this;
        }

        Matrix operator*(Matrix b) const {
            Matrix r = Matrix(*this);
		    return r *= b;
        }

        Matrix operator*(const float& b) const {
            Matrix r = Matrix(*this);
            return r *= b;
        }

	    template<int size = 4>
        static Matrix<size, size> Identity() {
            Matrix<size, size> result;
            for (int i = 0; i < size; i++) {
                result.Set(i, i, 1.0f);
            }
            return result;
        }

        static Matrix Translation(float x, float y, float z) {
            Matrix result = Identity();
            result.Set(0, 3, x);
            result.Set(1, 3, y);
            result.Set(2, 3, z);
            return result;
        }

	    static Matrix Translation(const Vector3& v) {
		    return Translation(v[0], v[1], v[2]);
	    }

        static Matrix Scale(float x, float y, float z) {
            Matrix result = Identity();
            result.Set(0, 0, x);
            result.Set(1, 1, y);
            result.Set(2, 2, z);
            return result;
        }
	
        static Matrix Scale(const Vector3& v)  {
		    return Scale(v[0], v[1], v[2]);
	    }

        static Matrix RotationX(float angle)  {
            Matrix result = Identity();
            result.Set(1, 1, cos(angle));
            result.Set(1, 2, -sin(angle));
            result.Set(2, 1, sin(angle));
            result.Set(2, 2, cos(angle));
            return result;
        }

        static Matrix RotationY(float angle)  {
            Matrix result = Identity();
            result.Set(0, 0, cos(angle));
            result.Set(0, 2, sin(angle));
            result.Set(2, 0, -sin(angle));
            result.Set(2, 2, cos(angle));
            return result;
        }

        static Matrix RotationZ(float angle) {
            Matrix result = Identity();
            result.Set(0, 0, cos(angle));
            result.Set(0, 1, -sin(angle));
            result.Set(1, 0, sin(angle));
            result.Set(1, 1, cos(angle));
            return result;
        }

        static Matrix Rotation(Vector3 angles) {
            return RotationY(angles[1]) * RotationZ(angles[2]) * RotationX(angles[0]);
        }


        static Matrix Perspective(float fov, float aspect, float near, float far) {
            Matrix result = Identity();
            float yScale = 1.0f / tanf(fov / 2.0f);
            float xScale = yScale / aspect;
            result.Set(0, 0, xScale);
            result.Set(1, 1, yScale);
            result.Set(2, 2, (far + near) / (near - far));
            result.Set(2, 3, (2 * near * far) / (near - far));
            result.Set(3, 2, -1.0f);
            result.Set(3, 3, 0.0f);
            return result;
        }

        static Matrix Ortho(float left, float right, float bottom, float top, float near, float far)  {
            Matrix result = Identity();
            result.Set(0, 0, 2.0f / (right - left));
            result.Set(0, 3, -(right + left) / (right - left));
            result.Set(1, 1, 2.0f / (top - bottom));
            result.Set(1, 3, -(top + bottom) / (top - bottom));
            result.Set(2, 2, -2.0f / (far - near));
            result.Set(2, 3, -(far + near) / (far - near));
            return result;
        }

	    static Matrix GetTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale)  {
		    Matrix result = Identity();
		    result = result * Translation(position);
		    result = result * Rotation(rotation * M_PI / 180.0f);
		    result = result * Scale(scale);
		    return result;
	    }

        Matrix Transpose() const{
		    Matrix result = Identity();
		    for (int i = 0; i < m_Rows; i++) {
			    for (int j = 0; j < m_Cols; j++) {
				    result.Set(j, i, Get(i, j));
			    }
		    }
		    return result;
        }

        void print() {
            for (int i = 0; i < m_Rows; i++) {
                for (int j = 0; j < m_Cols; j++) {
                    printf("%f ", m_pTab[i * m_Cols + j]);
                }
                printf("\n");
            }
            printf("\n");
        }

        void rawprint() {
            for (int i = 0; i < m_Rows * m_Cols; i++) {
                printf("%f ", m_pTab[i]);
            }
            printf("\n");
        }

};


typedef Matrix<4, 4> Matrix4x4;
typedef Matrix<3, 3> Matrix3x3;