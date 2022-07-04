#pragma once
#include <array>
#include <vector>
#include <queue>
#include <cassert>
#include <functional>

#include "Cube.h"
#include "MemoryPool.h"

consteval std::array<int, 16> GenerateParityCases()
{
	std::array<int, 16> ret = std::array<int, 16>();
	for (int i = 0; i < 16; i++)
	{
		// Count number of positive bits
		int n = 0;
		for (int b = 0; b < 4; b++)
		{
			if (i & (1 << b)) n++;
		}

		// Compute h from n
		int h = 2 * (n - 1);

		// Add 1 if no pieces have 0 pairty
		if (!(i & 1)) h++;

		ret[i] = h;
	}

	return ret;
}

class CrossSolver
{
private:
	class Node
	{
	public:
		Node(const Cube<3>& state_, int g_, Node* cameFrom_, Turn move_)
			: state(state_), g(g_), cameFrom(cameFrom_), move(move_) {}

		Cube<3> state;
		int g;
		Node* cameFrom;
		Turn move;

		int HCost()
		{
			if (h == -1)
				h = CrossSolver::HCost(state);

			return h;
		}
		int FCost()
		{
			return g + HCost();
		}
	private:
		int h = -1;
	};

public:
	CrossSolver() {}

	Sequence operator()(const Cube<3>& start)
	{
		// Initialize queue with custom predicate
		auto pred = [](Node* n1, Node* n2) { return n1->FCost() > n2->FCost(); };
		std::priority_queue<Node*, std::vector<Node*>, decltype(pred)> nodes(pred);

		// Add startng node to the queue
		Node* startNode = new Node(start, 0, nullptr, Turn());
		nodes.push(startNode);

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
		delete startNode;

		pool.Clear();

		return result;
	}

	static int HCost(const Cube<3>& state)
	{
		int h = 0;
		uint8_t parityByte = 0;
		for (int fi = 0; fi < 6; fi++)
		{
			for (int pi = 1; pi < 9; pi += 2)
			{
				if (state.faces[fi][pi].col == ColorEnum::WHITE) // White tile found
					EdgeFound(h, parityByte, state, fi, pi);
			}
		}

		static constexpr std::array<int, 16> parityCases = GenerateParityCases();

		return h + parityCases[parityByte];
	}

private:
	// Attributes
	MemoryPool<Node> pool = MemoryPool<Node>(576);

	static void EdgeFound(int& h, uint8_t& parityByte, const Cube<3>& state, int fi, int pi)
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

		ColorEnum edgeOther = state.faces[otherFI][otherPI].col;
		assert(edgeOther != ColorEnum::WHITE);
		ColorEnum adjCenter = state.faces[otherFI][4].col;

		parityByte |= ColorParity(edgeOther, adjCenter);
	}

	static int ColorParity(ColorEnum c1, ColorEnum c2)
	{
		static constexpr int ringIndexLUT[6] = { -1, -1, 0, 2, 1, 3 };

		int ringIndex1 = ringIndexLUT[(int)c1];
		int ringIndex2 = ringIndexLUT[(int)c2];

		int d = (ringIndex2 - ringIndex1 + 4) % 4;
		return (1 << d);
	}

	template<typename... T>
	void AppendAdjacentNodes(std::priority_queue<Node*, T...>& nodes, Node* node)
	{
		for (int fi = 0; fi < 6; fi++)
		{
			for (int num = 0; num < 3; num++)
			{
				int numLUT[3] = { -1, 1, 2 };
				Turn t((FaceEnum)fi, numLUT[num]);

				Node* newNode = pool.Emplace(node->state, node->g + 1, node, t);
				newNode->state.ApplyTurn(t);
				nodes.push(newNode);
			}
		}
	}

	Sequence ReconstructSequence(Node* end)
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
};