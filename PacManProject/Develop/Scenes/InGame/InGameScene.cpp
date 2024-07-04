#include "InGameScene.h"
#include "../../Objects/Player/Player.h"
#include "../../Objects/Enemy/EnemyBase.h"
#include "../../Objects/Wall/Wall.h"
#include "../../Objects/Food/Food.h"
#include "../../Objects/Food/PowerFood.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <fstream>

InGameScene::InGameScene() 
	: player(nullptr)
	, back_ground_image(NULL)
	, back_ground_sound(NULL)
	, pause_flag(false)
{

}

InGameScene::~InGameScene()
{

}

void InGameScene::Initialize()
{
	// マップデータ読み込み生成処理
	LoadStageMapCSV();
	LoadStageMapFoodsCSV();

	// スクリーンオフセットを設定
	screen_offset.y = D_OBJECT_SIZE * 3.0f;

	// 背景画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	back_ground_image = rm->GetImages("Resource/Images/map.png")[0];
	// BGMの読み込み
	back_ground_sound = rm->GetSounds("Resource/Sounds/start-music.mp3");

	PlaySoundMem(back_ground_sound, DX_PLAYTYPE_BACK);
}

eSceneType InGameScene::Update(const float& delta_second)
{
	InputManager* input = InputManager::GetInstance();
	
	if (input->GetKeyDown(KEY_INPUT_P) || input->GetButtonDown(XINPUT_BUTTON_START))
	{
		pause_flag = !pause_flag;
	}

	if (!pause_flag)
	{
		// 親クラスの更新処理を呼び出す
		__super::Update(delta_second);

		// 全ての餌を食べたら、再スタート
		if (player->GetFoodCount() >= 244)
		{
			return eSceneType::re_start;
		}

		// プレイヤーが死んだら、再スタート
		if (player->GetDestroy())
		{
			return eSceneType::re_start;
		}
	}

	// シーン情報を返却する
	return GetNowSceneType();
}

void InGameScene::Draw() const
{
	// 背景画像の描画
	DrawRotaGraph(336, 432, 1.5, 0.0, back_ground_image, TRUE);

	// 親クラスの描画処理を呼び出す
	__super::Draw();

	// UI系の描画処理
	if (pause_flag)
	{
		DrawString(10, 10, " P A U S E ", GetColor(255, 255, 255), TRUE);
	}
}

void InGameScene::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

const eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::in_game;
}

/// <summary>
/// 当たり判定確認処理
/// </summary>
/// <param name="target">1つ目のゲームオブジェクト</param>
/// <param name="partner">2つ目のゲームオブジェクト</param>
void InGameScene::CheckCollision(GameObjectBase* target, GameObjectBase* partner)
{
	// ヌルポチェック
	if (target == nullptr || partner == nullptr)
	{
		return;
	}

	// 当たり判定情報を取得
	CapsuleCollision tc = target->GetCollision();
	CapsuleCollision pc = partner->GetCollision();

	// 当たり判定が有効か確認する
	if (tc.IsCheckHitTarget(pc.object_type) || pc.IsCheckHitTarget(tc.object_type))
	{
		
		// 線分の始点と終点を設定する
		tc.point[0] += target->GetLocation();
		tc.point[1] += target->GetLocation();
		pc.point[0] += partner->GetLocation();
		pc.point[1] += partner->GetLocation();

		// カプセル同士の当たり判定
		if (IsCheckCollision(tc, pc))
		{
			// 当たっていることを通知する
			target->OnHitCollision(partner);
			partner->OnHitCollision(target);
		}
		
	}
}

/// <summary>
/// ステージマップ（壁）読み込み処理
/// </summary>
void InGameScene::LoadStageMapCSV()
{
	// 読み込むファイル名
	std::string file_name = "Resource/Map/StageMap.csv";
	// 指定ファイルを読み込む
	std::ifstream ifs(file_name);

	// エラーチェック
	if (ifs.fail())
	{
		throw (file_name + "が開けません\n");
	}

	// ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(ifs, line))
	{
		// 文字列を書式指定で分解して値を各変数に格納する
		char mode = NULL;		// 生成するオブジェクトモード
		int x_size = NULL;		// 生成する数（横）
		int y_size = NULL;		// 生成する数（縦）
		int spos_x = 0;			// 生成位置情報（横）
		int spos_y = 0;			// 生成位置情報（縦）
		sscanf_s(
			line.c_str(),
			"%c,%d,%d,%d,%d",
			&mode, (unsigned int)sizeof(mode),
			&x_size, &y_size,
			&spos_x, &spos_y
		);

		// モードによって、生成するオブジェクトを変える
		Vector2D generate_location;		// 生成位置の情報
		switch (mode)
		{
			// 壁
			case '#':
				generate_location = (Vector2D((float)(spos_x - 1), (float)(spos_y - 1)) * D_OBJECT_SIZE) + (D_OBJECT_SIZE / 2.0f);
				CreateObject<Wall>(generate_location)->SetWallData(x_size, y_size);
				break;
			// プレイヤー
			case 'P':
				generate_location = (Vector2D((float)(spos_x - 1), (float)(spos_y - 1)) * D_OBJECT_SIZE) + (D_OBJECT_SIZE / 2.0f);
				player = CreateObject<Player>(generate_location);
				break;
			// エネミー
			case 'E':
				generate_location = (Vector2D((float)(spos_x - 1), (float)(spos_y - 1)) * D_OBJECT_SIZE) + (D_OBJECT_SIZE / 2.0f);
				CreateObject<EnemyBase>(generate_location);
				break;
			// 上記以外
			default:
				break;
		}

	}
}

/// <summary>
/// ステージマップ餌読み込み処理
/// </summary>
void InGameScene::LoadStageMapFoodsCSV()
{
	FILE* fp = NULL;
	std::string file_name = "Resource/Map/StageMapFoods.csv";

	// 指定されたファイルを開く
	errno_t result = fopen_s(&fp, file_name.c_str(), "r");

	// エラーチェック
	if (result != 0)
	{
		throw (file_name + "が開けません");
	}

	int x = 0;
	int y = 0;

	// ファイル内の文字を確認していく
	while (true)
	{
		// ファイルから1文字抽出する
		int c = fgetc(fp);

		// 抽出した文字がEOFならループ終了
		if (c == EOF)
		{
			break;
		}
		// 抽出した文字がドットなら、通常餌を生成
		else if (c == '.')
		{
			Vector2D generate_location = (Vector2D((float)x, (float)y) * D_OBJECT_SIZE) + (D_OBJECT_SIZE / 2.0f);

			CreateObject<Food>(generate_location);
			x++;
		}
		// 抽出した文字がドットなら、パワー餌を生成
		else if (c == 'P')
		{
			Vector2D generate_location = (Vector2D((float)x, (float)y) * D_OBJECT_SIZE) + (D_OBJECT_SIZE / 2.0f);
			CreateObject<PowerFood>(generate_location);
			x++;
		}
		// 抽出した文字が空白文字なら、生成しないで次の文字を見に行く
		else if (c == ' ')
		{
			x++;
		}
		// 抽出した文字が改行文字なら、次の行を見に行く
		else if (c == '\n')
		{
			x = 0;
			y++;
		}

	}

	// 開いたファイルを閉じる
	fclose(fp);
}
