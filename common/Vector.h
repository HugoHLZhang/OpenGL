#pragma once
#include <utility>
#include <cmath>
#include < cstdio >
template <int N>
class Vector {
	private:
		float data[N];

	public:
		Vector() {
			for (int i = 0; i < N; i++) {
				data[i] = 0;
			}
		}
		
		Vector(const float v[N]) {
			for (int i = 0; i < N; i++) {
				data[i] = v[i];
			}
		}

		template <typename... Ts, typename = typename std::enable_if<sizeof...(Ts) == N>::type>
		Vector(Ts && ... Vector) : data{ (float)std::forward<Ts>(Vector)... }
		{ }

		
		float dot(const Vector& b) const {
			float result = 0;
			for (int i = 0; i < N; i++) {
				result += data[i] * b.data[i];
			}
			return result;
		}
		
		Vector& operator+= (const Vector& a) {
			for (int i = 0; i < N; i++) {
				data[i] += a.data[i];
			}
			return *this;
		}
		
		Vector& operator-= (const Vector& a) {
			for (int i = 0; i < N; i++) {
				data[i] -= a.data[i];
			}
			return *this;
		}
		
		Vector& operator*= (const float& a) {
			for (int i = 0; i < N; i++) {
				data[i] *= a;
			}
			return *this;
		}

		Vector& operator/= (const float& a) {
			for (int i = 0; i < N; i++) {
				data[i] /= a;
			}
			return *this;
		}

		Vector operator+ (Vector a) const{
			Vector result = *this;
			result += a;
			return result;
		}

		Vector operator- (const Vector& a) const{
			Vector result = *this;
			result -= a;
			return result;
		}
		
		Vector operator-() const{
			Vector result = *this;
			for (int i = 0; i < N; i++) {
				result.data[i] = -result.data[i];
			}
			return result;
		}

		Vector operator* (float a) const {
			Vector result = *this;
			result *= a;
			return result;
		}
		
		Vector operator/ (float a) const {
			Vector result = *this;
			result /= a;
			return result;
		}

		float& operator[] (int i) {
			return data[i];
		}

		const float& operator[] (int i) const {
			return data[i];
		}

		float length() const{
			float result = 0;
			for (int i = 0; i < N; i++) {
				result += data[i] * data[i];
			}
			return sqrt(result);
		}

		Vector normalize() {
			float length = this->length();
			if(length)
			for (int i = 0; i < N; i++) 
				data[i] /= length;		
			return *this;
		}

		float distance(Vector a) const{
			Vector result = *this - a;
			return result.length();
		}
		
		Vector<3> cross(const Vector<3>& a) const{
			Vector result;
			result.data[0] = data[1] * a.data[2] - data[2] * a.data[1];
			result.data[1] = data[2] * a.data[0] - data[0] * a.data[2];
			result.data[2] = data[0] * a.data[1] - data[1] * a.data[0];
			return result;
		}
		
		void print() const{
			for (int i = 0; i < N; i++) {
				printf("%f ", data[i]);
			}
			printf("\n");
		}
};

typedef Vector<3> Vector3;
typedef Vector<4> Vector4;

