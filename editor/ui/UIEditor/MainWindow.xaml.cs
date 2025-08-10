using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace MainCanvasEditor
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private const double LongPressMs = 350.0;

        public CanvasDocument Data { get; set; }
        public ObservableCollection<UIItemVM> UIItems { get; } = new();

        // ★イベントカタログ（プリセット一覧）
        public ObservableCollection<EventPreset> CatalogPresets { get; } = new();

        private UIItemVM _selected;
        public UIItemVM Selected
        {
            get => _selected;
            set { if (_selected == value) return; if (_selected != null) _selected.IsSelected = false; _selected = value; if (_selected != null) _selected.IsSelected = true; OnPropertyChanged(); }
        }

        private bool _showGuides = true;
        public bool ShowGuides { get => _showGuides; set { _showGuides = value; OnPropertyChanged(); } }

        public double CanvasWidth { get => Data?.End?.X - Data?.Begin?.X ?? 1920; set { if (Data?.End != null) { Data.End.X = (int)Math.Round(value + (Data.Begin?.X ?? 0)); OnPropertyChanged(); } } }
        public double CanvasHeight { get => Data?.End?.Y - Data?.Begin?.Y ?? 1080; set { if (Data?.End != null) { Data.End.Y = (int)Math.Round(value + (Data.Begin?.Y ?? 0)); OnPropertyChanged(); } } }

        public string CurrentPath { get; private set; }

        public MainWindow()
        {
            DataContext = this;
            InitializeComponent();

            // ★イベントカタログの読込（なければ既定を生成）
            var catalogPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Events", "EventsCatalog.json");
            EventCatalog.EnsureFile(catalogPath);
            var catalog = EventCatalog.LoadFrom(catalogPath);
            CatalogPresets.Clear();
            foreach (var p in catalog.Presets.OrderBy(p => p.Category).ThenBy(p => p.DisplayName))
                CatalogPresets.Add(p);

            Data = new CanvasDocument
            {
                CanvasName = "MainCanvas",
                Begin = new Point2D { X = 0, Y = 0 },
                End = new Point2D { X = 1920, Y = 1080 },
                IsFitScreen = true,
                IsDefaultActivate = true
            };

            // サンプル1件
            Data.UIList.Add(new UIItem
            {
                NAME = "Sample",
                GRAPH_PATH = "Images/Sample.png",
                OFFSET_X_FROM_ANCHOR = 0,
                OFFSET_Y_FROM_ANCHOR = 0,
                GRAPH_SRC_X = 0,
                GRAPH_SRC_Y = 0,
                GRAPH_DEST_X = 256,
                GRAPH_DEST_Y = 256,
                INDEX_X = 0,
                INDEX_Y = 0,
                DESCRIPTION = "Sample",
                ANCHORPOINT = "TUI_CANVAS_ANCHOR_POINT_CENTER",
                COLLISION = "TUI_COLLISION_MODE_RECT",
                IS_LOOP_EVENT = false,
                IS_SELECTABLE = true,
                IsVisible = true,
                Rotation = 0,
                Scale = 1.0,
                Opacity = 1.0,
                ZIndex = 0,
                LockAspectRatio = true,
                PivotX = 0.5,
                PivotY = 0.5,
                EVENTS = new ObservableCollection<UIEvent>
                {
                    new UIEvent{ Timing="TUI_EVENT_TRIGGER_TIMING_ENTER", Event="GameStart", Description="Start!", Argument = JsonDocument.Parse("{\"SceneName\":\"Main\"}").RootElement.Clone() }
                }
            });

            RebuildUIItems();
            Selected = UIItems.FirstOrDefault();
        }

        private void RebuildUIItems()
        {
            UIItems.Clear();
            if (Data?.UIList == null) return;
            foreach (var u in Data.UIList) UIItems.Add(new UIItemVM(u, this));
        }

        // ===== ファイル =====
        private void OnOpenJsonClick(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog { Filter = "JSON file|*.json" };
            if (dlg.ShowDialog() == true)
            {
                try
                {
                    var json = File.ReadAllText(dlg.FileName);
                    var opt = JsonOptions();
                    var doc = JsonSerializer.Deserialize<CanvasDocument>(json, opt);
                    if (doc != null)
                    {
                        foreach (var ui in doc.UIList)
                        {
                            ui.IsVisible ??= true; ui.Rotation ??= 0; ui.Scale ??= 1.0; ui.Opacity ??= 1.0;
                            ui.ZIndex ??= 0; ui.LockAspectRatio ??= true; ui.PivotX ??= 0.5; ui.PivotY ??= 0.5;
                        }
                        Data = doc; CurrentPath = dlg.FileName;

                        OnPropertyChanged(nameof(Data));
                        OnPropertyChanged(nameof(CanvasWidth));
                        OnPropertyChanged(nameof(CanvasHeight));
                        RebuildUIItems();
                        Selected = UIItems.FirstOrDefault();
                    }
                }
                catch (Exception ex) { MessageBox.Show($"読み込みに失敗: {ex.Message}"); }
            }
        }

        private void OnSaveJsonClick(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(CurrentPath))
            {
                var dlg = new Microsoft.Win32.SaveFileDialog
                {
                    Filter = "JSON file|*.json",
                    FileName = $"{Data?.CanvasName ?? "MainCanvas"}.json"
                };
                if (dlg.ShowDialog() == true) CurrentPath = dlg.FileName; else return;
            }

            try
            {
                // ★保存前に Argument をできるだけ JSON 化（文字列ならパースを試す）
                foreach (var ui in Data.UIList)
                {
                    foreach (var ev in ui.EVENTS)
                    {
                        if (ev.Argument is string s)
                        {
                            try { ev.Argument = JsonDocument.Parse(string.IsNullOrWhiteSpace(s) ? "{}" : s).RootElement.Clone(); }
                            catch { /* 文字列のままでもC++側で未使用ならOK */ }
                        }
                    }
                }

                var opt = JsonOptions();
                var json = JsonSerializer.Serialize(Data, opt);
                File.WriteAllText(CurrentPath, json);
            }
            catch (Exception ex) { MessageBox.Show($"保存に失敗: {ex.Message}"); }
        }

        private void OnExitClick(object sender, RoutedEventArgs e) => Close();

        private static JsonSerializerOptions JsonOptions()
        {
            var opt = new JsonSerializerOptions
            {
                ReadCommentHandling = JsonCommentHandling.Skip,
                AllowTrailingCommas = true,
                WriteIndented = true
            };
            opt.Converters.Add(new JsonStringEnumConverter());
            return opt;
        }

        // ===== UI長押しドラッグ =====
        private UIItemVM _pressedItem;
        private System.Windows.Point _pressPos;
        private readonly System.Diagnostics.Stopwatch _pressWatch = new();
        private bool _isDragging;

        private void UIElement_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Viewport.Focus();
            if (sender is Border b && b.Tag is UIItemVM vm)
            {
                Selected = vm;
                _pressedItem = vm;
                _pressPos = e.GetPosition(DesignCanvas);
                _pressWatch.Restart();
                _isDragging = false;
                b.CaptureMouse();
            }
        }

        private void UIElement_MouseMove(object sender, MouseEventArgs e)
        {
            if (_pressedItem == null) return;

            var nowPos = e.GetPosition(DesignCanvas);
            if (!_isDragging)
            {
                if (_pressWatch.Elapsed.TotalMilliseconds >= LongPressMs) _isDragging = true;
                else return;
            }

            var dx = nowPos.X - _pressPos.X;
            var dy = nowPos.Y - _pressPos.Y;

            _pressedItem.PosX += dx;
            _pressedItem.PosY += dy;
            _pressedItem.SyncOffsetFromPos();

            _pressPos = nowPos;
        }

        private void UIElement_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (sender is Border b) b.ReleaseMouseCapture();
            _pressedItem = null; _pressWatch.Reset(); _isDragging = false;
        }

        // ===== Unity風ビューポート =====
        private bool _isPanningMMB;
        private bool _isAltRightDrag;
        private Point _lastMouseScreen;
        private bool _isAltHeld;

        private const double ZoomWheelFactor = 1.2;
        private const double ZoomDragSensitivity = 0.005;
        private const double RotateDragSensitivity = 0.25;

        private void Viewport_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            var pos = e.GetPosition(World);
            var factor = e.Delta > 0 ? ZoomWheelFactor : 1.0 / ZoomWheelFactor;
            ZoomAt(pos, factor);
        }

        private void Viewport_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Viewport.Focus();
            if (e.ChangedButton == MouseButton.Middle)
            {
                _isPanningMMB = true;
                _lastMouseScreen = e.GetPosition(Viewport);
                Mouse.Capture((IInputElement)sender);
            }
            else if (e.ChangedButton == MouseButton.Right && _isAltHeld)
            {
                _isAltRightDrag = true;
                _lastMouseScreen = e.GetPosition(Viewport);
                Mouse.Capture((IInputElement)sender);
            }
        }

        private void Viewport_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Middle) _isPanningMMB = false;
            if (e.ChangedButton == MouseButton.Right) _isAltRightDrag = false;
            if (!_isPanningMMB && !_isAltRightDrag) Mouse.Capture(null);
        }

        private void Viewport_MouseMove(object sender, MouseEventArgs e)
        {
            var cur = e.GetPosition(Viewport);

            if (_isPanningMMB)
            {
                var delta = cur - _lastMouseScreen;
                PanTransform.X += delta.X;
                PanTransform.Y += delta.Y;
                _lastMouseScreen = cur;
                return;
            }

            if (_isAltRightDrag)
            {
                var delta = cur - _lastMouseScreen;

                var zoomFactor = Math.Exp(-delta.Y * ZoomDragSensitivity);
                var pivot = e.GetPosition(World);
                ZoomAt(pivot, zoomFactor);

                if (Selected != null) Selected.Rotation += delta.X * RotateDragSensitivity;

                _lastMouseScreen = cur;
            }
        }

        private void Viewport_MouseLeave(object sender, MouseEventArgs e)
        {
            _isPanningMMB = false;
            _isAltRightDrag = false;
            Mouse.Capture(null);
        }

        private void Viewport_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.LeftAlt || e.Key == Key.RightAlt) _isAltHeld = true;
        }

        private void Viewport_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.LeftAlt || e.Key == Key.RightAlt) _isAltHeld = false;
        }

        private void ZoomAt(Point contentPivot, double factor)
        {
            var oldScale = ZoomTransform.ScaleX;
            var newScale = Math.Clamp(oldScale * factor, 0.05, 20.0);
            factor = newScale / oldScale;

            PanTransform.X = PanTransform.X - (contentPivot.X * (factor - 1.0) * oldScale);
            PanTransform.Y = PanTransform.Y - (contentPivot.Y * (factor - 1.0) * oldScale);

            ZoomTransform.ScaleX = newScale;
            ZoomTransform.ScaleY = newScale;
        }

        // ===== 表示用画像 参照/クリア =====
        private void OnBrowseDisplayImageClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null) return;

            var dlg = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Image files|*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.webp|All files|*.*",
                InitialDirectory = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Images")
            };
            if (dlg.ShowDialog() == true)
            {
                var baseDir = AppDomain.CurrentDomain.BaseDirectory.TrimEnd(System.IO.Path.DirectorySeparatorChar, System.IO.Path.AltDirectorySeparatorChar);
                var chosen = dlg.FileName;
                string rel;
                if (chosen.StartsWith(baseDir, StringComparison.OrdinalIgnoreCase))
                {
                    rel = chosen.Substring(baseDir.Length).TrimStart(System.IO.Path.DirectorySeparatorChar, System.IO.Path.AltDirectorySeparatorChar);
                }
                else
                {
                    rel = chosen;
                }
                Selected.DisplayImagePath = rel.Replace(System.IO.Path.DirectorySeparatorChar, '/');
            }
        }

        private void OnClearDisplayImageClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null) return;
            Selected.DisplayImagePath = null;
        }

        // ===== イベント追加・削除 =====
        private void OnAddEventClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null) return;
            Selected.Events.Add(new UIEvent
            {
                Timing = "TUI_EVENT_TRIGGER_TIMING_ENTER",
                Event = "",
                Description = "",
                Argument = "{}"
            });
        }

        private void OnRemoveEventClick(object sender, RoutedEventArgs e)
        {
            if (Selected == null) return;
            if (sender is Button btn && btn.DataContext is UIEvent ev)
            {
                Selected.Events.Remove(ev);
            }
        }

        // ★プリセット選択時の自動補完
        private void OnEventPresetChanged(object sender, SelectionChangedEventArgs e)
        {
            if (Selected == null) return;
            if (sender is not ComboBox cb) return;
            if (cb.SelectedItem is not EventPreset preset) return;
            if (cb.DataContext is not UIEvent ev) return;

            // Event は SelectedValuePath=Key によりバインド済み
            // 未入力項目をプリセットで補完
            if (string.IsNullOrWhiteSpace(ev.Description))
                ev.Description = preset.Description ?? "";

            if (string.IsNullOrWhiteSpace(ev.Timing) && !string.IsNullOrWhiteSpace(preset.DefaultTiming))
                ev.Timing = preset.DefaultTiming;

            if (ev.Argument == null || (ev.Argument is string s && string.IsNullOrWhiteSpace(s)))
                ev.Argument = ToJsonElementOrString(preset.DefaultArgument);
        }

        private static object ToJsonElementOrString(object? any)
        {
            if (any == null) return "{}";
            try
            {
                var json = JsonSerializer.Serialize(any);
                using var doc = JsonDocument.Parse(json);
                return doc.RootElement.Clone();
            }
            catch
            {
                return any.ToString() ?? "{}";
            }
        }

        // INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string n = null) => PropertyChanged?.Invoke(this, new(n));
    }
}
