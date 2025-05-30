using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;
using Object = UnityEngine.Object;

public class SceneExporterWindow : EditorWindow
{
    private List<EditableObject> editableObjects = new();
    private Vector2 scrollPosition;
    private string fileName = "ExportedStage";

    [MenuItem("Tools/Scene Exporter")]
    public static void ShowWindow()
    {
        GetWindow<SceneExporterWindow>("Scene Exporter");
    }

    private void OnEnable()
    {
        LoadSceneObjects();
    }

    private void LoadSceneObjects()
    {
        editableObjects.Clear();
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.None);

        foreach (var obj in allObjects)
        {
            if (!obj.activeInHierarchy || obj.transform.parent != null)
                continue;

            if (obj.gameObject.name == "Main Camera" || obj.gameObject.name == "Directional Light")
                continue;

            var mesh = obj.GetComponent<MeshFilter>()?.sharedMesh;
            string meshName = mesh != null
                ? Regex.Replace(mesh.name, @"\s*\([^\)]*\)", "")
                : "Unknown";

            editableObjects.Add(new EditableObject
            {
                GameObject = obj,
                Tag = obj.tag,
                Export = true,
                CustomMeshName = meshName,
                IsCollision = obj.GetComponent<Collider>() != null
            });
        }
    }

    private void OnGUI()
    {
        GUILayout.Label("エクスポートツール", EditorStyles.boldLabel);

        // ファイル名入力
        GUILayout.Label("保存ファイル名（拡張子は自動で .json）:");
        fileName = GUILayout.TextField(fileName);

        // オブジェクト一覧
        GUILayout.Space(10);
        GUILayout.Label("エクスポート対象オブジェクト:");
        scrollPosition = GUILayout.BeginScrollView(scrollPosition, GUILayout.Height(300));

        string[] tags = UnityEditorInternal.InternalEditorUtility.tags;

        foreach (var item in editableObjects)
        {
            GUILayout.BeginVertical("box");
            item.Export = GUILayout.Toggle(item.Export, $"エクスポートする: {item.GameObject.name}");
            item.tagIndex = EditorGUILayout.Popup(item.tagIndex, tags);
            if (GUILayout.Button("SetTag"))
                item.Tag = tags[item.tagIndex];
            GUILayout.Label($"タグ:, {item.Tag}");
            item.CustomMeshName = EditorGUILayout.TextField("メッシュ名", item.CustomMeshName);
            item.IsCollision = EditorGUILayout.Toggle("衝突判定", item.IsCollision);
            GUILayout.EndVertical();
        }

        GUILayout.EndScrollView();

        GUILayout.Space(10);
        if (GUILayout.Button("エクスポート"))
        {
            ExportScene();
        }
    }

    private void ExportScene()
    {
        string safeFileName = string.IsNullOrWhiteSpace(fileName) ? "ExportedStage" : fileName.Trim();

        string path = EditorUtility.SaveFilePanel(
            "保存先を選択",
            Application.dataPath,
            safeFileName,
            "json"
        );

        if (string.IsNullOrEmpty(path))
        {
            Debug.LogWarning("保存がキャンセルされました");
            return;
        }

        var exportData = new ObjectList();

        foreach (var item in editableObjects)
        {
            if (!item.Export) continue;

            var obj = item.GameObject;
            var data = new ObjectData
            {
                Name = obj.name,
                Type = item.CustomMeshName,
                Tag = obj.tag,
                Position = new SimpleVector3(obj.transform.position * 100.0f),
                Rotation = new SimpleVector3(obj.transform.eulerAngles * Mathf.Deg2Rad),
                Scale = new SimpleVector3(obj.transform.localScale),
                IsCollision = item.IsCollision
            };

            exportData.Objects.Add(data);
        }

        string json = JsonConvert.SerializeObject(exportData, Formatting.Indented);
        File.WriteAllText(path, json);
        Debug.Log($"シーンデータを書き出しました: {path}");
    }

    [System.Serializable]
    public class SimpleVector3
    {
        public float x, y, z;

        public SimpleVector3(Vector3 v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
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

    private class EditableObject
    {
        public GameObject GameObject;
        public bool Export;
        public string CustomMeshName;
        public string Tag;
        public bool IsCollision;
        public int tagIndex;
    }
}
