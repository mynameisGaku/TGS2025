using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;

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

            var mesh = obj.GetComponent<MeshFilter>()?.sharedMesh;
            string meshName = mesh != null
                ? Regex.Replace(mesh.name, @"\s*\([^\)]*\)", "")
                : "Unknown";

            editableObjects.Add(new EditableObject
            {
                GameObject = obj,
                Export = true,
                CustomMeshName = meshName,
                IsCollision = obj.GetComponent<Collider>() != null
            });
        }
    }

    private void OnGUI()
    {
        GUILayout.Label("�G�N�X�|�[�g�c�[��", EditorStyles.boldLabel);

        // �t�@�C��������
        GUILayout.Label("�ۑ��t�@�C�����i�g���q�͎����� .json�j:");
        fileName = GUILayout.TextField(fileName);

        // �I�u�W�F�N�g�ꗗ
        GUILayout.Space(10);
        GUILayout.Label("�G�N�X�|�[�g�ΏۃI�u�W�F�N�g:");
        scrollPosition = GUILayout.BeginScrollView(scrollPosition, GUILayout.Height(300));

        foreach (var item in editableObjects)
        {
            GUILayout.BeginVertical("box");
            item.Export = GUILayout.Toggle(item.Export, $"�G�N�X�|�[�g: {item.GameObject.name}");
            item.CustomMeshName = EditorGUILayout.TextField("���b�V����", item.CustomMeshName);
            item.IsCollision = EditorGUILayout.Toggle("�Փ˔���", item.IsCollision);
            GUILayout.EndVertical();
        }

        GUILayout.EndScrollView();

        GUILayout.Space(10);
        if (GUILayout.Button("�G�N�X�|�[�g"))
        {
            ExportScene();
        }
    }

    private void ExportScene()
    {
        string safeFileName = string.IsNullOrWhiteSpace(fileName) ? "ExportedStage" : fileName.Trim();

        string path = EditorUtility.SaveFilePanel(
            "�ۑ����I��",
            Application.dataPath,
            safeFileName,
            "json"
        );

        if (string.IsNullOrEmpty(path))
        {
            Debug.LogWarning("�ۑ����L�����Z������܂���");
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
                Position = new SimpleVector3(obj.transform.position * 100.0f),
                Rotation = new SimpleVector3(obj.transform.eulerAngles * Mathf.Deg2Rad),
                Scale = new SimpleVector3(obj.transform.localScale),
                IsCollision = item.IsCollision
            };

            exportData.Objects.Add(data);
        }

        string json = JsonConvert.SerializeObject(exportData, Formatting.Indented);
        File.WriteAllText(path, json);
        Debug.Log($"�V�[���f�[�^�������o���܂���: {path}");
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
        public bool IsCollision;
    }
}
