#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

static const double PI = 3.1415;
static const double TWO_PI = 2 * PI;

static const double DEG_TO_RAD = 180.0 / PI;
static const double RAD_TO_DEG = PI / 180.0;

template<typename T> inline T magnitude(sf::Vector2<T> vec) {
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

template<typename T> inline T normalize(sf::Vector2<T> vec) {
	T mag = magnitude(vec);
	return Vector2<T>(vec.x / mag, vec.y / mag);
}