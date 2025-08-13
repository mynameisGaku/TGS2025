using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text.Json.Serialization;
using System.Windows;
using System.Windows.Media.Imaging;

namespace MainCanvasEditor
{

    // ====== JSON ルート ======
    public class CanvasDocument : INotifyPropertyChanged
    {
        [JsonPropertyName("CanvasName")] public string CanvasName { get; set; }
        [JsonPropertyName("Begin")] public Point2D Begin { get; set; }
        [JsonPropertyName("End")] public Point2D End { get; set; }
        [JsonPropertyName("IsFitScreen")] public bool IsFitScreen { get; set; }
        [JsonPropertyName("IsDefaultActivate")] public bool IsDefaultActivate { get; set; }
        [JsonPropertyName("UIList")] public ObservableCollection<UIItem> UIList { get; set; } = new();

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string n)
        {
            PropertyChanged?.Invoke(this, new(n));
        }
    }

    public class Point2D
    {
        [JsonPropertyName("X")] public int X { get; set; }
        [JsonPropertyName("Y")] public int Y { get; set; }
    }

    // ====== UIイベント（JSONそのまま保持） ======
    public class UIEvent
    {
        [JsonPropertyName("Event")] public string Event { get; set; } = "";
        [JsonPropertyName("Description")] public string Description { get; set; } = "";
        [JsonPropertyName("Timing")] public string Timing { get; set; } = "";
        // 形は自由なので object として保持（JsonElement が入ってきます）
        [JsonPropertyName("Argument")] public object Argument { get; set; }

        public UIEvent Clone()
        {
            return new UIEvent
            {
                Event = this.Event,
                Description = this.Description,
                Timing = this.Timing,
                Argument = this.Argument // JsonElement でもそのまま保持できる
            };
        }
    }

    // ====== UI1個分（JSONと1:1） ======
    public class UIItem
    {
        [JsonPropertyName("NAME")] public string NAME { get; set; }
        [JsonPropertyName("GRAPH_PATH")] public string GRAPH_PATH { get; set; }
        [JsonPropertyName("OFFSET_X_FROM_ANCHOR")] public int OFFSET_X_FROM_ANCHOR { get; set; }
        [JsonPropertyName("OFFSET_Y_FROM_ANCHOR")] public int OFFSET_Y_FROM_ANCHOR { get; set; }
        [JsonPropertyName("GRAPH_SRC_X")] public int GRAPH_SRC_X { get; set; }
        [JsonPropertyName("GRAPH_SRC_Y")] public int GRAPH_SRC_Y { get; set; }
        [JsonPropertyName("GRAPH_DEST_X")] public int GRAPH_DEST_X { get; set; }
        [JsonPropertyName("GRAPH_DEST_Y")] public int GRAPH_DEST_Y { get; set; }
        [JsonPropertyName("INDEX_X")] public int INDEX_X { get; set; }
        [JsonPropertyName("INDEX_Y")] public int INDEX_Y { get; set; }
        [JsonPropertyName("DESCRIPTION")] public string DESCRIPTION { get; set; }
        [JsonPropertyName("ANCHORPOINT")] public string ANCHORPOINT { get; set; }
        [JsonPropertyName("COLLISION")] public string COLLISION { get; set; } = "TUI_CANVAS_COLLISION_NONE";

        [JsonPropertyName("IS_LOOP_EVENT")] public bool IS_LOOP_EVENT { get; set; }
        [JsonPropertyName("IS_SELECTABLE")] public bool IS_SELECTABLE { get; set; }

        // ★ 追加：イベント配列（読み込んだまま保持→保存で復元）
        [JsonPropertyName("EVENTS")] public ObservableCollection<UIEvent> EVENTS { get; set; } = new();

        // ===== エディタ拡張（JSONにも保存される） =====
        [JsonPropertyName("IsVisible")] public bool? IsVisible { get; set; } = true;
        [JsonPropertyName("Rotation")] public double? Rotation { get; set; } = 0.0;
        [JsonPropertyName("Scale")] public double? Scale { get; set; } = 1.0;
        [JsonPropertyName("Opacity")] public double? Opacity { get; set; } = 1.0;
        [JsonPropertyName("ZIndex")] public int? ZIndex { get; set; } = 0;
        [JsonPropertyName("LockAspectRatio")] public bool? LockAspectRatio { get; set; } = true;
        [JsonPropertyName("PivotX")] public double? PivotX { get; set; } = 0.5;
        [JsonPropertyName("PivotY")] public double? PivotY { get; set; } = 0.5;
    }

    // ====== 画面表示用VM（UIItem を参照） ======
    public class UIItemVM : INotifyPropertyChanged
    {
        public UIItem Src { get; }
        private readonly MainWindow _root;

        public UIItemVM(UIItem src, MainWindow root)
        {
            Src = src;
            _root = root;
        }

        private bool _isSelected;
        public bool IsSelected
        {
            get
            {
                return _isSelected;
            }
            set
            {
                if (_isSelected == value)
                {
                    return;
                }
                _isSelected = value;
                OnPropertyChanged();
            }
        }

        public string NAME
        {
            get
            {
                return Src.NAME;
            }
            set
            {
                Src.NAME = value;
                OnPropertyChanged();
            }
        }

        public string GRAPH_PATH
        {
            get
            {
                return Src.GRAPH_PATH;
            }
            set
            {
                Src.GRAPH_PATH = value;
                _imageSource = null;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ImageSource));
            }
        }

        public string ANCHORPOINT
        {
            get
            {
                return Src.ANCHORPOINT;
            }
            set
            {
                Src.ANCHORPOINT = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosX));
                OnPropertyChanged(nameof(PosY));
            }
        }

        // === 衝突（エディタで編集する用の素通し） ===
        public string COLLISION
        {
            get
            {
                return Src.COLLISION;
            }
            set
            {
                if (Src.COLLISION == value)
                {
                    return;
                }
                Src.COLLISION = value;
                OnPropertyChanged();
            }
        }

        public int OFFSET_X_FROM_ANCHOR
        {
            get
            {
                return Src.OFFSET_X_FROM_ANCHOR;
            }
            set
            {
                Src.OFFSET_X_FROM_ANCHOR = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosX));
            }
        }

        public int OFFSET_Y_FROM_ANCHOR
        {
            get
            {
                return Src.OFFSET_Y_FROM_ANCHOR;
            }
            set
            {
                Src.OFFSET_Y_FROM_ANCHOR = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosY));
            }
        }

        public int GRAPH_DEST_X
        {
            get
            {
                return Src.GRAPH_DEST_X;
            }
            set
            {
                Src.GRAPH_DEST_X = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(DisplayWidth));
                OnPropertyChanged(nameof(PosX));
            }
        }

        public int GRAPH_DEST_Y
        {
            get
            {
                return Src.GRAPH_DEST_Y;
            }
            set
            {
                Src.GRAPH_DEST_Y = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(DisplayHeight));
                OnPropertyChanged(nameof(PosY));
            }
        }

        public int GRAPH_SRC_X
        {
            get
            {
                return Src.GRAPH_SRC_X;
            }
            set
            {
                Src.GRAPH_SRC_X = value;
                OnPropertyChanged();
            }
        }

        public int GRAPH_SRC_Y
        {
            get
            {
                return Src.GRAPH_SRC_Y;
            }
            set
            {
                Src.GRAPH_SRC_Y = value;
                OnPropertyChanged();
            }
        }

        public int INDEX_X
        {
            get
            {
                return Src.INDEX_X;
            }
            set
            {
                Src.INDEX_X = value;
                OnPropertyChanged();
            }
        }

        public int INDEX_Y
        {
            get
            {
                return Src.INDEX_Y;
            }
            set
            {
                Src.INDEX_Y = value;
                OnPropertyChanged();
            }
        }

        public string DESCRIPTION
        {
            get
            {
                return Src.DESCRIPTION;
            }
            set
            {
                Src.DESCRIPTION = value;
                OnPropertyChanged();
            }
        }

        public bool IS_SELECTABLE
        {
            get
            {
                return Src.IS_SELECTABLE;
            }
            set
            {
                Src.IS_SELECTABLE = value;
                OnPropertyChanged();
            }
        }

        // === そのまま保存する系 ===
        public bool IsVisible
        {
            get
            {
                return Src.IsVisible ?? true;
            }
            set
            {
                Src.IsVisible = value;
                OnPropertyChanged();
            }
        }

        public double Rotation
        {
            get
            {
                return Src.Rotation ?? 0.0;
            }
            set
            {
                Src.Rotation = value;
                OnPropertyChanged();
            }
        }

        public double Scale
        {
            get
            {
                return Src.Scale ?? 1.0;
            }
            set
            {
                Src.Scale = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(DisplayWidth));
                OnPropertyChanged(nameof(DisplayHeight));
                OnPropertyChanged(nameof(PosX));
                OnPropertyChanged(nameof(PosY));
            }
        }

        public double Opacity
        {
            get
            {
                return Src.Opacity ?? 1.0;
            }
            set
            {
                Src.Opacity = value;
                OnPropertyChanged();
            }
        }

        public int ZIndex
        {
            get
            {
                return Src.ZIndex ?? 0;
            }
            set
            {
                Src.ZIndex = value;
                OnPropertyChanged();
            }
        }

        public bool LockAspectRatio
        {
            get
            {
                return Src.LockAspectRatio ?? true;
            }
            set
            {
                Src.LockAspectRatio = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(DisplayHeight));
            }
        }

        public double PivotX
        {
            get
            {
                return Src.PivotX ?? 0.5;
            }
            set
            {
                Src.PivotX = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PivotPoint));
                OnPropertyChanged(nameof(PosX));
            }
        }

        public double PivotY
        {
            get
            {
                return Src.PivotY ?? 0.5;
            }
            set
            {
                Src.PivotY = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PivotPoint));
                OnPropertyChanged(nameof(PosY));
            }
        }

        // ★ イベント配列（UIで編集しないなら触らずそのまま保存される）
        public ObservableCollection<UIEvent> EVENTS
        {
            get
            {
                return Src.EVENTS;
            }
        }

        public Point PivotPoint
        {
            get
            {
                return new(PivotX, PivotY);
            }
        }

        public double DisplayWidth
        {
            get
            {
                return GRAPH_DEST_X * Scale;
            }
        }

        public double DisplayHeight
        {
            get
            {
                return GRAPH_DEST_Y * Scale;
            }
        }

        public string[] AnchorOptions
        {
            get
            {
                return new[]
                {
                    "TUI_CANVAS_ANCHOR_POINT_TOP_LEFT",
                    "TUI_CANVAS_ANCHOR_POINT_TOP",
                    "TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT",
                    "TUI_CANVAS_ANCHOR_POINT_LEFT",
                    "TUI_CANVAS_ANCHOR_POINT_CENTER",
                    "TUI_CANVAS_ANCHOR_POINT_RIGHT",
                    "TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT",
                    "TUI_CANVAS_ANCHOR_POINT_BOTTOM",
                    "TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT"
                };
            }
        }

        public string[] CollisionOptions
        {
            get
            {
                return new[]
                {
                    "TUI_CANVAS_COLLISION_NONE",
                    "TUI_CANVAS_COLLISION_RECT",
                    "TUI_CANVAS_COLLISION_CIRCLE"
                };
            }
        }

        // 画像（エディタ表示用）
        private string _displayImagePath;
        public string DisplayImagePath
        {
            get
            {
                return _displayImagePath;
            }
            set
            {
                _displayImagePath = value;
                _imageSource = null;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ImageSource));
            }
        }

        private BitmapImage _imageSource;
        public BitmapImage ImageSource
        {
            get
            {
                if (_imageSource != null)
                {
                    return _imageSource;
                }
                var path = !string.IsNullOrWhiteSpace(DisplayImagePath) ? DisplayImagePath : GRAPH_PATH;
                if (string.IsNullOrWhiteSpace(path))
                {
                    return null;
                }

                try
                {
                    var baseDir = AppDomain.CurrentDomain.BaseDirectory;
                    var abs = System.IO.Path.IsPathRooted(path)
                        ? path
                        : System.IO.Path.Combine(baseDir, path.Replace('/', System.IO.Path.DirectorySeparatorChar));

                    var bi = new BitmapImage();
                    bi.BeginInit();
                    bi.UriSource = System.IO.File.Exists(abs)
                        ? new Uri(abs, UriKind.Absolute)
                        : new Uri(path, UriKind.RelativeOrAbsolute);
                    bi.CacheOption = BitmapCacheOption.OnLoad;
                    bi.EndInit();
                    _imageSource = bi;
                }
                catch
                {
                    // 読み込めない場合は null
                }
                return _imageSource;
            }
        }

        // ===== レイアウト =====
        public double PosX
        {
            get
            {
                var (ax, _) = AnchorPointToXY();
                return ax + OFFSET_X_FROM_ANCHOR - DisplayWidth * PivotX;
            }
            set
            {
                var (ax, _) = AnchorPointToXY();
                var px = value + DisplayWidth * PivotX;
                Src.OFFSET_X_FROM_ANCHOR = (int)Math.Round(px - ax);
                OnPropertyChanged();
            }
        }

        public double PosY
        {
            get
            {
                var (_, ay) = AnchorPointToXY();
                return ay + OFFSET_Y_FROM_ANCHOR - DisplayHeight * PivotY;
            }
            set
            {
                var (_, ay) = AnchorPointToXY();
                var py = value + DisplayHeight * PivotY;
                Src.OFFSET_Y_FROM_ANCHOR = (int)Math.Round(py - ay);
                OnPropertyChanged();
            }
        }

        public void SyncOffsetFromPos()
        {
            OnPropertyChanged(nameof(PosX));
            OnPropertyChanged(nameof(PosY));
        }

        public void RefreshLayout()
        {
            OnPropertyChanged(nameof(PosX));
            OnPropertyChanged(nameof(PosY));
        }

        private (double, double) AnchorPointToXY()
        {
            var bx = _root.CanvasLeft;
            var by = _root.CanvasTop;
            var w = _root.CanvasWidth;
            var h = _root.CanvasHeight;

            return ANCHORPOINT switch
            {
                "TUI_CANVAS_ANCHOR_POINT_TOP" => (bx + w / 2.0, by),
                "TUI_CANVAS_ANCHOR_POINT_BOTTOM" => (bx + w / 2.0, by + h),
                "TUI_CANVAS_ANCHOR_POINT_LEFT" => (bx, by + h / 2.0),
                "TUI_CANVAS_ANCHOR_POINT_RIGHT" => (bx + w, by + h / 2.0),
                "TUI_CANVAS_ANCHOR_POINT_CENTER" => (bx + w / 2.0, by + h / 2.0),
                "TUI_CANVAS_ANCHOR_POINT_TOP_LEFT" => (bx, by),
                "TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT" => (bx + w, by),
                "TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT" => (bx, by + h),
                "TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT" => (bx + w, by + h),
                _ => (bx + w / 2.0, by + h / 2.0)
            };
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string n = null)
        {
            PropertyChanged?.Invoke(this, new(n));
        }
    }

}
