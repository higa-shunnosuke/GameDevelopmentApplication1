#pragma once

#include "../Objects/GameObjectBase.h"
#include <vector>
#include <string>

/// <summary>
/// シーンタイプ
/// </summary>
enum class eSceneType
{
	title,
	in_game,
	re_start,
	result,
	exit,
};

/// <summary>
/// シーン基底クラス
/// </summary>
class SceneBase
{
protected:
	// 各シーンが所有する情報
	std::vector<GameObjectBase*>	create_list;	// 生成を行うオブジェクトリスト
	std::vector<GameObjectBase*>	object_list;	// 更新と描画を行うオブジェクトリスト
	std::vector<GameObjectBase*>	destroy_list;	// 破棄を行うオブジェクトリスト
	Vector2D						screen_offset;	// スクリーンオフセット

public:
	SceneBase() :
		create_list(),
		object_list(),
		destroy_list(),
		screen_offset(0.0f)
	{

	}
	virtual ~SceneBase()
	{
		// 解放忘れ防止
		Finalize();
	}

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize()
	{
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual eSceneType Update(const float& delta_second)
	{
		// 生成するオブジェクトがあれば、オブジェクトリスト内に挿入する
		if(!create_list.empty())
		{
			for(GameObjectBase* obj : create_list)
			{
				// レイヤー情報を基に順番を入れ替える
				int z_layer = obj->GetZLayer();
				std::vector<GameObjectBase*>::iterator itr = object_list.begin();	// オブジェクトリストの先頭
				// リストの末尾になるまで走査する
				for(; itr != object_list.end(); itr++)
				{
					// Zレイヤーが大きい場所に要素を追加する
					// 例 itr->ZLayer{1, 1, 2, 3}、z_layer = 2 の時
					//    itr->ZLayer{1, 1, 2, z_layer, 3}とする
					if(z_layer < (*itr)->GetZLayer())
					{
						break;
					}
				}
				// リストの途中に挿入する
				object_list.insert(itr, obj);
			}
			// 生成リストを解放する
			create_list.clear();
		}

		// リスト内のオブジェクトを更新する
		for(GameObjectBase* obj : object_list)
		{
			obj->Update(delta_second);
		}

		// 当たり判定確認処理
		for(int i = 0; i < object_list.size(); i++)
		{
			if(eMobilityType::Stationary == object_list[i]->GetMobility())
			{
				continue;
			}

			for(int j = 0; j < object_list.size(); j++)
			{
				if(i == j)
				{
					continue;
				}

				CheckCollision(object_list[i], object_list[j]);
			}
		}

		// 破棄リスト内が空でない場合、リスト内のオブジェクトを破棄する
		if(!destroy_list.empty())
		{
			for(GameObjectBase* obj : destroy_list)
			{
				std::vector<GameObjectBase*>::iterator itr = object_list.begin();	// オブジェクトリストの先頭
				// リストの末尾になるまで走査する
				for(; itr != object_list.end(); itr++)
				{
					// リスト内にあれば、削除する
					if((*itr) == obj)
					{
						object_list.erase(itr);
						obj->Finalize();
						delete obj;
						break;
					}
				}
			}
			// 動的配列の開放
			destroy_list.clear();
		}

		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const
	{
		// オブジェクトリスト内のオブジェクトを描画する
		for(GameObjectBase* obj : object_list)
		{
			obj->Draw(screen_offset);
		}
	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		// オブジェクトリスト内のオブジェクトを破棄する
		DestoryAllObject();

		// 動的配列の解放
		if(!create_list.empty())
		{
			create_list.clear();
		}
		if(!destroy_list.empty())
		{
			destroy_list.clear();
		}
	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const eSceneType GetNowSceneType() const = 0;

	/// <summary>
	/// 当たり判定確認処理
	/// </summary>
	/// <param name="target">1つ目のゲームオブジェクト</param>
	/// <param name="partner">2つ目のゲームオブジェクト</param>
	virtual void CheckCollision(GameObjectBase* target, GameObjectBase* partner)
	{
	}

public:
	/// <summary>
	/// ゲームオブジェクト生成処理
	/// </summary>
	/// <typeparam name="OBJECT">生成するゲームオブジェクトクラス</typeparam>
	/// <param name="generate_location">生成位置</param>
	/// <returns>生成したゲームオブジェクトのポインタ</returns>
	template <class OBJECT>
	OBJECT* CreateObject(const Vector2D& generate_location)
	{
		// 生成するゲームオブジェクトクラスを動的確保
		OBJECT* new_instance = new OBJECT();
		// GameObjectBaseを継承しているか確認
		GameObjectBase* new_object = dynamic_cast<GameObjectBase*>(new_instance);
		// エラーチェック
		if(new_object == nullptr)
		{
			// 動的確保したメモリの開放
			delete new_instance;
			// 例外テキストを通知
			throw std::string("ゲームオブジェクトが生成できませんでした");
			return nullptr;
		}
		// シーン情報の設定
		new_object->SetOwnerScene(this);
		// オブジェクトの初期化
		new_object->Initialize();
		// 位置情報の設定
		new_object->SetLocation(generate_location);
		// 生成オブジェクトリストの末尾に格納する
		create_list.push_back(new_object);
		// 生成したゲームオブジェクトのポインタを返す
		return new_instance;
	}

	/// <summary>
	/// オブジェクト破棄処理
	/// </summary>
	/// <param name="target">破棄を行うゲームオブジェクトのポインタ</param>
	void DestroyObject(GameObjectBase* target)
	{
		// ヌルポチェック
		if(target == nullptr)
		{
			return;
		}

		// 破棄オブジェクトリスト内にあれば、追加しない
		for(GameObjectBase* obj : destroy_list)
		{
			if(obj == target)
			{
				return;
			}
		}

		// 破棄を行うオブジェクトリストに追加する
		destroy_list.push_back(target);
	}

	/// <summary>
	/// スクリーンオフセット情報取得処理
	/// </summary>
	/// <returns>スクリーンオフセット値</returns>
	const Vector2D GetScreenOffset() const
	{
		return screen_offset;
	}

private:
	/// <summary>
	/// シーン内オブジェクト破棄処理
	/// </summary>
	void DestoryAllObject()
	{
		// オブジェクトリストが空なら処理を終了する
		if(object_list.empty())
		{
			return;
		}
		// オブジェクトリスト内のオブジェクトを削除する
		for(GameObjectBase* obj : object_list)
		{
			obj->Finalize();
			delete obj;
		}
		// 動的配列の解放
		object_list.clear();
	}
};
