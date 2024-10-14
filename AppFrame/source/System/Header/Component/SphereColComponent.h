//----------------------------------------------------------------------
// @filename DrawComponent.h
// @author: Fukuma Kyohei
// @explanation
// 球の当たり判定に用いるクラス
//----------------------------------------------------------------------
#pragma once
#include "CollisionComponent.h"
class SphereColComponent :public CollisionComponent
{
public:
	SphereColComponent(ObjectBase* owner, int updateowder = 10);
	virtual ~SphereColComponent();
	void Update()override;
};

