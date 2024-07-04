#include <vector>
#include <map>

#include "Vector2D.h"

/// <summary>
/// パネル情報
/// </summary>
enum ePanelID : unsigned char
{
	WALL,
	BRANCH,
	GATE,
	NONE
};

/// <summary>
/// 隣接情報のキー
/// </summary>
enum eAdjacentDirection : unsigned char
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

/// <summary>
/// ステージのパネル情報
/// </summary>
class StageData
{
private:
	static StageData* instance;
	std::vector<std::vector<ePanelID>> data;

private:
	StageData() = default;

	// コピーガード
	StageData(const StageData&) = delete;
	StageData& operator = (const StageData&) = delete;
	// ~コピーガード

public:
	/// <summary>
	/// 全てのパネル情報を取得する
	/// </summary>
	/// <returns>全てのパネル情報</returns>
	static const std::vector<std::vector<ePanelID>> GetAll();

	/// <summary>
	/// 隣接するパネル情報を取得
	/// </summary>
	/// <param name="location">現在座標</param>
	/// <returns>隣接するパネル情報</returns>
	static const std::map<eAdjacentDirection, ePanelID> GetAdjacentPanelData(const Vector2D& location);

	/// <summary>
	/// 現在座標のパネル情報を取得する
	/// </summary>
	/// <param name="location">現在座標</param>
	/// <returns>パネル情報</returns>
	static const ePanelID GetPanelData(const Vector2D& location);

	/// <summary>
	/// 座標情報を添字に変換する
	/// </summary>
	/// <param name="location">座標情報</param>
	/// <param name="i">変換後の添字</param>
	/// <param name="j">変換後の添字</param>
	static void ConvertToIndex(const Vector2D& location, int& i, int& j);

private:
	/// <summary>
	/// ステージデータを読み込む
	/// </summary>
	void Load();
};