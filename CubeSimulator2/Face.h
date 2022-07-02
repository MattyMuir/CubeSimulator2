#pragma once

enum class Axis { MIDDLE = 0, EQUATOR, STANDING };
enum class FaceEnum { UP = 0, DOWN, FRONT, BACK, RIGHT, LEFT };

constexpr FaceEnum middle[4] = { FaceEnum::FRONT, FaceEnum::DOWN, FaceEnum::BACK, FaceEnum::UP };
constexpr FaceEnum equator[4] = { FaceEnum::FRONT, FaceEnum::RIGHT, FaceEnum::BACK, FaceEnum::LEFT };
constexpr FaceEnum standing[4] = { FaceEnum::UP, FaceEnum::RIGHT, FaceEnum::DOWN, FaceEnum::LEFT };

constexpr int middleStart[6] = { 3, 1, 0, 2, -1, -1 };
constexpr int equatorStart[6] = { -1, -1, 0, 2, 1, 3 };
constexpr int standingStart[6] = { 0, 2, -1, -1, 1, 3 };

static int PosMod(int a, int b)
{
	int m = a % b;
	if (m < 0) m += b;
	return m;
}

class Face
{
public:
	Face() {}
	Face(FaceEnum face_)
		: face(face_) {}

	void Opposite()
	{
		int i = (int)face;
		i = (i % 2) ? (i - 1) : (i + 1);
		face = (FaceEnum)i;
	}

	void Rotate(Axis axis, int num)
	{
		int i = (int)face;
		switch (axis)
		{
		case Axis::MIDDLE:
			if (i > 3) return;
			if (num == 2) { Opposite(); return; }
			face = middle[PosMod(middleStart[i] + num, 4)];
			break;
		case Axis::EQUATOR:
			if (i < 2) return;
			if (num == 2) { Opposite(); return; }
			face = equator[PosMod(equatorStart[i] + num, 4)];
			break;
		case Axis::STANDING:
			if (i == 2 || i == 3) return;
			if (num == 2) { Opposite(); return; }
			face = standing[PosMod(standingStart[i] + num, 4)];
			break;
		}
	}

	FaceEnum face = FaceEnum::UP;
};