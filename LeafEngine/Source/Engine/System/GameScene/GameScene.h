#ifndef GAMESCENE_H
#define GAMESCENE_H

namespace GameSystem
{

	class GameScene
	{

	public:

		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Update(float delaTime) = 0;
		virtual void Render() = 0;

	};

} // System

#endif