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
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube); // ���FCube��z�u

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

        Debug.Log("�V�[���ǂݍ��݂��������܂����B");
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

    // �f�[�^�N���X��`�iExporter�Ɠ����\���j
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
