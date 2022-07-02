#pragma once

enum class ColorEnum { WHITE = 0, YELLOW, GREEN, BLUE, RED, ORANGE };

class Color
{
public:
	Color() {}
	Color(ColorEnum col_)
		: col(col_) {}

	ColorEnum col = ColorEnum::WHITE;
};