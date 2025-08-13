using System.Collections.ObjectModel;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace MainCanvasEditor
{
    public class EventPreset
    {
        [JsonPropertyName("Key")] public string Key { get; set; } = "";                // 内部キー（C++側で識別する文字列）
        [JsonPropertyName("DisplayName")] public string DisplayName { get; set; } = ""; // UI表示名
        [JsonPropertyName("Category")] public string Category { get; set; } = "";       // 任意の分類
        [JsonPropertyName("Description")] public string Description { get; set; } = ""; // 既定説明
        [JsonPropertyName("DefaultTiming")] public string DefaultTiming { get; set; } = "TUI_EVENT_TRIGGER_TIMING_ENTER";
        [JsonPropertyName("DefaultArgument")] public object? DefaultArgument { get; set; } // 任意JSON
    }

    public class EventCatalog
    {
        [JsonPropertyName("Version")] public int Version { get; set; } = 1;
        [JsonPropertyName("Presets")] public ObservableCollection<EventPreset> Presets { get; set; } = new();

        public static EventCatalog LoadFrom(string path)
        {
            try
            {
                if (!File.Exists(path))
                    return DefaultCatalog();

                var json = File.ReadAllText(path);
                var opt = new JsonSerializerOptions
                {
                    ReadCommentHandling = JsonCommentHandling.Skip,
                    AllowTrailingCommas = true
                };
                var cat = JsonSerializer.Deserialize<EventCatalog>(json, opt);
                return cat ?? DefaultCatalog();
            }
            catch
            {
                return DefaultCatalog();
            }
        }

        public static void EnsureFile(string path)
        {
            var dir = Path.GetDirectoryName(path);
            if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir)) Directory.CreateDirectory(dir);
            if (!File.Exists(path))
            {
                var def = DefaultCatalog();
                var json = JsonSerializer.Serialize(def, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText(path, json);
            }
        }

        public static EventCatalog DefaultCatalog()
        {
            return new EventCatalog
            {
                Version = 1,
                Presets =
                {
                    new EventPreset
                    {
                        Key="GameStart",
                        DisplayName="GameStart (シーン開始)",
                        Category="System",
                        Description="ゲーム開始処理",
                        DefaultTiming="TUI_EVENT_TRIGGER_TIMING_ENTER",
                        DefaultArgument= new { SceneName="Main" }
                    },
                    new EventPreset
                    {
                        Key="PlaySound",
                        DisplayName="PlaySound (効果音再生)",
                        Category="Audio",
                        Description="SEを再生",
                        DefaultTiming="TUI_EVENT_TRIGGER_TIMING_RELEASE",
                        DefaultArgument= new { Cue="ui_click", Volume=1.0 }
                    },
                    new EventPreset
                    {
                        Key="OpenURL",
                        DisplayName="OpenURL (外部リンク)",
                        Category="System",
                        Description="既定ブラウザでURLを開く",
                        DefaultTiming="TUI_EVENT_TRIGGER_TIMING_RELEASE",
                        DefaultArgument= new { Url="https://example.com" }
                    },
                    new EventPreset
                    {
                        Key="ActivateCanvas",
                        DisplayName="ActivateCanvas (キャンバス切替)",
                        Category="UI",
                        Description="別キャンバスを有効化",
                        DefaultTiming="TUI_EVENT_TRIGGER_TIMING_RELEASE",
                        DefaultArgument= new { Canvas="MainCanvas" }
                    }
                }
            };
        }
    }
}
