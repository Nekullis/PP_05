//----------------------------------------------------------------------
// @filename XInput.cpp
// @author: Saito Ko
// @explanation
// XInput状態のコントローラーの入力を管理するクラス
//----------------------------------------------------------------------
#include "../../Header/Input/XInput.h"
#include "../../Header/Math/mymath.h"

int XInput::_connectNum = 0;
XInput* XInput::_instatnce = nullptr;

//----------------------------------------------------------------------
// @brief コンストラクタ
// @return 無し
//----------------------------------------------------------------------
XInput::XInput() :
	_isConnect(true) 
{
	if(_instatnce == nullptr){
		_instatnce = this;
	}
	_connectNum++;
	_padNum = _connectNum;
	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
		_input.Buttons[i] = 0;
	}
};

//----------------------------------------------------------------------
// @brief コンストラクタ
// @param number パッド番号
// @return 無し
//----------------------------------------------------------------------
XInput::XInput(int number):
	_isConnect(false) 
{
	_padNum = number;
	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
		_input.Buttons[i] = 0;
	}

	ReSet();

};

//----------------------------------------------------------------------
// @brief デストラクタ
// @return 無し
//----------------------------------------------------------------------
XInput::~XInput() {
	if (_isConnect) {
		_connectNum--;
	}
	ReSet();
}

//----------------------------------------------------------------------
// @brief パッドの入力情報を更新
// @return 成功したかどうか
//----------------------------------------------------------------------
bool XInput::Input() {
	// キーの入力、トリガ入力、リリース入力を得る
	unsigned char keyold[PAD_BUTTON_MAX]{};

	//UpdateJoyPad();

	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}
	for (int i = 0; i < 4; i++) {
		keyold[DXINPUT_BUTTON_MAX + i] = _stickInput[i];
	}

	// 入力状態を取得
	GetJoypadXInputState(_padNum, &_input);

	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		// トリガ入力の取得
		_trg[i] = (_input.Buttons[i] ^ keyold[i]) & _input.Buttons[i];
		// リリース入力の取得
		_rel[i] = (_input.Buttons[i] ^ keyold[i]) & ~_input.Buttons[i];
	}

	// LTボタン
	unsigned char triggerButton = _input.LeftTrigger == 0 ? 0 : 1;
	// トリガ入力の取得
	_trg[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & triggerButton;
	// リリース入力の取得
	_rel[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & ~triggerButton;

	// RTボタン
	triggerButton = _input.RightTrigger == 0 ? 0 : 1;
	// トリガ入力の取得
	_trg[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & triggerButton;
	// リリース入力の取得
	_rel[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & ~triggerButton;

	float deadZone = 0.05f;
	// スティック入力値が50%以下なら入力をカットする（デッドゾーン設定）
	_input.ThumbLX = abs(_input.ThumbLX) > SHRT_MAX * deadZone ? _input.ThumbLX : 0;
	_input.ThumbLY = abs(_input.ThumbLY) > SHRT_MAX * deadZone ? _input.ThumbLY : 0;
	_input.ThumbRX = abs(_input.ThumbRX) > SHRT_MAX * deadZone ? _input.ThumbRX : 0;
	_input.ThumbRY = abs(_input.ThumbRY) > SHRT_MAX * deadZone ? _input.ThumbRY : 0;

	_adjustedLStick.x = (float)_input.ThumbLX / (float)SHRT_MAX;
	_adjustedLStick.y = (float)_input.ThumbLY / (float)SHRT_MAX;
	_adjustedRStick.x = (float)_input.ThumbRX / (float)SHRT_MAX;
	_adjustedRStick.y = (float)_input.ThumbRY / (float)SHRT_MAX;

	// 誤差によって範囲を超えた場合にクランプをする
	_adjustedLStick.x = Math::Clamp(-1.0f, 1.0f, _adjustedLStick.x);
	_adjustedLStick.y = Math::Clamp(-1.0f, 1.0f, _adjustedLStick.y);
	_adjustedRStick.x = Math::Clamp(-1.0f, 1.0f, _adjustedRStick.x);
	_adjustedRStick.y = Math::Clamp(-1.0f, 1.0f, _adjustedRStick.y);


	// スティック入力の値を「-1.0f ~ 1.0f」に変換する
	// スティック入力の取得
	// 上　下　左　右
	short stickParam[4] = { _input.ThumbLY, _input.ThumbLX,-_input.ThumbLY,  -_input.ThumbLX };
	short ParamToZero[2] = { _input.ThumbLY,_input.ThumbLX };
	for (int i = 0; i < 4; i++) {
		_stickInput[i] = std::signbit(static_cast<float>(stickParam[i])) ? 0 : 1;
		_stickInput[i] = (ParamToZero[i % 2] != 0 ? 1 : 0) & _stickInput[i];
		// トリガ入力の取得
		_trg[DXINPUT_BUTTON_MAX + i] = (_stickInput[i] ^ keyold[DXINPUT_BUTTON_MAX + i]) & _stickInput[i];
		// リリース入力の取得
		_rel[DXINPUT_BUTTON_MAX + i] = (_stickInput[i] ^ keyold[DXINPUT_BUTTON_MAX + i]) & ~_stickInput[i];
	}



	return true;
};

//----------------------------------------------------------------------
// @brief パッドがつなぎなおされた時に再設定する
// @return 成功したかどうか
//----------------------------------------------------------------------
bool XInput::UpdateJoyPad() {
	int connectNum = GetJoypadNum();
	if (_connectNum != connectNum) {
		_connectNum = connectNum;
		ReSet();
	}

	return true;
};

//----------------------------------------------------------------------
// @brief セットアップを行う
// @return なし
//----------------------------------------------------------------------
void XInput::ReSet() {
	ReSetupJoypad();
	ReSetupInputSystem();
};

//----------------------------------------------------------------------
// @brief 左スティックが動かされているか
// @return 動かされているかどうか
//----------------------------------------------------------------------
bool XInput::IsInputStickLeft() const{
	//左アナログスティックが動かされているならtrue
	if (_input.ThumbLX != 0 || _input.ThumbLY != 0) { return true; }
	return false;
};

//----------------------------------------------------------------------
// @brief 右スティックが動かされているか
// @return 動かされているかどうか
//----------------------------------------------------------------------
bool XInput::IsInputStickRight() const{
	//右アナログスティックが動かされているならtrue
	if (_input.ThumbRX != 0 || _input.ThumbRY != 0) { return true; }
	return false;
};