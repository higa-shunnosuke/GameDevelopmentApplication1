#include "StageData.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "../Objects/GameObjectBase.h"

StageData* StageData::instance = nullptr;

/// <summary>
/// ステージデータを読み込む
/// </summary>
void StageData::Load()
{
	// 読み込むファイル名
	std::string file_name = "Resource/Map/StageMap.csv";

	// ファイルストリーム
	std::ifstream ifs;
	// 指定ファイルを読み込む
	ifs.open(file_name, std::ios::in);

	// エラーチェック
	if(ifs.fail())
	{
		throw (file_name + "が開けません\n");
	}

	// データを初期化
	std::vector<ePanelID> init_data(28, ePanelID::NONE);
	data = std::vector<std::vector<ePanelID>>(31, init_data);

	// 壁のデータを作成
	std::string line;
	while(std::getline(ifs, line))
	{
		// １行読み込んだデータをストリングストリームに変換
		std::stringstream line_stream = std::stringstream(line);

		// 符号を取得
		std::string mode;
		std::getline(line_stream, mode, ',');

		// idの初期値
		ePanelID id = ePanelID::WALL;

		// 壁の登録
		switch((*mode.c_str()))
		{
			case 'G':
				id = ePanelID::GATE;	// idをWALLにする
			case '#':
			{
				// 挿入開始地点と挿入数を取得
				std::string line_data;
				std::getline(line_stream, line_data, ',');
				int x_size = std::stoi(line_data);

				std::getline(line_stream, line_data, ',');
				int y_size = std::stoi(line_data);

				std::getline(line_stream, line_data, ',');
				int x_start = std::stoi(line_data) - 1;

				std::getline(line_stream, line_data, ',');
				int y_start = std::stoi(line_data) - 1;

				// 壁データを登録
				if(x_size == 1)
				{
					for(int i = y_start; i < (y_start + y_size); i++)
					{
						data[i][x_start] = id;
					}
				}
				else
				{
					for(int i = x_start; i < (x_start + x_size); i++)
					{
						data[y_start][i] = id;
					}
				}
			}
			break;

			case 'B':
			{
				// 挿入地点を取得
				std::string line_data;
				std::getline(line_stream, line_data, ',');
				int x_start = std::stoi(line_data) - 1;

				std::getline(line_stream, line_data, ',');
				int y_start = std::stoi(line_data) - 1;

				// 分岐路を登録
				data[y_start][x_start] = ePanelID::BRANCH;
			}
			break;

			default:
				break;
		}
	}
}

/// <summary>
/// 全てのパネル情報を取得する
/// </summary>
/// <returns>全てのパネル情報</returns>
const std::vector<std::vector<ePanelID>> StageData::GetAll()
{
	if(instance == nullptr)
	{
		// オブジェクトの作成
		instance = new StageData();
		instance->Load();
	}

	return instance->data;
}

/// <summary>
/// 隣接するパネル情報を取得
/// </summary>
/// <param name="location">現在座標</param>
/// <returns>隣接するパネル情報</returns>
const std::map<eAdjacentDirection, ePanelID> StageData::GetAdjacentPanelData(const Vector2D& location)
{
	if(instance == nullptr)
	{
		// オブジェクトの作成
		instance = new StageData();
		instance->Load();
	}

	// 戻り値となるデータを作成する
	std::map<eAdjacentDirection, ePanelID> ret = {
		{ eAdjacentDirection::UP, ePanelID::NONE },
		{ eAdjacentDirection::DOWN, ePanelID::NONE },
		{ eAdjacentDirection::LEFT, ePanelID::NONE },
		{ eAdjacentDirection::RIGHT, ePanelID::NONE }
	};

	// 座標を添字に変換
	int i = 0, j = 0;
	ConvertToIndex(location, i, j);

	// 範囲チェック
	if((0 <= i) || (i < instance->data.size()))
	{
		// LEFTデータ作成
		if(0 <= (j - 1))
		{
			ret[eAdjacentDirection::LEFT] = instance->data[i][j - 1];
		}

		// RIGHTデータ作成
		if((j + 1) < instance->data[i].size())
		{
			ret[eAdjacentDirection::RIGHT] = instance->data[i][j + 1];
		}
	}

	// 範囲チェック
	if((0 <= j) || (j < instance->data[0].size()))
	{
		// UPデータ作成
		if(0 <= (i - 1))
		{
			ret[eAdjacentDirection::UP] = instance->data[i - 1][j];
		}

		// DOWNデータ作成
		if((i + 1) < instance->data.size())
		{
			ret[eAdjacentDirection::DOWN] = instance->data[i + 1][j];
		}
	}

	return ret;
}

/// <summary>
/// 現在座標のパネル情報を取得する
/// </summary>
/// <param name="location">現在座標</param>
/// <returns>パネル情報</returns>
const ePanelID StageData::GetPanelData(const Vector2D& location)
{
	if(instance == nullptr)
	{
		// オブジェクトの作成
		instance = new StageData();
		instance->Load();
	}

	// 座標を添字に変換
	int i = 0, j = 0;
	ConvertToIndex(location, i, j);

	// iとjの範囲チェック
	if((i < 0) || (j < 0) || (instance->data.size() <= i) || (instance->data[0].size() <= j))
	{
		return ePanelID::NONE;
	}

	return instance->data[i][j];
}

/// <summary>
/// 座標情報を添字に変換する
/// </summary>
/// <param name="location">座標情報</param>
/// <param name="i">変換後の添字</param>
/// <param name="j">変換後の添字</param>
void StageData::ConvertToIndex(const Vector2D& location, int& i, int& j)
{
	j = static_cast<int>(location.x / D_OBJECT_SIZE);
	i = static_cast<int>(location.y / D_OBJECT_SIZE);
}