//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	MODEL_PLAYER		"data/MODEL/cone.obj"			// 読み込むモデル名
//#define	MODEL_PLAYER_PARTS	"data/MODEL/torus.obj"			// 読み込むモデル名

#define		MODEL_PLAYER			"data/MODEL/bear_body.obj"		// 体
#define		MODEL_PLAYER_PARTS_H	"data/MODEL/bear_head.obj"		// 頭
#define		MODEL_PLAYER_PARTS_M	"data/MODEL/bear_mouse.obj"		// 口
#define		MODEL_PLAYER_PARTS_E	"data/MODEL/bear_eye.obj"		// 目
#define		MODEL_PLAYER_PARTS_RA	"data/MODEL/bear_Rarm.obj"		// 右腕
#define		MODEL_PLAYER_PARTS_LA	"data/MODEL/bear_Larm.obj"		// 左腕
#define		MODEL_PLAYER_PARTS_RL	"data/MODEL/bear_Rleg.obj"		// 右足
#define		MODEL_PLAYER_PARTS_LL	"data/MODEL/bear_Lleg.obj"		// 左足
#define		MODEL_PLAYER_PARTS_TAIL	"data/MODEL/bear_tail.obj"		// 左足



#define		VALUE_MOVE			(2.0f)							// 移動量
#define		VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define		PLAYER_SHADOW_SIZE	(1.0f)							// 影の大きさ
#define		PLAYER_OFFSET_Y		(15.0f)							// プレイヤーの足元をあわせる

#define		PLAYER_PARTS_MAX	(8)								// プレイヤーのパーツの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;						// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用


// プレイヤーの階層アニメーションデータ
// プレイヤーの頭を左右に動かしているアニメデータ
//static INTERPOLATION_DATA move_tbl_right[] = {	// pos, rot, scl, frame
//	{ XMFLOAT3(20.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
//	{ XMFLOAT3(20.0f, 20.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
//
//};

static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

//static INTERPOLATION_DATA move_tbl_left[] = {	// pos, rot, scl, frame
//	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
//	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
//
//};

static INTERPOLATION_DATA move_tbl_mouse[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 20, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_eye[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(0.0f, 30.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 0.1f, 1.0f), 10 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_Rarm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Larm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f,0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f,0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Rleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Lleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_tail[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 12, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 12,  0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};



static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	move_tbl_head,
	move_tbl_Rarm,
	move_tbl_Larm,
	move_tbl_Rleg,
	move_tbl_Lleg,
	move_tbl_tail,
	move_tbl_mouse,
	move_tbl_eye,

};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, 0.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 0.2f, 0.2f, 0.2f };

	g_Player.spd = 0.0f;			// 移動スピードクリア
	g_Player.size = PLAYER_SIZE;	// 当たり判定の大きさ

	g_Player.use = true;

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号



	// 階層アニメーション用の初期化処理
	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	// パーツの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = false;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 親の原点からのオフセット座標
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 自分の回転情報
		g_Parts[i].scl = XMFLOAT3(0.2f, 0.2f, 0.2f);	// 自分の拡大縮小

		// 親子関係
		g_Parts[i].parent = &g_Player;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = 0;
	}

	// 頭の設定
	g_Parts[0].use = true;
	g_Parts[0].parent = &g_Player;			// 親をセット
	g_Parts[0].tblNo = 0;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[0].tblMax = sizeof(move_tbl_head) / sizeof(INTERPOLATION_DATA);
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_H, &g_Parts[0].model);

	// 右腕の設定
	g_Parts[1].use = true;
	g_Parts[1].parent = &g_Player;			// 親をセット
	g_Parts[1].tblNo = 1;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[1].tblMax = sizeof(move_tbl_Rarm) / sizeof(INTERPOLATION_DATA);
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_RA, &g_Parts[1].model);

	// 右腕の設定
	g_Parts[2].use = true;
	g_Parts[2].parent = &g_Player;			// 親をセット
	g_Parts[2].tblNo = 2;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[2].tblMax = sizeof(move_tbl_Larm) / sizeof(INTERPOLATION_DATA);
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_LA, &g_Parts[2].model);

	// 右腕の設定
	g_Parts[3].use = true;
	g_Parts[3].parent = &g_Player;			// 親をセット
	g_Parts[3].tblNo = 3;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[3].tblMax = sizeof(move_tbl_Rleg) / sizeof(INTERPOLATION_DATA);
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_RL, &g_Parts[3].model);

	// 右腕の設定
	g_Parts[4].use = true;
	g_Parts[4].parent = &g_Player;			// 親をセット
	g_Parts[4].tblNo = 4;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[4].tblMax = sizeof(move_tbl_Lleg) / sizeof(INTERPOLATION_DATA);
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_LL, &g_Parts[4].model);

	// 右腕の設定
	g_Parts[5].use = true;
	g_Parts[5].parent = &g_Player;			// 親をセット
	g_Parts[5].tblNo = 5;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[5].tblMax = sizeof(move_tbl_tail) / sizeof(INTERPOLATION_DATA);
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_TAIL, &g_Parts[5].model);

	// 口の設定
	g_Parts[6].use = true;
	g_Parts[6].parent = &g_Player;			// 親をセット
	g_Parts[6].tblNo = 6;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[6].tblMax = sizeof(move_tbl_mouse) / sizeof(INTERPOLATION_DATA);
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_M, &g_Parts[6].model);

	// 目の設定
	g_Parts[7].use = true;
	g_Parts[7].parent = &g_Player;			// 親をセット
	g_Parts[7].tblNo = 7;					// 再生するアニメデータの戦闘アドレスをセット
	g_Parts[7].tblMax = sizeof(move_tbl_eye) / sizeof(INTERPOLATION_DATA);
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_E, &g_Parts[7].model);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}

	// パーツの解放処理
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = false;
		}
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	// 移動させちゃう
	if (GetKeyboardPress(DIK_LEFT))
	{	// 左へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{	// 右へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_UP))
	{	// 上へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = XM_PI;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{	// 下へ移動
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = 0.0f;
	}


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);

	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(g_Player.pos, g_Player.rot);
	}

	g_Player.spd *= 0.5f;

	if (g_Player.spd > 0.1f)
	{
		// 階層アニメーション
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// 使われているなら処理する
			if ((g_Parts[i].use == true) && (g_Parts[i].tblMax > 0))
			{	// 線形補間の処理
				int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}

		}
	}


	{	// ポイントライトのテスト
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = true;
		//SetLightData(1, light);
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	// 
	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// モデル描画
	DrawModel(&g_Player.model);



	// パーツの階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		////// パーツの描画処理 //////
		// 1.まずはパーツ自身のSRTを行う
		// 2.親か子供かチェックする
		// 3.子供だった場合、親のmtzWorldと自分のmtxWorldを掛け算する
		// 4.その結果（mtxWorld）を使って自分（パーツ）を描画する


		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			// 親のmtxWorldはどこにあるのか
			// g_Parts[i].parent->mtxWorld = g_Player.mtxWorld
			// g_Parts[i].prent = &g_Player
			// g_Player.mtxWorld

			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																// ↑
																// g_Player.mtxWorldを指している
		}

		// さらなる子パーツのために、自分のmtxWorldを保存している
		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == false) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_Parts[i].model);

	}



	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

