using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

public class SceneImporterWindow : EditorWindow
{
    private string jsonFilePath = "";
    private Vector2 scrollPosition;

    [MenuItem("Tools/Scene Importer")]
    public static void ShowWindow()
    {
        GetWindow<SceneImporterWindow>("Scene Importer");
    }

    private void OnGUI()
    {
        GUILayout.Label("シーンインポーター", EditorStyles.boldLabel);

        GUILayout.Space(10);
        GUILayout.Label("読み込むJSONファイル:");

        GUILayout.BeginHorizontal();
        jsonFilePath = EditorGUILayout.TextField(jsonFilePath);
        if (GUILayout.Button("参照", GUILayout.Width(60)))
        {
            string path = EditorUtility.OpenFilePanel("JSONファイルを選択", Application.dataPath, "json");
            if (!string.IsNullOrEmpty(path))
            {
                jsonFilePath = path;
            }
        }
        GUILayout.EndHorizontal();

        GUILayout.Space(20);

        if (GUILayout.Button("読み込み実行"))
        {
            if (string.IsNullOrEmpty(jsonFilePath) || !File.Exists(jsonFilePath))
            {
                EditorUtility.DisplayDialog("エラー", "有効なJSONファイルを選択してください。", "OK");
                return;
            }

            bool confirm = EditorUtility.DisplayDialog(
                "確認",
                "既存のオブジェクトを削除して読み込みを実行します。\nよろしいですか？",
                "ええで", "アカーーン！！");

            if (confirm)
            {
                LoadSceneFromJson(jsonFilePath);
            }
        }
    }

    private void LoadSceneFromJson(string path)
    {
        DeleteAllObjects();

        string json = File.ReadAllText(path);
        ObjectList objectList = JsonConvert.DeserializeObject<ObjectList>(json);

        foreach (var objData in objectList.Objects)
        {
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube); // 仮：Cubeを配置

            go.name = objData.Name;
            go.tag = objData.Tag;

            go.transform.position = objData.Position.ToVector3() / 100.0f;
            Vector3 eulerRad = objData.Rotation.ToVector3();
            go.transform.rotation = Quaternion.Euler(eulerRad * Mathf.Rad2Deg);
            go.transform.localScale = objData.Scale.ToVector3();

            if (!objData.IsCollision)
            {
                DestroyImmediate(go.GetComponent<Collider>());
            }
        }

        Debug.Log("シーン読み込みが完了しました。");
    }

    private void DeleteAllObjects()
    {
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.None);
        foreach (var obj in allObjects)
        {
            if (obj.transform.parent != null)
                continue;

            if (obj.name == "Main Camera" || obj.name == "Directional Light")
                continue;

            DestroyImmediate(obj);
        }
    }

    // データクラス定義（Exporterと同じ構造）
    [System.Serializable]
    public class SimpleVector3
    {
        public float x, y, z;

        public Vector3 ToVector3()
        {
            return new Vector3(x, y, z);
        }
    }

    [System.Serializable]
    public class ObjectData
    {
        public string Name;
        public string Type;
        public string Tag;
        public SimpleVector3 Position;
        public SimpleVector3 Rotation;
        public SimpleVector3 Scale;
        public bool IsCollision;
    }

    [System.Serializable]
    public class ObjectList
    {
        public List<ObjectData> Objects = new();
    }
}
