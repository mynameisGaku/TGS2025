#pragma once
#include <list>
#include <unordered_map>
#include "framework/gameObject.h"
#include "src/scene/play/chara/CharaSpawnPoint.h"

/// <summary>
/// キャラクターのスポナーを管理するクラス。
/// ステージ情報を読み込むときにJSONからセットで読み込んでいます。
/// </summary>
/// <author>藤本樂</author>
class CharaSpawnPointManager : public GameObject
{
public:
    CharaSpawnPointManager();
    ~CharaSpawnPointManager();

    void Update() override;
    void Draw() override;

    // スポーン地点を追加する
    void AddSpawnPoint(int hModel, const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);

    // 与えられた座標から最も近いスポナーを取得する
    CharaSpawnPoint* Get_Near(const Vector3& pos);
    // 与えられた座標から最も遠いスポナーを取得する
    CharaSpawnPoint* Get_Far(const Vector3& pos);
    // 存在するスポナーからランダムに一つ取得する
    CharaSpawnPoint* Get_Random();
    // 存在するスポナーの中で最も使用率が低いスポナーを取得する
    CharaSpawnPoint* Get_LowUsageRate();
    // 存在するスポナーの中で最も使用率が高いスポナーを取得する
    CharaSpawnPoint* Get_HighUsageRate();
private:
    // 生成されたスポナーのリスト
    std::list<CharaSpawnPoint*> m_SpawnPoints;
};