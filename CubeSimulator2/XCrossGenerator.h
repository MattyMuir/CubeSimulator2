#pragma once
#include "Cube.h"
#include "CrossSolver.h"

class XCrossGenerator
{
public:
	XCrossGenerator() {}

	static Sequence Generate(int length)
	{
		Cube<3> cube;
		CrossSolver solver;

		for (;;)
		{
			// Scramble cube
			cube.Reset();
			Sequence scramble = Sequence::RandomSequence(length);
			cube.ApplySequence(scramble);

			// Solve cross
			Sequence solution = solver(cube);

			// Check if pair is solved
			cube.ApplySequence(solution);

			if (HasPair(cube)) return scramble;
		}
	}

private:
	static bool HasPair(const Cube<3>& cube)
	{
		FaceEnum ring[4] = { FaceEnum::FRONT, FaceEnum::RIGHT, FaceEnum::BACK, FaceEnum::LEFT };

		int pairNum = 0;
		for (int i = 0; i < 4; i++)
		{
			int fi1 = (int)ring[i];
			int fi2 = (int)ring[(i + 3) % 4];
			ColorEnum c1 = cube.faces[fi1][4].col;
			ColorEnum c2 = cube.faces[fi2][4].col;

			bool t1 = (cube.faces[fi1][3].col == c1);
			bool t2 = (cube.faces[fi1][6].col == c1);
			bool t3 = (cube.faces[fi2][5].col == c2);
			bool t4 = (cube.faces[fi2][8].col == c2);
			
			int topTiles[4] = {0, 2, 6, 8};
			bool t5 = (cube.faces[0][topTiles[i]].col == ColorEnum::WHITE);

			if (t1 && t2 && t3 && t4 && t5)
			{
				return true;
			}
		}
		return false;
	}
};