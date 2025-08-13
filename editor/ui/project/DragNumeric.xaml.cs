using System;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace MainCanvasEditor
{

    public partial class DragNumeric : UserControl, INotifyPropertyChanged
    {
        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register(nameof(Value), typeof(double), typeof(DragNumeric),
                new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        public static readonly DependencyProperty StepProperty =
            DependencyProperty.Register(nameof(Step), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(1.0));

        public static readonly DependencyProperty MinProperty =
            DependencyProperty.Register(nameof(Min), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(double.NegativeInfinity));

        public static readonly DependencyProperty MaxProperty =
            DependencyProperty.Register(nameof(Max), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(double.PositiveInfinity));

        public static readonly DependencyProperty FineFactorProperty =
            DependencyProperty.Register(nameof(FineFactor), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(0.1)); // Alt で微調整

        public static readonly DependencyProperty CoarseFactorProperty =
            DependencyProperty.Register(nameof(CoarseFactor), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(10.0)); // Shift で粗く

        public static readonly DependencyProperty DefaultValueProperty =
            DependencyProperty.Register(nameof(DefaultValue), typeof(double), typeof(DragNumeric),
                new PropertyMetadata(double.NaN));

        public double Value
        {
            get => (double)GetValue(ValueProperty);
            set => SetValue(ValueProperty, Clamp(value));
        }
        public double Step { get => (double)GetValue(StepProperty); set => SetValue(StepProperty, value); }
        public double Min { get => (double)GetValue(MinProperty); set => SetValue(MinProperty, value); }
        public double Max { get => (double)GetValue(MaxProperty); set => SetValue(MaxProperty, value); }
        public double FineFactor { get => (double)GetValue(FineFactorProperty); set => SetValue(FineFactorProperty, value); }
        public double CoarseFactor { get => (double)GetValue(CoarseFactorProperty); set => SetValue(CoarseFactorProperty, value); }
        public double DefaultValue { get => (double)GetValue(DefaultValueProperty); set => SetValue(DefaultValueProperty, value); }

        public DragNumeric()
        {
            InitializeComponent();
            DataContext = this;

            // テキスト確定
            Box.LostFocus += (_, __) => ParseText();
            Box.KeyDown += (s, e) => { if (e.Key == Key.Enter) { ParseText(); Keyboard.ClearFocus(); } };

            // ===== ドラッグ（ここでダブルクリックも判定） =====
            DragLayer.PreviewMouseLeftButtonDown += (s, e) =>
            {
                if (Box.IsKeyboardFocusWithin) return; // 入力中はドラッグ無効
                if (e.ClickCount == 2)                 // ダブルクリックでデフォルト値
                {
                    if (!double.IsNaN(DefaultValue)) Value = DefaultValue;
                    e.Handled = true;
                    return;
                }
                _dragStartPos = e.GetPosition(this);
                _dragStartValue = Value;
                Mouse.Capture(DragLayer);
                Cursor = Cursors.SizeWE;
            };

            DragLayer.PreviewMouseMove += (s, e) =>
            {
                if (_dragStartPos is null || Mouse.Captured != DragLayer) return;
                var pos = e.GetPosition(this);
                var dx = pos.X - _dragStartPos.Value.X;
                var mul = GetMul();
                var delta = dx * Step * mul * 0.05; // 1px = 0.05 * Step
                Value = _dragStartValue + delta;
            };

            DragLayer.PreviewMouseLeftButtonUp += (s, e) =>
            {
                if (Mouse.Captured == DragLayer) Mouse.Capture(null);
                _dragStartPos = null;
                Cursor = Cursors.Arrow;
            };

            // ホイール
            DragLayer.PreviewMouseWheel += (s, e) =>
            {
                var mul = GetMul();
                var delta = Math.Sign(e.Delta) * Step * mul;
                Value += delta;
                e.Handled = true;
            };
        }

        private Point? _dragStartPos;
        private double _dragStartValue;

        private double GetMul()
        {
            var mul = 1.0;
            if (Keyboard.IsKeyDown(Key.LeftShift) || Keyboard.IsKeyDown(Key.RightShift)) mul *= CoarseFactor;
            if (Keyboard.IsKeyDown(Key.LeftAlt) || Keyboard.IsKeyDown(Key.RightAlt)) mul *= FineFactor;
            return mul;
        }

        private double Clamp(double v) => Math.Max(Min, Math.Min(Max, v));

        private void ParseText()
        {
            if (double.TryParse(Box.Text, NumberStyles.Float, CultureInfo.InvariantCulture, out var v) ||
                double.TryParse(Box.Text, NumberStyles.Float, CultureInfo.CurrentCulture, out v))
            {
                Value = v;
            }
            else
            {
                Box.Text = Value.ToString(CultureInfo.CurrentCulture);
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string n) => PropertyChanged?.Invoke(this, new(n));
    }
}
