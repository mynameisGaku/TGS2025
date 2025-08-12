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
        protected void OnPropertyChanged(string n) => PropertyChanged?.Invoke(this, new(n));
    }

    public class Point2D
    {
        [JsonPropertyName("X")] public int X { get; set; }
        [JsonPropertyName("Y")] public int Y { get; set; }
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
        [JsonPropertyName("COLLISION")] public string COLLISION { get; set; }
        [JsonPropertyName("IS_LOOP_EVENT")] public bool IS_LOOP_EVENT { get; set; }
        [JsonPropertyName("IS_SELECTABLE")] public bool IS_SELECTABLE { get; set; }

        // エディタ拡張（JSONにも保存）
        [JsonPropertyName("IsVisible")] public bool? IsVisible { get; set; } = true;
        [JsonPropertyName("Rotation")] public double? Rotation { get; set; } = 0.0;
        [JsonPropertyName("Scale")] public double? Scale { get; set; } = 1.0;
        [JsonPropertyName("Opacity")] public double? Opacity { get; set; } = 1.0;
        [JsonPropertyName("ZIndex")] public int? ZIndex { get; set; } = 0;
        [JsonPropertyName("LockAspectRatio")] public bool? LockAspectRatio { get; set; } = true;
        [JsonPropertyName("PivotX")] public double? PivotX { get; set; } = 0.5;
        [JsonPropertyName("PivotY")] public double? PivotY { get; set; } = 0.5;
    }

    // ====== 画面表示用VM（JSONは UIItem を参照） ======
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
            get => _isSelected;
            set
            {
                if (_isSelected == value) return;
                _isSelected = value;
                OnPropertyChanged();
            }
        }

        public string NAME
        {
            get => Src.NAME;
            set
            {
                Src.NAME = value;
                OnPropertyChanged();
            }
        }

        public string GRAPH_PATH
        {
            get => Src.GRAPH_PATH;
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
            get => Src.ANCHORPOINT;
            set
            {
                Src.ANCHORPOINT = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosX));
                OnPropertyChanged(nameof(PosY));
            }
        }

        public int OFFSET_X_FROM_ANCHOR
        {
            get => Src.OFFSET_X_FROM_ANCHOR;
            set
            {
                Src.OFFSET_X_FROM_ANCHOR = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosX));
            }
        }

        public int OFFSET_Y_FROM_ANCHOR
        {
            get => Src.OFFSET_Y_FROM_ANCHOR;
            set
            {
                Src.OFFSET_Y_FROM_ANCHOR = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(PosY));
            }
        }

        public int GRAPH_DEST_X
        {
            get => Src.GRAPH_DEST_X;
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
            get => Src.GRAPH_DEST_Y;
            set
            {
                Src.GRAPH_DEST_Y = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(DisplayHeight));
                OnPropertyChanged(nameof(PosY));
            }
        }

        public int GRAPH_SRC_X { get => Src.GRAPH_SRC_X; set { Src.GRAPH_SRC_X = value; OnPropertyChanged(); } }
        public int GRAPH_SRC_Y { get => Src.GRAPH_SRC_Y; set { Src.GRAPH_SRC_Y = value; OnPropertyChanged(); } }
        public int INDEX_X { get => Src.INDEX_X; set { Src.INDEX_X = value; OnPropertyChanged(); } }
        public int INDEX_Y { get => Src.INDEX_Y; set { Src.INDEX_Y = value; OnPropertyChanged(); } }
        public string DESCRIPTION { get => Src.DESCRIPTION; set { Src.DESCRIPTION = value; OnPropertyChanged(); } }
        public bool IS_SELECTABLE { get => Src.IS_SELECTABLE; set { Src.IS_SELECTABLE = value; OnPropertyChanged(); } }
        public bool IsVisible { get => Src.IsVisible ?? true; set { Src.IsVisible = value; OnPropertyChanged(); } }
        public double Rotation { get => Src.Rotation ?? 0.0; set { Src.Rotation = value; OnPropertyChanged(); } }
        public double Scale { get => Src.Scale ?? 1.0; set { Src.Scale = value; OnPropertyChanged(); OnPropertyChanged(nameof(DisplayWidth)); OnPropertyChanged(nameof(DisplayHeight)); OnPropertyChanged(nameof(PosX)); OnPropertyChanged(nameof(PosY)); } }
        public double Opacity { get => Src.Opacity ?? 1.0; set { Src.Opacity = value; OnPropertyChanged(); } }
        public int ZIndex { get => Src.ZIndex ?? 0; set { Src.ZIndex = value; OnPropertyChanged(); } }
        public bool LockAspectRatio { get => Src.LockAspectRatio ?? true; set { Src.LockAspectRatio = value; OnPropertyChanged(); OnPropertyChanged(nameof(DisplayHeight)); } }
        public double PivotX { get => Src.PivotX ?? 0.5; set { Src.PivotX = value; OnPropertyChanged(); OnPropertyChanged(nameof(PivotPoint)); OnPropertyChanged(nameof(PosX)); } }
        public double PivotY { get => Src.PivotY ?? 0.5; set { Src.PivotY = value; OnPropertyChanged(); OnPropertyChanged(nameof(PivotPoint)); OnPropertyChanged(nameof(PosY)); } }
        public Point PivotPoint => new(PivotX, PivotY);

        public double DisplayWidth => GRAPH_DEST_X * Scale;
        public double DisplayHeight => GRAPH_DEST_Y * Scale;

        public string[] AnchorOptions => new[]
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

        // エディタ専用の表示画像（JSON非保存）：指定がなければ GRAPH_PATH を使う
        private string _displayImagePath;
        public string DisplayImagePath
        {
            get => _displayImagePath;
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
                if (_imageSource != null) return _imageSource;
                var path = !string.IsNullOrWhiteSpace(DisplayImagePath) ? DisplayImagePath : GRAPH_PATH;
                if (string.IsNullOrWhiteSpace(path)) return null;

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

        // ===== レイアウト（Begin/End を反映した Anchor 計算） =====
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
