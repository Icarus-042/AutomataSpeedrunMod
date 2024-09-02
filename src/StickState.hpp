#pragma once

namespace AutomataMod {

/**
 * X: -1000 full left, 1000 full right
 * Y: -1000 full up, 1000 full down
 */
struct StickState {
	float leftX;
	float leftY;
	float rightX;
	float rightY;
};

} // namespace AutomataMod
