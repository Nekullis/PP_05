//----------------------------------------------------------------------
// @filename Quaternion.cpp
// @author: Saito Ko
// @explanation
// クォータニオンの計算を行うクラス
//----------------------------------------------------------------------
#include "../../Header/Math/Quaternion.h"
#include <math.h>
#include "../../../Application/UtilMacro.h"

//----------------------------------------------------------------------
// @brief コンストラクタ
// @return なし
//----------------------------------------------------------------------
Quaternion::Quaternion(){
	identity();
};

//----------------------------------------------------------------------
// @brief コンストラクタ
// @param w 回転角
// @param x 回転軸のx成分
// @param y 回転軸のy成分
// @param z 回転軸のz成分
// @return なし
//----------------------------------------------------------------------
Quaternion::Quaternion(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
};

//----------------------------------------------------------------------
// @brief X軸回転のクォータニオンを設定
// @param theta 回転角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetToRotateX(float theta){
	// 	半角に変換する
	float thetaOver2 = theta * 0.5f; 

	w = cos(thetaOver2);
	x = sin(thetaOver2);
	y = 0.0f;
	z = 0.0f;
};

//----------------------------------------------------------------------
// @brief Y軸回転のクォータニオンを設定
// @param theta 回転角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetToRotateY(float theta){
	// 	半角に変換する
	float thetaOver2 = theta * 0.5f;

	w = cos(thetaOver2);
	x = 0.0f;
	y = sin(thetaOver2);
	z = 0.0f;
};

//----------------------------------------------------------------------
// @brief Z軸回転のクォータニオンを設定
// @param theta 回転角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetToRotateZ(float theta){
	// 	半角に変換する
	float thetaOver2 = theta * 0.5f;

	w = cos(thetaOver2);
	x = 0.0f;
	y = 0.0f;
	z = sin(thetaOver2);
};

//----------------------------------------------------------------------
// @brief マトリックスからクォータニオンを設定
// @param matrix マトリックス
// @return なし
//----------------------------------------------------------------------
void Quaternion::SetToMatrix(MATRIX& matrix){
	float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
		float w, x, y, z;

		if (trace > 0) {
			float s = 0.5f / std::sqrtf(trace + 1.0f);
			w = 0.25f / s;
			x = (matrix.m[2][1] - matrix.m[1][2]) * s;
			y = (matrix.m[0][2] - matrix.m[2][0]) * s;
			z = (matrix.m[1][0] - matrix.m[0][1]) * s;
		}
		else {
			if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2]) {
				float s = 2.0f * std::sqrtf(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]);
				w = (matrix.m[2][1] - matrix.m[1][2]) / s;
				x = 0.25f * s;
				y = (matrix.m[0][1] + matrix.m[1][0]) / s;
				z = (matrix.m[0][2] + matrix.m[2][0]) / s;
			}
			else if (matrix.m[1][1] > matrix.m[2][2]) {
				float s = 2.0f * std::sqrtf(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][0]);
				w = (matrix.m[0][2] - matrix.m[2][0]) / s;
				x = (matrix.m[0][1] + matrix.m[1][0]) / s;
				y = 0.25f * s;
				z = (matrix.m[1][2] + matrix.m[2][1]) / s;
			}
			else {
				float s = 2.0f * std::sqrtf(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]);
				w = (matrix.m[1][0] - matrix.m[0][1]) / s;
				x = (matrix.m[0][2] + matrix.m[2][0]) / s;
				y = (matrix.m[1][2] + matrix.m[2][1]) / s;
				z = 0.25f * s;
			}
		}

		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
}

//----------------------------------------------------------------------
// @brief X軸回転のクォータニオンを生成
// @param theta 回転角
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Quaternion::CreateRotateX(float theta){
	Quaternion q;
	q.SetToRotateX(theta);
	return q;
};

//----------------------------------------------------------------------
// @brief Y軸回転のクォータニオンを生成
// @param theta 回転角
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Quaternion::CreateRotateY(float theta){
	Quaternion q;
	q.SetToRotateY(theta);
	return q;
};

//----------------------------------------------------------------------
// @brief Z軸回転のクォータニオンを生成
// @param theta 回転角
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Quaternion::CreateRotateZ(float theta){
	Quaternion q;
	q.SetToRotateZ(theta);
	return q;
};

//----------------------------------------------------------------------
// @brief 軸回転のクォータニオンを設定
// @param axis 回転軸
// @param theta 回転角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetToRotateAxis(const Vector3D& axis, float theta){
	// 回転軸が正規化されていなかったらエラー
	if(fabs(axis.Len() - 1.0f) < 0.01f) {
		MessageBox(NULL, "正規化されていないです", "", MB_OK);
		return ;
	};

	// 	半角に変換する
	float	thetaOver2 = theta * .5f;
	// 	sin値を算出
	float	sinThetaOver2 = sin(thetaOver2);

	w = cos(thetaOver2);
	x = axis.x * sinThetaOver2;
	y = axis.y * sinThetaOver2;
	z = axis.z * sinThetaOver2;
};

