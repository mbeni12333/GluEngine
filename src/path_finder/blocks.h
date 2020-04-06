#pragma once
#include <vector>

enum class BlockType{ StartBlock = 0, EndBlock, ObstacleBlock, NeutralBlock, OpenBlock, ClosedBlock, PathBlock };
using Board = std::vector<std::vector<BlockType>>;