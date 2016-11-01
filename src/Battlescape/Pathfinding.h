#pragma once
/*
 * Copyright 2010-2016 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <vector>
#include "Position.h"
#include "PathfindingNode.h"
#include "../Mod/MapData.h"

namespace OpenXcom
{

class SavedBattleGame;
class Tile;
class BattleUnit;
struct BattleActionCost;

/**
 * A utility class that calculates the shortest path between two points on the battlescape map.
 */
class Pathfinding
{
private:
	constexpr static int dir_max = 10;
	constexpr static int dir_x[dir_max] = {  0, +1, +1, +1,  0, -1, -1, -1,  0,  0};
	constexpr static int dir_y[dir_max] = { -1, -1,  0, +1, +1, +1,  0, -1,  0,  0};
	constexpr static int dir_z[dir_max] = {  0,  0,  0,  0,  0,  0,  0,  0, +1, -1};

	SavedBattleGame *_save;
	std::vector<PathfindingNode> _nodes;
	int _size;
	BattleUnit *_unit;
	bool _pathPreviewed;
	bool _strafeMove;
	int _totalTUCost;
	bool _modifierUsed;
	MovementType _movementType;
	/// Gets the node at certain position.
	PathfindingNode *getNode(const Position& pos);
	/// Determines whether a tile blocks a certain movementType.
	bool isBlocked(Tile *tile, const int part, BattleUnit *missileTarget, int bigWallExclusion = -1);
	/// Tries to find a straight line path between two positions.
	bool bresenhamPath(const Position& origin, const Position& target, BattleUnit *missileTarget, bool sneak = false, int maxTUCost = 1000);
	/// Tries to find a path between two positions.
	bool aStarPath(const Position& origin, const Position& target, BattleUnit *missileTarget, bool sneak = false, int maxTUCost = 1000);
	/// Determines whether a unit can fall down from this tile.
	bool canFallDown(Tile *destinationTile);
	/// Determines whether a unit can fall down from this tile.
	bool canFallDown(Tile *destinationTile, int size);
	std::vector<int> _path;
public:
	/// Determines whether the unit is going up a stairs.
	bool isOnStairs(const Position &startPosition, const Position &endPosition);
	/// Determines whether or not movement between starttile and endtile is possible in the direction.
	bool isBlocked(Tile *startTile, Tile *endTile, const int direction, BattleUnit *missileTarget);
	static const int DIR_UP = 8;
	static const int DIR_DOWN = 9;
	enum bigWallTypes{ BLOCK = 1, BIGWALLNESW, BIGWALLNWSE, BIGWALLWEST, BIGWALLNORTH, BIGWALLEAST, BIGWALLSOUTH, BIGWALLEASTANDSOUTH, BIGWALLWESTANDNORTH};
	static const int O_BIGWALL = -1;
	static int red;
	static int green;
	static int yellow;
	/// Creates a new Pathfinding class.
	Pathfinding(SavedBattleGame *save);
	/// Cleans up the Pathfinding.
	~Pathfinding();
	/// Calculates the shortest path.
	void calculate(BattleUnit *unit, Position endPosition, BattleUnit *missileTarget = 0, int maxTUCost = 1000);

	/**
	 * Converts direction to a vector. Direction starts north = 0 and goes clockwise.
	 * @param direction Source direction.
	 * @param vector Pointer to a position (which acts as a vector).
	 */
	static void directionToVector(const int direction, Position *vector)
	{
		vector->x = dir_x[direction];
		vector->y = dir_y[direction];
		vector->z = dir_z[direction];
	}

	/**
	 * Converts direction to a vector. Direction starts north = 0 and goes clockwise.
	 * @param vector Pointer to a position (which acts as a vector).
	 * @param dir Resulting direction.
	 */
	static void vectorToDirection(const Position &vector, int &dir)
	{
		dir = -1;
		for (int i = 0; i < 8; ++i)
		{
			if (dir_x[i] == vector.x && dir_y[i] == vector.y)
			{
				dir = i;
				return;
			}
		}
	}
	/// Checks whether a path is ready and gives the first direction.
	int getStartDirection();
	/// Dequeues a direction.
	int dequeuePath();
	/// Gets the TU cost to move from 1 tile to the other.
	int getTUCost(const Position &startPosition, int direction, Position *endPosition, BattleUnit *unit, BattleUnit *target, bool missile);
	/// Aborts the current path.
	void abortPath();
	/// Gets the strafe move setting.
	bool getStrafeMove() const;
	/// Checks, for the up/down button, if the movement is valid.
	bool validateUpDown(BattleUnit *bu, Position startPosition, const int direction, bool missile = false);
	/// Previews the path.
	bool previewPath(bool bRemove = false);
	/// Removes the path preview.
	bool removePreview();
	/// Sets _unit in order to abuse low-level pathfinding functions from outside the class.
	void setUnit(BattleUnit *unit);
	/// Gets all reachable tiles, based on cost.
	std::vector<int> findReachable(BattleUnit *unit, const BattleActionCost &cost);
	/// Gets _totalTUCost; finds out whether we can hike somewhere in this turn or not.
	int getTotalTUCost() const { return _totalTUCost; }
	/// Gets the path preview setting.
	bool isPathPreviewed() const;
	/// Gets the modifier setting.
	bool isModifierUsed() const;
	/// Gets a reference to the path.
	const std::vector<int> &getPath();
	/// Makes a copy to the path.
	std::vector<int> copyPath() const;
};

}
