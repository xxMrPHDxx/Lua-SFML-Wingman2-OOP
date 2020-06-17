#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

static const double PI = 3.1415f;
static const double TWO_PI = 2.f * PI;

static const double DEG_TO_RAD = PI / 180.f;
static const double RAD_TO_DEG = 180.f / PI;

template<typename T> inline T magnitude(sf::Vector2<T> vec) {
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

template<typename T> inline sf::Vector2<T> normalize(sf::Vector2<T> vec) {
	T mag = magnitude(vec);
	return sf::Vector2<T>(vec.x / mag, vec.y / mag);
}

template<typename T> inline T dot(sf::Vector2<T> v1, sf::Vector2<T> v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

template<typename T> inline double angle_between(sf::Vector2<T> v1, sf::Vector2<T> v2) {
	return std::acos(dot(v1, v2) / (magnitude(v1) * magnitude(v2))) * RAD_TO_DEG;
}

template<typename T> inline sf::Vector2<T> angle_relative_to(sf::Vector2<T>& in, float degree) {
	float radian = degree * DEG_TO_RAD;
	float sin = std::sin(radian);
	float cos = std::cos(radian);
	return sf::Vector2<T>(in.x * cos - in.y * sin, in.x * sin + in.y * cos);
}