void	Quaternion::Normalize(){
	// 長さを算出
	float len = sqrt(w * w + x * x + y * y + z * z);

	// 長さが-だったらエラー
	if (len < 0.0f) {
		MessageBox(NULL, "長さが−です", "", MB_OK);
		return ;
	};


	float	tmp = 1.0f / len;
	w *= tmp;
	x *= tmp;
	y *= tmp;
	z *= tmp;

};

//----------------------------------------------------------------------
// @brief 回転角を取得
// @return 回転角
//----------------------------------------------------------------------
float	Quaternion::GetToRotationAngle() const {
	// w = cos(θ / 2)なので。
	return std::acos(w) * 2.0f;
};

//----------------------------------------------------------------------
// @brief 回転軸を取得
// @return 回転軸
//----------------------------------------------------------------------
Vector3D	Quaternion::GetToRotationAxis() const {
	// sin(θ / 2) / |v| = sin(θ / 2)
	// sin^2(x) + cos^2(x) = 1より
	float sinThetaOver2 = sqrt(1.0f - w * w);
	float tmp = 1.0f / sinThetaOver2;
	return Vector3D(x * tmp, y * tmp, z * tmp);
};

//----------------------------------------------------------------------
// @brief クォータニオンの内積を取得
// @param q クォータニオン
// @return 内積
//----------------------------------------------------------------------
float Quaternion::Dot(const Quaternion& q){
	float result = ::Dot(*this, q);
	return result;
};

//----------------------------------------------------------------------
// @brief オブジェクト空間から慣性空間に変換
// @param orientation オイラー角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetRotateObjectToInertial(const Vector3D& orientation){
	auto SinCos = [](float* s, float* c, float theta) {
		*s = sin(theta);
		*c = cos(theta);
	};

	// オブジェクト空間から慣性空間に変換
	// 半角のサインとコサインを計算
	//ｙｘｚ　ヘディング・ピッチ・バンク
	float	sp, sb, sh;
	float	cp, cb, ch;
	SinCos(&sp, &cp, orientation.x * 0.5f);
	SinCos(&sh, &ch, orientation.y * 0.5f);
	SinCos(&sb, &cb, orientation.z * 0.5f);

	w = ch * cp * cb + sh * sp * sb;
	x = ch * sp * cb + sh * cp * sb;
	y = -ch * sp * sb + sh * cp * cb;
	z = -sh * sp * cb + ch * cp * sb;
};

//----------------------------------------------------------------------
// @brief 慣性空間からオブジェクト空間に変換
// @param orientation オイラー角
// @return なし
//----------------------------------------------------------------------
void	Quaternion::SetRotateInertialToObject(const Vector3D& orientation){
	auto SinCos = [](float* s, float* c, float theta) {
		*s = sin(theta);
		*c = cos(theta);
	};

	// 慣性空間からオブジェクト空間に変換
	// 半角のサインとコサインを計算

	float	sp, sb, sh;
	float	cp, cb, ch;
	SinCos(&sp, &cp, orientation.x * 0.5f);
	SinCos(&sh, &ch, orientation.y * 0.5f);
	SinCos(&sb, &cb, orientation.z * 0.5f);

	w = ch * cp * cb + sh * sp * sb;
	x = -ch * sp * cb - sh * cp * sb;
	y = ch * sp * sb - sh * cb * cp;
	z = sh * sp * cb - ch * cp * sb;
};

//----------------------------------------------------------------------
// @brief クォータニオンの掛け算
// @param q クォータニオン
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Quaternion::operator *(const Quaternion& q)  const {
	Quaternion tmp;

	tmp.w = w * q.w - x * q.x - y * q.y - z * q.z;
	tmp.x =  w * q.x + x * q.w + y * q.z - z * q.y;
	tmp.y =  w * q.y + y * q.w + z * q.x - x * q.z;
	tmp.z =  w * q.z + z * q.w + x * q.y - y * q.x;

	return tmp;
};

//----------------------------------------------------------------------
// @brief クォータニオンの掛け算
// @param q クォータニオン
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion& Quaternion::operator *=(const Quaternion& q){
	*this = *this * q;
	return *this;
};

//----------------------------------------------------------------------
// @brief クォータニオンをz-y-x系オイラー角に変換
// @return オイラー角
//----------------------------------------------------------------------
Vector3D Quaternion::byEuler() {
	// クォータニオンをz-y-x系オイラー角に変換
	float roll, pitch, yaw;
	float sin, cos;

	// roll X軸回転
	sin = 2 * (w * x + y * z);//32
	cos = 1 - 2 * (x * x + y * y);//33
	roll = atan2(sin, cos);

	// pitch Y軸回転
	sin = sqrt(1 + 2 * (w * y - x * z));//31
	cos = sqrt(1 - 2 * (w * y - x * z));//31
	pitch = 2 * atan2(sin, cos) - PI / 2;

	// yaw Z軸回転
	sin = 2 * (w * z + x * y);//21
	cos = 1 - 2 * (y * y + z * z);//11
	yaw = atan2(sin, cos);

	return Vector3D(roll, pitch, yaw);
};

