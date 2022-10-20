#pragma once
#include "Color.h"
#include "Face.h"
#include "Turn.h"
#include "Sequence.h"

#include <array>
#include <ranges>

template <size_t Order>
class Cube
{
public:
	Cube()
	{
		Reset();
	}

	Cube(const Cube& other)
	{
		memcpy(faces.data(), other.faces.data(), 6 * Order * Order * sizeof(Color));
	}

	void Reset()
	{
		for (int fi = 0; fi < 6; fi++)
			std::fill(faces[fi].begin(), faces[fi].end(), (ColorEnum)fi);
	}

	void ApplyTurn(Turn turn)
	{
		Axis axis = Axis::STANDING;
		if ((int)turn.face < 2) axis = Axis::EQUATOR;
		else if ((int)turn.face > 3) axis = Axis::MIDDLE;

		// Rotate sides
		Face face = turn.face;
		int num = turn.num;
		int layer = turn.layer;

		if (face.face == FaceEnum::RIGHT) { num *= -1; layer = Order - layer - 1; face.Opposite(); }
		else if (face.face == FaceEnum::UP) { num *= -1; layer = Order - layer - 1; face.Opposite(); }
		else if (face.face == FaceEnum::BACK) { num *= -1; layer = Order - layer - 1; face.Opposite(); }

		// Update sides
		Color temp;
		switch (axis)
		{
		case Axis::MIDDLE:
			switch (num)
			{
			case 1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[2][layer + hi * Order];
					faces[2][layer + hi * Order] = faces[0][layer + hi * Order];
					faces[0][layer + hi * Order] = faces[3][(Order - layer - 1) + (Order - hi - 1) * Order];
					faces[3][(Order - layer - 1) + (Order - hi - 1) * Order] = faces[1][layer + hi * Order];
					faces[1][layer + hi * Order] = temp;
				}
				break;
			case -1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[2][layer + hi * Order];
					faces[2][layer + hi * Order] = faces[1][layer + hi * Order];
					faces[1][layer + hi * Order] = faces[3][(Order - layer - 1) + (Order - hi - 1) * Order];
					faces[3][(Order - layer - 1) + (Order - hi - 1) * Order] = faces[0][layer + hi * Order];
					faces[0][layer + hi * Order] = temp;
				}
				break;
			case -2:
			case 2:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[0][layer + hi * Order];
					faces[0][layer + hi * Order] = faces[1][layer + hi * Order];
					faces[1][layer + hi * Order] = temp;

					temp = faces[2][layer + hi * Order];
					faces[2][layer + hi * Order] = faces[3][(Order - layer - 1) + (Order - hi - 1) * Order];
					faces[3][(Order - layer - 1) + (Order - hi - 1) * Order] = temp;
				}
				break;
			}
			break;
		case Axis::EQUATOR:
			switch (num)
			{
			case 1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[2][hi + layer * Order];
					faces[2][hi + layer * Order] = faces[5][hi + layer * Order];
					faces[5][hi + layer * Order] = faces[3][hi + layer * Order];
					faces[3][hi + layer * Order] = faces[4][hi + layer * Order];
					faces[4][hi + layer * Order] = temp;
				}
				break;
			case -1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[2][hi + layer * Order];
					faces[2][hi + layer * Order] = faces[4][hi + layer * Order];
					faces[4][hi + layer * Order] = faces[3][hi + layer * Order];
					faces[3][hi + layer * Order] = faces[5][hi + layer * Order];
					faces[5][hi + layer * Order] = temp;
				}
				break;
			case -2:
			case 2:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[2][hi + layer * Order];
					faces[2][hi + layer * Order] = faces[3][hi + layer * Order];
					faces[3][hi + layer * Order] = temp;

					temp = faces[4][hi + layer * Order];
					faces[4][hi + layer * Order] = faces[5][hi + layer * Order];
					faces[5][hi + layer * Order] = temp;
				}
				break;
			}
			break;
		case Axis::STANDING:
			switch (num)
			{
			case 1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[0][hi + layer * Order];
					faces[0][hi + layer * Order] = faces[5][(Order - layer - 1) + hi * Order];
					faces[5][(Order - layer - 1) + hi * Order] = faces[1][(Order - hi - 1) + (Order - layer - 1) * Order];
					faces[1][(Order - hi - 1) + (Order - layer - 1) * Order] = faces[4][layer + (Order - hi - 1) * Order];
					faces[4][layer + (Order - hi - 1) * Order] = temp;
				}
				break;
			case -1:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[0][hi + layer * Order];
					faces[0][hi + layer * Order] = faces[4][layer + (Order - hi - 1) * Order];
					faces[4][layer + (Order - hi - 1) * Order] = faces[1][(Order - hi - 1) + (Order - layer - 1) * Order];
					faces[1][(Order - hi - 1) + (Order - layer - 1) * Order] = faces[5][(Order - layer - 1) + hi * Order];
					faces[5][(Order - layer - 1) + hi * Order] = temp;
				}
				break;
			case -2:
			case 2:
				for (int hi = 0; hi < Order; hi++)
				{
					temp = faces[0][hi + layer * Order];
					faces[0][hi + layer * Order] = faces[1][(Order - hi - 1) + (Order - layer - 1) * Order];
					faces[1][(Order - hi - 1) + (Order - layer - 1) * Order] = temp;

					temp = faces[4][layer + (Order - hi - 1) * Order];
					faces[4][layer + (Order - hi - 1) * Order] = faces[5][(Order - layer - 1) + hi * Order];
					faces[5][(Order - layer - 1) + hi * Order] = temp;
				}
				break;
			}
			break;
		}

		// Update Face
		if (layer == 0 || layer == Order - 1)
		{
			face = turn.face;
			num = turn.num;
			layer = turn.layer;
			if (layer == Order - 1) { num *= -1; layer = 0; face.Opposite(); }

			TurnFaceTiles((int)face.face, num);
		}
	}

	void ApplySequence(const Sequence& seq)
	{
		for (const Turn& turn : seq.turns)
			ApplyTurn(turn);
	}

	bool IsSolved() const
	{
		bool col[6] = { false, false, false, false, false, false };
		for (const auto& face : faces)
		{
			col[(int)face[0]] = true;
		}

		if (!(col[0] && col[1] && col[2] && col[3] && col[4] && col[5])) return false;

		for (const auto& face : faces)
		{
			ColorEnum faceCol = face[0];
			for (int i = 1; i < Order * Order; i++)
			{
				if (face[i] != faceCol) return false;
			}
		}

		return true;
	}

	std::array<std::array<Color, Order * Order>, 6> faces;

