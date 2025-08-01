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
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.InstanceID);

        foreach (var obj in allObjects)
        {
            if (!obj.activeInHierarchy || obj.transform.parent != null)
                continue;

            if (obj.gameObject.name == "Main Camera" || obj.gameObject.name == "Directional Light")
                continue;

            MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
            Mesh mesh = null;
            if (meshFilter)
            {
                mesh = meshFilter.sharedMesh;
            }

            CharaSpawnPointDesc charaSpawnerDesc = null;
            BallSpawnerDesc ballSpawnerDesc = null;
            TrampolineParam trampolineParam = null;

            string meshName = "";
            if (mesh != null)
            {
                meshName = Regex.Replace(mesh.name, @"\s*\([^\)]*\)", "");
            }
            else
            {
                if (obj.tag == "CharaSpawnPoint")
                {
                    meshName = "CharaSpawnPoint";
                    charaSpawnerDesc = obj.gameObject.GetComponent<CharaSpawnPointDesc>();
                    if (charaSpawnerDesc == null)
                    {
                        charaSpawnerDesc = obj.AddComponent<CharaSpawnPointDesc>();
                    }
                }
                else if (obj.tag == "BallSpawner")
                {
                    meshName = "BallSpawner";
                    ballSpawnerDesc = obj.gameObject.GetComponent<BallSpawnerDesc>();
                    if (ballSpawnerDesc == null)
                    {
                        ballSpawnerDesc = obj.AddComponent<BallSpawnerDesc>();
                    }
                }
                else if (obj.tag == "MovableArea")
                {
                    meshName = "MovableArea";
                    ballSpawnerDesc = obj.gameObject.GetComponent<BallSpawnerDesc>();
                    if (ballSpawnerDesc == null)
                    {
                        ballSpawnerDesc = obj.AddComponent<BallSpawnerDesc>();
                    }
                }
                else if (obj.tag == "Trampoline")
                {
                    meshName = "Trampoline";
                    trampolineParam = obj.gameObject.GetComponent<TrampolineParam>();
                    if (trampolineParam == null)
                    {
                        trampolineParam = obj.AddComponent<TrampolineParam>();
                    }
                }
                else
                {
                    meshName = "Unknown";
                }
            }

            editableObjects.Add(new EditableObject
            {
                GameObject = obj,
                Tag = obj.tag,
                Export = true,
                CharaSpawnPointDesc = charaSpawnerDesc,
                BallSpawnerDesc = ballSpawnerDesc,
                TrampolineParam = trampolineParam,
                CustomMeshName = meshName,
                IsCollision = obj.GetComponent<Collider>() != null
            });
        }
    }

    private void OnGUI()
    {
        editableObjects.RemoveAll(item => item.GameObject == null);
        GUILayout.Label("エクスポートツール", EditorStyles.boldLabel);
        GUILayout.Label("保存ファイル名（拡張子は自動で .json）:");
        fileName = GUILayout.TextField(fileName);
        GUILayout.Space(10);
        GUILayout.Label("エクスポート対象オブジェクト:");
        scrollPosition = GUILayout.BeginScrollView(scrollPosition, GUILayout.Height(400));

        string[] tags = UnityEditorInternal.InternalEditorUtility.tags;

        foreach (var item in editableObjects)
        {
            GUILayout.BeginVertical("box");
            item.Export = GUILayout.Toggle(item.Export, $"エクスポートする: {item.GameObject.name}");
            item.CustomMeshName = EditorGUILayout.TextField("メッシュ名", item.CustomMeshName);
            item.IsCollision = EditorGUILayout.Toggle("衝突判定", item.IsCollision);
            item.tagIndex = EditorGUILayout.Popup(item.tagIndex, tags);
            if (GUILayout.Button("SetTag"))
                item.Tag = tags[item.tagIndex];
            GUILayout.Label($"タグ: {item.Tag}");
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
                CharaSpawnPointDesc = item.CharaSpawnPointDesc != null ? new CharaSpawnPointData(item.CharaSpawnPointDesc) : null,
                BallSpawnerDesc = item.BallSpawnerDesc != null ? new BallSpawnerData(item.BallSpawnerDesc) : null,
                TrampolineParam = item.TrampolineParam != null ? new TrampolineData(item.TrampolineParam) : null,
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
    public class CharaSpawnPointData
    {
        public SimpleVector3 SPAWN_INITIAL_VELOCITY;
        public SimpleVector3 SPAWNER_POSITION;
        public string SPAWNER_NAME;

        public CharaSpawnPointData(CharaSpawnPointDesc desc)
        {
            SPAWN_INITIAL_VELOCITY = new SimpleVector3(desc.SPAWN_INITIAL_VELOCITY);
        }
    }

    [System.Serializable]
    public class BallSpawnerData
    {
        public float INTERVAL_SEC;
        public float INTERVAL_SEC_RANDOM_RANGE;
        public int SPAWN_AMOUNT_ONCE_MAX;
        public int SPAWN_AMOUNT_ONCE_MIN;
        public int SPAWN_AMOUNT_ONCE_RANDOM_RANGE;
        public int SPAWN_AMOUNT_INITIAL;
        public SimpleVector3 SPAWN_RANGE;
        public SimpleVector3 SPAWN_INITIAL_VELOCITY;

        public BallSpawnerData(BallSpawnerDesc desc)
        {
            INTERVAL_SEC = desc.INTERVAL_SEC;
            INTERVAL_SEC_RANDOM_RANGE = desc.INTERVAL_SEC_RANDOM_RANGE;
            SPAWN_AMOUNT_ONCE_MAX = desc.SPAWN_AMOUNT_ONCE_MAX;
            SPAWN_AMOUNT_ONCE_MIN = desc.SPAWN_AMOUNT_ONCE_MIN;
            SPAWN_AMOUNT_ONCE_RANDOM_RANGE = desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE;
            SPAWN_AMOUNT_INITIAL = desc.SPAWN_AMOUNT_INITIAL;
            SPAWN_RANGE = new SimpleVector3(desc.SPAWN_RANGE);
            SPAWN_INITIAL_VELOCITY = new SimpleVector3(desc.SPAWN_INITIAL_VELOCITY);
        }
    }

    [System.Serializable]
    public class TrampolineData
    {
        public float RepulsionForce;

        public TrampolineData(TrampolineParam desc)
        {
            RepulsionForce = desc.RepulsionForce;
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
        public CharaSpawnPointData CharaSpawnPointDesc;
        public BallSpawnerData BallSpawnerDesc;
        public TrampolineData TrampolineParam;
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
        public CharaSpawnPointDesc CharaSpawnPointDesc;
        public BallSpawnerDesc BallSpawnerDesc;
        public TrampolineParam TrampolineParam;
        public string CustomMeshName;
        public string Tag;
        public bool IsCollision;
        public int tagIndex;
    }
}
