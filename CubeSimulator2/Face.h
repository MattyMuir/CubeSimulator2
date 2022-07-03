#pragma once

enum class Axis { MIDDLE = 0, EQUATOR, STANDING };
enum class FaceEnum { UP = 0, DOWN, FRONT, BACK, RIGHT, LEFT };

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

	FaceEnum face = FaceEnum::UP;
};