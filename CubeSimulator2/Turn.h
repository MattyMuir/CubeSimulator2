#pragma once
#include <string>

#include "Face.h"

class Turn
{
public:
	Turn() {}
	Turn(FaceEnum face_, int num_ = 1, int layer_ = 0)
		: face(face_), num(num_), layer(layer_) {}
	Turn(Face face_, int num_ = 1, int layer_ = 0)
		: face(face_.face), num(num_), layer(layer_) {}
	Turn(const std::string& turnStr)
	{
		switch (turnStr[0])
		{
		case 'U':
			face = FaceEnum::UP;
			break;
		case 'D':
			face = FaceEnum::DOWN;
			break;
		case 'F':
			face = FaceEnum::FRONT;
			break;
		case 'B':
			face = FaceEnum::BACK;
			break;
		case 'R':
			face = FaceEnum::RIGHT;
			break;
		case 'L':
			face = FaceEnum::LEFT;
			break;
		case 'M':
			face = FaceEnum::LEFT;
			layer = 1;
			break;
		case 'E':
			face = FaceEnum::DOWN;
			layer = 1;
			break;
		case 'S':
			face = FaceEnum::FRONT;
			layer = 1;
			break;
		}

		if (turnStr.length() > 1)
		{
			switch (turnStr[1])
			{
			case '\'':
				num = -1;
				break;
			case '2':
				num = 2;
				break;
			}
		}
	}

	std::string ToString()
	{
		std::string res = "";
		switch (face)
		{
		case FaceEnum::UP:
			res += 'U';
			break;
		case FaceEnum::DOWN:
			res += 'D';
			break;
		case FaceEnum::FRONT:
			res += 'F';
			break;
		case FaceEnum::BACK:
			res += 'B';
			break;
		case FaceEnum::RIGHT:
			res += 'R';
			break;
		case FaceEnum::LEFT:
			res += 'L';
			break;
		}

		switch (num)
		{
		case -1:
			res += '\'';
			break;
		case 2:
			res += '2';
			break;
		}

		return res;
	}

	FaceEnum face = FaceEnum::UP;
	int num = 1;
	int layer = 0;
};