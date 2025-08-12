using Microsoft.Win32;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Text.Unicode;
using System.Windows;
using System.Windows.Input;

namespace MainCanvasEditor
{

    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        // ====== バインド用 ======
        public CanvasDocument Data
        {
            get
            {
                return _data;
            }
            set
            {
                _data = value;
                OnPropertyChanged();
                MaybeFit();
            }
        }
        private CanvasDocument _data;

        public ObservableCollection<UIItemVM> UIItems
        {
            get
            {
                return _uiItems;
            }
            set
            {
                _uiItems = value;
                OnPropertyChanged();
            }
        }
        private ObservableCollection<UIItemVM> _uiItems = new();

        public UIItemVM Selected
        {
            get
            {
                return _selected;
            }
            set
            {
                if (_selected == value)
                {
                    return;
                }
                if (_selected != null)
                {
                    _selected.IsSelected = false;
                }
                _selected = value;
                if (_selected != null)
                {
                    _selected.IsSelected = true;
                }
                OnPropertyChanged();
            }
        }
        private UIItemVM _selected;

        private bool _showGuides;
        public bool ShowGuides
        {
            get
            {
                return _showGuides;
            }
            set
            {
                _showGuides = value;
                OnPropertyChanged();
            }
        }

        private bool _autoFitOnResize = true;
        public bool AutoFitOnResize
        {
            get
            {
                return _autoFitOnResize;
            }
            set
            {
                _autoFitOnResize = value;
                OnPropertyChanged();
            }
        }

        public int CanvasLeft
        {
            get
            {
                return Data?.Begin?.X ?? 0;
            }
            set
            {
                if (Data == null)
                {
                    return;
                }
                Data.Begin ??= new Point2D();
                Data.Begin.X = value;
                OnPropertyChanged();
                RefreshAllItemPositions();
                MaybeFit();
            }
        }

        public int CanvasTop
        {
            get
            {
                return Data?.Begin?.Y ?? 0;
            }
            set
            {
                if (Data == null)
                {
                    return;
                }
                Data.Begin ??= new Point2D();
                Data.Begin.Y = value;
                OnPropertyChanged();
                RefreshAllItemPositions();
                MaybeFit();
            }
        }

        public int CanvasWidth
        {
            get
            {
                if (Data == null || Data.Begin == null || Data.End == null)
                {
                    return 0;
                }
                return Data.End.X - Data.Begin.X;
            }
            set
            {
                if (Data == null)
                {
                    return;
                }
                Data.Begin ??= new Point2D();
                Data.End ??= new Point2D();
                Data.End.X = Data.Begin.X + value;
                OnPropertyChanged();
                RefreshAllItemPositions();
                MaybeFit();
            }
        }

        public int CanvasHeight
        {
            get
            {
                if (Data == null || Data.Begin == null || Data.End == null)
                {
                    return 0;
                }
                return Data.End.Y - Data.Begin.Y;
            }
            set
            {
                if (Data == null)
                {
                    return;
                }
                Data.Begin ??= new Point2D();
                Data.End ??= new Point2D();
                Data.End.Y = Data.Begin.Y + value;
                OnPropertyChanged();
                RefreshAllItemPositions();
                MaybeFit();
            }
        }

        // ====== 状態 ======
        public string CurrentPath
        {
            get
            {
                return _currentPath;
            }
            set
            {
                _currentPath = value;
                OnPropertyChanged();
            }
        }
        private string _currentPath = "";

        private bool _isDragging = false;
        private UIItemVM _pressedItem = null;
        private Point _pressPosWorld;

        private bool _isPanningMMB = false;
        private Point _lastMouseScreen;

        private bool _isAltRightDrag = false;

        public double ZoomWheelFactor { get; set; } = 1.1;
        public double ZoomDragSensitivity { get; set; } = 0.005;
        public double RotateDragSensitivity { get; set; } = 0.3;
        public int LongPressMs { get; set; } = 150;

        public event PropertyChangedEventHandler PropertyChanged;

