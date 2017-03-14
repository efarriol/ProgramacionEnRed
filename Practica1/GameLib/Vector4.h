#pragma once
template <typename T>

struct Vector4 {
	T x, y, x2, y2;
	Vector4();
	Vector4(T _x, T _y, T _x2, T _y2) : x{ _x }, y{ _y }, x2{ _x2 }, y2{ _y2 } {}
	void SetCoordinates(T _x, T _y, T _x2, T _y2) {
		x = _x;
		y = _y;
		x2 = _x2;
		y2 = _y2;
	}
};

typedef Vector4<int>          Vector4i;
typedef Vector4<unsigned int> Vector4u;
typedef Vector4<float>        Vector4f;