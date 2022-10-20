#pragma once
#include <vector>
#include <string>
#include <sstream>

#include "Turn.h"

class Sequence
{
public:
	Sequence() {}
	Sequence(std::string_view notation)
	{
		std::stringstream ss;
		ss << notation;
		std::string token;

		while (std::getline(ss, token, ' '))
		{
			turns.emplace_back(token);
		}
	}

	static Sequence RandomSequence(int length)
	{
		constexpr int turnNum[3] = { 1, -1, 2 };
		Sequence s;
		s.turns.reserve(length);
		Axis lastAxis = (Axis)(rand() % 3);
		for (int i = 0; i < length; i++)
		{
			Axis axis;
			do
			{
				axis = (Axis)(rand() % 3);
			} while (axis == lastAxis);
			lastAxis = axis;

			FaceEnum face = (FaceEnum)((int)axis * 2 + rand() % 2);
			int num = turnNum[rand() % 3];
			s.turns.emplace_back(face, num, 0);
		}

		return s;
	}

	std::string ToString()
	{
		std::string res = "";
		for (int i = 0; i < turns.size(); i++)
		{
			if (i > 0) res += ' ';
			res += turns[i].ToString();
		}
		return res;
	}

	operator std::string()
	{
		return ToString();
	}

	std::vector<Turn> turns;
};