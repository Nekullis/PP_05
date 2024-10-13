//----------------------------------------------------------------------
// @filename DrawComponent.h
// @author: Fukuma Kyohei
// @explanation
// OBBの当たり判定に用いるクラス
//----------------------------------------------------------------------
#include "../../AppFrame/source/System/Header/Component/OBBColComponent.h"
#include "../../AppFrame/source/System/Header/Component/Object/ObjectBase.h"
#include "../../AppFrame/source/System/Header/Collision/MyStructure.h"

//----------------------------------------------------------------------
// @brief コンストラクタ
// @param owner オーナー
// @param updateowder 更新順番
// @return なし
//----------------------------------------------------------------------
OBBColComponent::OBBColComponent(ObjectBase* owner, int updateowder) :CollisionComponent(owner, updateowder){
	m_Col = new OBB();
	mType = COLLISIONTYPE::OBB;
}

//----------------------------------------------------------------------
// @brief デストラクタ
// @return なし
//----------------------------------------------------------------------
OBBColComponent::~OBBColComponent(){
}

//----------------------------------------------------------------------
// @brief 更新処理
// @return なし
//----------------------------------------------------------------------
void OBBColComponent::Update(){
	CollisionComponent::Update();
}
