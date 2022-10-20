#pragma once
#include "Cube.h"
#include "CrossSolver.h"

class XCrossGenerator
{
public:
	XCrossGenerator() {}

	static Sequence Generate(int length);

private:
	static bool HasPair(const Cube<3>& cube);
};