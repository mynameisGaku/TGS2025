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
        GUILayout.Label("�V�[���C���|�[�^�[", EditorStyles.boldLabel);
        GUILayout.Space(10);
        GUILayout.Label("�ǂݍ���JSON�t�@�C��:");

        GUILayout.BeginHorizontal();
        jsonFilePath = EditorGUILayout.TextField(jsonFilePath);
        if (GUILayout.Button("�Q��", GUILayout.Width(60)))
        {
            string path = EditorUtility.OpenFilePanel("JSON�t�@�C����I��", Application.dataPath, "json");
            if (!string.IsNullOrEmpty(path))
            {
                jsonFilePath = path;
            }
        }
        GUILayout.EndHorizontal();

        GUILayout.Space(20);

        if (GUILayout.Button("�ǂݍ��ݎ��s"))
        {
            if (string.IsNullOrEmpty(jsonFilePath) || !File.Exists(jsonFilePath))
            {
                EditorUtility.DisplayDialog("�G���[", "�L����JSON�t�@�C����I�����Ă��������B", "OK");
                return;
            }

            bool confirm = EditorUtility.DisplayDialog(
                "�m�F",
                "�����̃I�u�W�F�N�g���폜���ēǂݍ��݂����s���܂��B\n��낵���ł����H",
                "������", "�A�J�[�[���I�I");

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
            GameObject go = null;

            // --- �v���n�u��T���ăC���X�^���X�� ---
            string prefabPath = $"Assets/{objData.Type}.prefab";
            GameObject prefab = AssetDatabase.LoadAssetAtPath<GameObject>(prefabPath);
            if (prefab != null)
            {
                go = (GameObject)PrefabUtility.InstantiatePrefab(prefab);
            }
            else
            {
                Debug.LogWarning($"Prefab not found for type: {objData.Type}, creating empty object.");
                go = GameObject.CreatePrimitive(PrimitiveType.Cube);  // ����Cube�𐶐�
            }

            // ��{���̕���
            go.name = objData.Name;
            go.tag = objData.Tag;
            go.transform.position = objData.Position.ToVector3() / 100.0f;
            Vector3 eulerRad = objData.Rotation.ToVector3();
            go.transform.rotation = Quaternion.Euler(eulerRad * Mathf.Rad2Deg);
            go.transform.localScale = objData.Scale.ToVector3();

            if (!objData.IsCollision)
            {
                Collider col = go.GetComponent<Collider>();
                if (col != null) DestroyImmediate(col);
            }

            // --- Desc�n�̕��� ---
            if (objData.CharaSpawnerDesc != null)
            {
                var charaComp = go.GetComponent<CharaSpawnerDesc>() ?? go.AddComponent<CharaSpawnerDesc>();
                charaComp.SPAWN_FIRST_VELOCITY = objData.CharaSpawnerDesc.SPAWN_FIRST_VELOCITY.ToVector3();
                charaComp.SPAWNER_POSITION = objData.CharaSpawnerDesc.SPAWNER_POSITION.ToVector3();
                charaComp.SPAWNER_NAME = objData.CharaSpawnerDesc.SPAWNER_NAME;
            }

            if (objData.BallSpawnerDesc != null)
            {
                var ballComp = go.GetComponent<BallSpawnerDesc>() ?? go.AddComponent<BallSpawnerDesc>();
                ballComp.INTERVAL_SEC = objData.BallSpawnerDesc.INTERVAL_SEC;
                ballComp.INTERVAL_SEC_RANDOM_RANGE = objData.BallSpawnerDesc.INTERVAL_SEC_RANDOM_RANGE;
                ballComp.SPAWN_AMOUNT_ONCE_MAX = objData.BallSpawnerDesc.SPAWN_AMOUNT_ONCE_MAX;
                ballComp.SPAWN_AMOUNT_ONCE_MIN = objData.BallSpawnerDesc.SPAWN_AMOUNT_ONCE_MIN;
                ballComp.SPAWN_AMOUNT_ONCE_RANDOM_RANGE = objData.BallSpawnerDesc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE;
                ballComp.SPAWN_AMOUNT_INITIAL = objData.BallSpawnerDesc.SPAWN_AMOUNT_INITIAL;
                ballComp.SPAWN_RANGE = objData.BallSpawnerDesc.SPAWN_RANGE.ToVector3();
                ballComp.SPAWN_FIRST_VELOCITY = objData.BallSpawnerDesc.SPAWN_FIRST_VELOCITY.ToVector3();
                ballComp.SPAWNER_POSITION = objData.BallSpawnerDesc.SPAWNER_POSITION.ToVector3();
                ballComp.SPAWNER_NAME = objData.BallSpawnerDesc.SPAWNER_NAME;
            }
        }

        Debug.Log("�V�[���ǂݍ��݂��������܂����B");
    }

    private void DeleteAllObjects()
    {
        GameObject[] allObjects = Object.FindObjectsByType<GameObject>(FindObjectsSortMode.None);

        List<GameObject> targets = new List<GameObject>();

        foreach (var obj in allObjects)
        {
            if (obj.transform.parent != null)
                continue;
            if (obj.name == "Main Camera" || obj.name == "Directional Light")
                continue;

            targets.Add(obj);
        }

        foreach (var target in targets)
        {
            DestroyImmediate(target);
        }
    }

    [System.Serializable] public class SimpleVector3 { public float x, y, z; public Vector3 ToVector3() => new Vector3(x, y, z); }
    [System.Serializable] public class CharaSpawnerData { public SimpleVector3 SPAWN_FIRST_VELOCITY; public SimpleVector3 SPAWNER_POSITION; public string SPAWNER_NAME; }
    [System.Serializable]
    public class BallSpawnerData
    {
        public float INTERVAL_SEC, INTERVAL_SEC_RANDOM_RANGE;
        public int SPAWN_AMOUNT_ONCE_MAX, SPAWN_AMOUNT_ONCE_MIN, SPAWN_AMOUNT_ONCE_RANDOM_RANGE, SPAWN_AMOUNT_INITIAL;
        public SimpleVector3 SPAWN_RANGE, SPAWN_FIRST_VELOCITY, SPAWNER_POSITION;
        public string SPAWNER_NAME;
    }
    [System.Serializable]
    public class ObjectData
    {
        public string Name, Type, Tag;
        public SimpleVector3 Position, Rotation, Scale;
        public CharaSpawnerData CharaSpawnerDesc;
        public BallSpawnerData BallSpawnerDesc;
        public bool IsCollision;
    }
    [System.Serializable] public class ObjectList { public List<ObjectData> Objects = new(); }
}
