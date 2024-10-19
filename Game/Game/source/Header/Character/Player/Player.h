//----------------------------------------------------------------------
// @filename Player.cpp
// @author: Fukuma Kyohei
// @explanation
// プレイヤークラス
// プレイヤーの処理全般クラス
//----------------------------------------------------------------------
#pragma once
#include "../CharacterBase.h"
class Player : public CharacterBase
{
public:
	Player(class ModeBase* game);
	virtual ~Player();
	void Process()	override;
	void Render()	override;
	void Debug()	override;
protected:
	class Sphere* m_Sphere;
	class SphereColComponent* m_Col;
	class DrawSphereComponent* m_Draw;
	class CameraComponent* m_Camera;
};