private:
	void TurnFaceTiles(int fi, int num)
	{
		constexpr int loopNum = Order / 2;
		Color temp;
		switch (num)
		{
		case 1:
			for (int x = 0; x < loopNum; x++)
			{
				for (int y = x; y < Order - 1 - x; y++)
				{
					temp = faces[fi][x + y * Order];
					faces[fi][x + y * Order] = faces[fi][Order - 1 - y + x * Order];
					faces[fi][Order - 1 - y + x * Order] = faces[fi][Order - 1 - x + (Order - 1 - y) * Order];
					faces[fi][Order - 1 - x + (Order - 1 - y) * Order] = faces[fi][y + (Order - 1 - x) * Order];
					faces[fi][y + (Order - 1 - x) * Order] = temp;
				}
			}
			break;
		case -1:
			for (int x = 0; x < loopNum; x++)
			{
				for (int y = x; y < Order - 1 - x; y++)
				{
					temp = faces[fi][x + y * Order];
					faces[fi][x + y * Order] = faces[fi][y + (Order - 1 - x) * Order];
					faces[fi][y + (Order - 1 - x) * Order] = faces[fi][Order - 1 - x + (Order - 1 - y) * Order];
					faces[fi][Order - 1 - x + (Order - 1 - y) * Order] = faces[fi][Order - 1 - y + x * Order];
					faces[fi][Order - 1 - y + x * Order] = temp;
				}
			}
			break;
		case -2:
		case 2:
			for (int x = 0; x < loopNum; x++)
			{
				for (int y = x; y < Order - 1 - x; y++)
				{
					temp = faces[fi][x + y * Order];
					faces[fi][x + y * Order] = faces[fi][Order - 1 - x + (Order - 1 - y) * Order];
					faces[fi][Order - 1 - x + (Order - 1 - y) * Order] = temp;

					temp = faces[fi][y + (Order - 1 - x) * Order];
					faces[fi][y + (Order - 1 - x) * Order] = faces[fi][Order - 1 - y + x * Order];
					faces[fi][Order - 1 - y + x * Order] = temp;
				}
			}
			break;
		}
	}
};