        // ====== ctor ======
        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            Data = new CanvasDocument
            {
                CanvasName = "MainCanvas",
                Begin = new Point2D { X = 0, Y = 0 },
                End = new Point2D { X = 1920, Y = 1080 },
                IsDefaultActivate = true
            };

            Loaded += (s, e) => FitCanvasToView();
            RebuildUIItems();
        }

        // ====== 便利関数 ======
        private void OnPropertyChanged([CallerMemberName] string name = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        private JsonSerializerOptions JsonOptions()
        {
            return new JsonSerializerOptions
            {
                WriteIndented = true,
                Encoder = JavaScriptEncoder.Create(UnicodeRanges.All)
            };
        }

        private void RebuildUIItems()
        {
            UIItems.Clear();
            if (Data?.UIList != null)
            {
                foreach (var it in Data.UIList)
                {
                    UIItems.Add(new UIItemVM(it, this));
                }
            }
            if (UIItems.Count > 0)
            {
                Selected = UIItems[0];
            }
            RefreshAllItemPositions();
        }

        private void RefreshAllItemPositions()
        {
            foreach (var vm in UIItems)
            {
                vm.RefreshLayout();
            }
        }

        private string MakeUniqueName(string baseName)
        {
            var name = baseName;
            int i = 1;
            while (true)
            {
                bool exists = false;
                foreach (var it in UIItems)
                {
                    if (string.Equals(it.NAME, name, StringComparison.OrdinalIgnoreCase))
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    return name;
                }
                name = $"{baseName}_{++i}";
            }
        }

        // ====== カメラ系 ======
        private Point ScreenToWorld(Point screen)
        {
            var tg = World.RenderTransform.Value;
            tg.Invert();
            return tg.Transform(screen);
        }

        private void ZoomAt(Point screenPos, double factor)
        {
            var wBefore = ScreenToWorld(screenPos);

            var oldX = ZoomTransform.ScaleX;
            var oldY = ZoomTransform.ScaleY;
            var newX = Math.Clamp(oldX * factor, 0.05, 50);
            var newY = Math.Clamp(oldY * factor, 0.05, 50);
            ZoomTransform.ScaleX = newX;
            ZoomTransform.ScaleY = newY;

            PanTransform.X += (oldX - newX) * wBefore.X;
            PanTransform.Y += (oldY - newY) * wBefore.Y;
        }

        private void FitCanvasToView()
        {
            if (ViewportHost == null)
            {
                return;
            }

            // 左ペインの実サイズ（枠線も含む）を基準に計算
            var viewW = Math.Max(1.0, ViewportHost.ActualWidth);
            var viewH = Math.Max(1.0, ViewportHost.ActualHeight);
            var cw = Math.Max(1.0, CanvasWidth);
            var ch = Math.Max(1.0, CanvasHeight);

            // 余白を少し確保（左右上下8pxずつ）
            const double margin = 16.0;
            var scaleW = (viewW - margin) / cw;
            var scaleH = (viewH - margin) / ch;
            var scale = Math.Max(0.01, Math.Min(scaleW, scaleH));

            ZoomTransform.ScaleX = ZoomTransform.ScaleY = scale;

            PanTransform.X = -CanvasLeft * scale + (viewW - cw * scale) / 2.0;
            PanTransform.Y = -CanvasTop * scale + (viewH - ch * scale) / 2.0;
        }

        private void MaybeFit()
        {
            if (AutoFitOnResize)
            {
                FitCanvasToView();
            }
        }

        // ====== メニュー ======
        private void OnOpenJsonClick(object sender, RoutedEventArgs e)
        {
            var dlg = new OpenFileDialog { Filter = "JSON (*.json)|*.json|All Files (*.*)|*.*" };
            if (dlg.ShowDialog() == true)
            {
                var json = File.ReadAllText(dlg.FileName);
                var doc = JsonSerializer.Deserialize<CanvasDocument>(json, JsonOptions());
                if (doc != null)
                {
                    Data = doc;
                    CurrentPath = System.IO.Path.GetDirectoryName(dlg.FileName) ?? "";
                    RebuildUIItems();
                }
            }
        }

        private void OnSaveJsonClick(object sender, RoutedEventArgs e)
        {
            var dlg = new SaveFileDialog
            {
                Filter = "JSON (*.json)|*.json|All Files (*.*)|*.*",
                FileName = (Data?.CanvasName ?? "MainCanvas") + ".json"
            };
            if (dlg.ShowDialog() == true)
            {
                var json = JsonSerializer.Serialize(Data, JsonOptions());
                File.WriteAllText(dlg.FileName, json);
                CurrentPath = System.IO.Path.GetDirectoryName(dlg.FileName) ?? "";
            }
        }

        private void OnExitClick(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void OnFitClick(object sender, RoutedEventArgs e)
        {
            FitCanvasToView();
        }

        // ====== 編集操作 ======
        private void OnAddUIClick(object sender, RoutedEventArgs e)
        {
            if (Data == null)
            {
                return;
            }

            var item = new UIItem
            {
                NAME = MakeUniqueName("NewUI"),
                GRAPH_PATH = "",
                ANCHORPOINT = "TUI_CANVAS_ANCHOR_POINT_CENTER",
                OFFSET_X_FROM_ANCHOR = 0,
                OFFSET_Y_FROM_ANCHOR = 0,
                GRAPH_SRC_X = 0,
                GRAPH_SRC_Y = 0,
                GRAPH_DEST_X = 64,
                GRAPH_DEST_Y = 64,
                INDEX_X = 0,
                INDEX_Y = 0,
                DESCRIPTION = "",
                IS_SELECTABLE = true,
                IsVisible = true,
                Rotation = 0.0,
                Scale = 1.0,
                Opacity = 1.0,
                ZIndex = 0,
                LockAspectRatio = true,
                PivotX = 0.5,
                PivotY = 0.5
            };

            Data.UIList.Add(item);
            var vm = new UIItemVM(item, this);
            UIItems.Add(vm);
            Selected = vm;
        }

        private void OnDuplicateUIClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null || Data == null)
            {
                return;
            }

            var src = Selected.Src;
            var clone = new UIItem
            {
                NAME = MakeUniqueName(src.NAME + "_Copy"),
                GRAPH_PATH = src.GRAPH_PATH,
                ANCHORPOINT = src.ANCHORPOINT,
                OFFSET_X_FROM_ANCHOR = src.OFFSET_X_FROM_ANCHOR + 10,
                OFFSET_Y_FROM_ANCHOR = src.OFFSET_Y_FROM_ANCHOR + 10,
                GRAPH_SRC_X = src.GRAPH_SRC_X,
                GRAPH_SRC_Y = src.GRAPH_SRC_Y,
                GRAPH_DEST_X = src.GRAPH_DEST_X,
                GRAPH_DEST_Y = src.GRAPH_DEST_Y,
                INDEX_X = src.INDEX_X,
                INDEX_Y = src.INDEX_Y,
                DESCRIPTION = src.DESCRIPTION,
                IS_SELECTABLE = src.IS_SELECTABLE,
                IsVisible = src.IsVisible,
                Rotation = src.Rotation,
                Scale = src.Scale,
                Opacity = src.Opacity,
                ZIndex = (src.ZIndex ?? 0) + 1,
                LockAspectRatio = src.LockAspectRatio,
                PivotX = src.PivotX,
                PivotY = src.PivotY
            };

            Data.UIList.Add(clone);
            var vm = new UIItemVM(clone, this);
            UIItems.Add(vm);
            Selected = vm;
        }

        private void OnDeleteUIClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null || Data == null)
            {
                return;
            }

            var idx = UIItems.IndexOf(Selected);
            Data.UIList.Remove(Selected.Src);
            UIItems.Remove(Selected);

            if (UIItems.Count > 0)
            {
                Selected = UIItems[Math.Clamp(idx - 1, 0, UIItems.Count - 1)];
            }
            else
            {
                Selected = null;
            }
        }

        private void OnBrowseDisplayImageClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null)
            {
                return;
            }
            var dlg = new OpenFileDialog
            {
                Filter = "画像 (*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp|All Files (*.*)|*.*",
                InitialDirectory = string.IsNullOrEmpty(CurrentPath) ? null : CurrentPath
            };
            if (dlg.ShowDialog() == true)
            {
                Selected.DisplayImagePath = dlg.FileName;
            }
        }

        // ====== ビューポート入力 ======
        private void ViewportHost_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            MaybeFit();
        }

        private void Viewport_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Viewport.Focus();

            if (e.ChangedButton == MouseButton.Left &&
                !(Keyboard.IsKeyDown(Key.LeftAlt) || Keyboard.IsKeyDown(Key.RightAlt)))
            {
                Selected = null;
            }

            if (e.ChangedButton == MouseButton.Middle)
            {
                _isPanningMMB = true;
                _lastMouseScreen = e.GetPosition(Viewport);
                Mouse.Capture(Viewport);
                return;
            }

            if ((Keyboard.IsKeyDown(Key.LeftAlt) || Keyboard.IsKeyDown(Key.RightAlt)) &&
                e.ChangedButton == MouseButton.Right)
            {
                _isAltRightDrag = true;
                _lastMouseScreen = e.GetPosition(Viewport);
                Mouse.Capture(Viewport);
                return;
            }
        }

        private void Viewport_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                if (_isDragging)
                {
                    _isDragging = false;
                    _pressedItem = null;
                    Mouse.Capture(null);
                    e.Handled = true;
                }
            }

            if (e.ChangedButton == MouseButton.Middle)
            {
                _isPanningMMB = false;
                Mouse.Capture(null);
            }
            if (e.ChangedButton == MouseButton.Right)
            {
                _isAltRightDrag = false;
                Mouse.Capture(null);
            }
        }

        private void Viewport_MouseMove(object sender, MouseEventArgs e)
        {
            var pos = e.GetPosition(Viewport);

            if (_isPanningMMB)
            {
                var d = pos - _lastMouseScreen;
                PanTransform.X += d.X;
                PanTransform.Y += d.Y;
                _lastMouseScreen = pos;
                return;
            }

            if (_isAltRightDrag)
            {
                var d = pos - _lastMouseScreen;

                if (Selected != null)
                {
                    Selected.Rotation += d.X * RotateDragSensitivity;
                }

                var factor = Math.Pow(1.0 + ZoomDragSensitivity, -d.Y);
                ZoomAt(pos, factor);

                _lastMouseScreen = pos;
                return;
            }

            if (_isDragging && _pressedItem != null && e.LeftButton == MouseButtonState.Pressed)
            {
                var world = ScreenToWorld(pos);
                var delta = world - _pressPosWorld;

                _pressedItem.PosX += delta.X;
                _pressedItem.PosY += delta.Y;

                _pressPosWorld = world;
            }
        }

        private void Viewport_MouseLeave(object sender, MouseEventArgs e)
        {
            _isPanningMMB = false;
            _isAltRightDrag = false;
            if (_isDragging)
            {
                _isDragging = false;
                _pressedItem = null;
                Mouse.Capture(null);
            }
        }

        private void Viewport_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            var factor = e.Delta > 0 ? ZoomWheelFactor : (1.0 / ZoomWheelFactor);
            ZoomAt(e.GetPosition(Viewport), factor);
        }

        private void Viewport_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape && _isDragging)
            {
                _isDragging = false;
                _pressedItem = null;
                Mouse.Capture(null);
                e.Handled = true;
            }
            if (e.Key == Key.F)
            {
                FitCanvasToView();
                e.Handled = true;
            }
        }

        private void Viewport_KeyUp(object sender, KeyEventArgs e)
        {
        }

        // ====== UI要素のクリック系 ======
        private void UIElement_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (sender is FrameworkElement fe && fe.Tag is UIItemVM vm)
            {
                Selected = vm;
                _pressedItem = vm;
                _pressPosWorld = ScreenToWorld(e.GetPosition(Viewport));
                _isDragging = true;
                Mouse.Capture(Viewport);
                e.Handled = true;
            }
        }

        private void UIElement_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (_isDragging)
            {
                _isDragging = false;
                _pressedItem = null;
                Mouse.Capture(null);
                e.Handled = true;
            }
        }

        private void UIElement_MouseMove(object sender, MouseEventArgs e)
        {
        }
    }

}
