#pragma once
#include <compare>

enum class ColorEnum { WHITE = 0, YELLOW, GREEN, BLUE, RED, ORANGE };

class Color
{
public:
	Color() {}
	Color(ColorEnum col_)
		: col(col_) {}

	operator ColorEnum() const { return col; }
	operator int() const { return (int)col; }

	auto operator<=> (const Color& other) { return (int)col <=> (int)other; }

	ColorEnum col = ColorEnum::WHITE;
};