#include "CrossSolver.h"

static consteval std::array<int, 16> GenerateParityCases()
{
	std::array<int, 16> ret = std::array<int, 16>();
	for (unsigned i = 0; i < 16; i++)
	{
		// Count number of positive bits
		int n = std::popcount(i);

		// Compute h from n
		int h = 2 * (n - 1);

		// Add 1 if no pieces have 0 pairty
		if (!(i & 1)) h++;

		ret[i] = h;
	}

	return ret;
}

Sequence CrossSolver::operator()(const Cube<3>& start)
{
	// Initialize queue with custom predicate
	auto pred = [](Node* n1, Node* n2) { return n1->FCost() > n2->FCost(); };
	std::priority_queue<Node*, std::vector<Node*>, decltype(pred)> nodes(pred);

	// Add starting node to the queue
	nodes.push(pool.Emplace(start, 0, nullptr, Turn()));

	Sequence result;
	for (;;)
	{
		Node* current = nodes.top();
		nodes.pop();

		if (current->HCost() == 0 && current->g <= 8)
		{
			result = ReconstructSequence(current);
			break;
		}

		AppendAdjacentNodes(nodes, current);
	}

	// Free memory
	pool.Clear();

	return result;
}

int CrossSolver::HCost(const Cube<3>& state)
{
	int h = 0;
	uint8_t parityByte = 0;
	for (int fi = 0; fi < 6; fi++)
	{
		for (int pi = 1; pi < 9; pi += 2)
		{
			if (state.faces[fi][pi] == ColorEnum::WHITE) // White tile found
				EdgeFound(h, parityByte, state, fi, pi);
		}
	}

	static constexpr std::array<int, 16> parityCases = GenerateParityCases();

	return h + parityCases[parityByte];
}

void CrossSolver::EdgeFound(int& h, uint8_t& parityByte, const Cube<3>& state, int fi, int pi)
{
	// Check if piece has parity
	if (fi > 1)
	{
		if (pi == 1 || pi == 7)
		{
			// Piece is parity-less
			h += 2;
			return;
		}
	}

	if (fi != 0) h++;

	// Find piece parity
	int edgeTileIndex = (pi - 1) / 2 + fi * 4;

	static constexpr std::pair<int, int> otherTile[24] = { { 2, 7 }, { 5, 7 }, { 4, 7 }, { 3, 7 },
														{ 3, 1 }, { 5, 1 }, { 4, 1 }, { 2, 1 },
														{ 1, 8 }, { 5, 5 }, { 4, 3 }, { 0, 1 },
														{ 1, 1 }, { 4, 5 }, { 5, 3 }, { 0, 8 },
														{ 1, 5 }, { 2, 5 }, { 3, 3 }, { 0, 5 },
														{ 1, 3 }, { 3, 5 }, { 2, 3 }, { 0, 3 } };

	// Use LUT to determine the correspodning tile for current tile
	auto [otherFI, otherPI] = otherTile[edgeTileIndex];

	ColorEnum edgeOther = state.faces[otherFI][otherPI];
	assert(edgeOther != ColorEnum::WHITE);
	ColorEnum adjCenter = state.faces[otherFI][4];

	parityByte |= ColorParity(edgeOther, adjCenter);
}

int CrossSolver::ColorParity(ColorEnum c1, ColorEnum c2)
{
	static constexpr int ringIndexLUT[6] = { -1, -1, 0, 2, 1, 3 };

	int ringIndex1 = ringIndexLUT[(int)c1];
	int ringIndex2 = ringIndexLUT[(int)c2];

	int d = (ringIndex2 - ringIndex1 + 4) % 4;
	return (1 << d);
}

Sequence CrossSolver::ReconstructSequence(Node* end)
{
	Sequence s;

	Node* current = end;
	s.turns.reserve(current->g);
	while (current->g != 0)
	{
		s.turns.push_back(current->move);
		current = current->cameFrom;
	}

	// Reverse turn list
	int seqLength = s.turns.size();
	for (int i = 0; i < seqLength / 2; i++)
	{
		std::swap(s.turns[i], s.turns[seqLength - 1 - i]);
	}

	return s;
}