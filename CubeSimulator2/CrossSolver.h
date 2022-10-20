#pragma once
#include <array>
#include <bit>
#include <optional>
#include <vector>
#include <queue>
#include <cassert>
#include <functional>

#include "Cube.h"
#include "MemoryPool.h"

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
			if (!h.has_value()) h = CrossSolver::HCost(state);
			return h.value();
		}
		int FCost()
		{
			return g + HCost();
		}
	private:
		std::optional<int> h = std::nullopt;
	};

public:
	CrossSolver() {}

	Sequence operator()(const Cube<3>& start);

	static int HCost(const Cube<3>& state);

private:
	// Attributes
	MemoryPool<Node> pool = MemoryPool<Node>(576);

	static void EdgeFound(int& h, uint8_t& parityByte, const Cube<3>& state, int fi, int pi);

	static int ColorParity(ColorEnum c1, ColorEnum c2);

	template<typename... T>
	void AppendAdjacentNodes(std::priority_queue<Node*, T...>& nodes, Node* node)
	{
		for (int fi = 0; fi < 6; fi++)
		{
			static constexpr int numLUT[3] = { -1, 1, 2 };
			for (int num = 0; num < 3; num++)
			{
				Turn t((FaceEnum)fi, numLUT[num]);

				Node* newNode = pool.Emplace(node->state, node->g + 1, node, t);
				newNode->state.ApplyTurn(t);
				nodes.push(newNode);
			}
		}
	}

	Sequence ReconstructSequence(Node* end);
};