//----------------------------------------------------------------------
// @brief クォータニオンをDxlibのマトリックスに変換
// @return マトリックス
//----------------------------------------------------------------------
MATRIX Quaternion::byDxlibMatrix(){
	MATRIX m;
	m.m[0][0] = 1 - 2 * (y * y) - 2 * (z * z);
	m.m[0][1] = (2 * x * y) - (2 * z * w);
	m.m[0][2] = (2 * x * z) + (2 * y * w);
	m.m[0][3] = 0;

	m.m[1][0] = (2 * x * y) + (2 * z * w);
	m.m[1][1] = 1 - 2 * (x * x) - 2 * (z * z);
	m.m[1][2] = (2 * y * z) - (2 * x * w);
	m.m[1][3] = 0;

	m.m[2][0] = (2 * x * z) - (2 * y * w);
	m.m[2][1] = (2 * y * z) + (2 * x * w);
	m.m[2][2] = 1 - 2 * (x * x) - 2 * (y * y);
	m.m[2][3] = 0;

	m.m[3][0] = 0;
	m.m[3][1] = 0;
	m.m[3][2] = 0;
	m.m[3][3] = 1;

	return m;
};

//----------------------------------------------------------------------
// @brief クォータニオンの内積を取得
// @param left クォータニオン
// @param right クォータニオン
// @return 内積
//----------------------------------------------------------------------
float Dot(const Quaternion& left, const Quaternion& right){
	return left.w * right.w + left.x * right.x + left.y * right.y + left.z * right.z;
};

//----------------------------------------------------------------------
// @brief クォータニオンの共役を取得
// @param q クォータニオン
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Conjugate(const Quaternion& q){
	Quaternion result;

	result.w = q.w;

	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;

	return result;
};

//----------------------------------------------------------------------
// @brief クォータニオンの指数を取得
// @param q クォータニオン
// @param exponent 指数
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Pow(const Quaternion& q, float exponent){

	// 恒等四元数かチェック
	if (fabs(q.w) > 0.9999f) {
		return q;
	}

	// alpha = theta/2
	float	alpha = acos(q.w);

	// αに指数を掛ける
	float	newAlpha = alpha * exponent;

	Quaternion result;
	result.w = cos(newAlpha);
	float	mult = sin(newAlpha) / sin(alpha);
	result.x = q.x * mult;
	result.y = q.y * mult;
	result.z = q.z * mult;

	return result;
};

//----------------------------------------------------------------------
// @brief クォータニオンを用いてベクトルを回転
// @param v ベクトル
// @param q クォータニオン
// @return 回転後のベクトル
//----------------------------------------------------------------------
Vector3D RotateVectorByQuaternion(const Vector3D& v, const Quaternion& q) {
	// クォータニオンとベクトルの積　q*v*q^-1 より
	Quaternion conjQuat = Conjugate(q);
	Quaternion vecQuat = Quaternion(0,v.x,v.y,v.z);
	Quaternion result = q * vecQuat * conjQuat;
	return Vector3D(result.x, result.y, result.z);
};

//----------------------------------------------------------------------
// @brief クォータニオンの球面線形補間
// @param p クォータニオン
// @param q クォータニオン
// @param t 時間
// @return クォータニオン
//----------------------------------------------------------------------
Quaternion Slerp(const Quaternion& p, const Quaternion& q, float t) {
	
	// 時間がだったらそれに合わせた端点を返す
	if (t <= 0.0f) return p;
	if (t >= 1.0f) return q;

	// 無い席を用いてcosを計算

	float cosOmega = Dot(p, q);

	//負の内積の場合、qを反転させる
	// q と -q は同じ回転を表すが、異なる球面線形補間を生成する場合がある
	//正確な角度を用いt回転するためににqか-qのどちらかを選択する

	float qw = p.w;
	float qx = p.x;
	float qy = p.y;
	float qz = p.z;
	if (cosOmega < 0.0f) {
		qw *= -1;
		qx *= -1;
		qy *= -1;
		qz *= -1;
		cosOmega *= -1;
	}

	// 単位四元数なので内積は <= 1.0f になる
	if(cosOmega < 1.1f){
		MessageBox(NULL, "内積が1.0f以上です", "", MB_OK);
		return Quaternion();
	}

	// 補間用の端数を計算
	// 四元数がほとんど同じか調べる
	float k0, k1;
	if (cosOmega > 0.9999f) {

		//非常に近い場合は線形補間を行う
		k0 = 1.0f - t;
		k1 = t;
	}
	else {

		//三角関数の加法定理を用いてsinを計算
		// sin^2(x) + cos^2(x) = 1
		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);

		// sinとcosから角度を計算
		float omega = atan2(sinOmega, cosOmega);

		//徐算が1回で済むように分母の逆数を計算
		float oneOverSinOmega = 1.0f / sinOmega;

		// 補間パラメータを計算
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	Quaternion result;
	result.x = k0 * p.x + k1 * qx;
	result.y = k0 * p.y + k1 * qy;
	result.z = k0 * p.z + k1 * qz;
	result.w = k0 * p.w + k1 * qw;

	return result;
};