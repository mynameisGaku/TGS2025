using System.Globalization;
using System.Text.Json;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace MainCanvasEditor
{
    public class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            => (value is bool b && b) ? Visibility.Visible : Visibility.Collapsed;

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            => value is Visibility v && v == Visibility.Visible;
    }

    // 選択中の枠色：青
    public class SelectedToBrushConverter : IValueConverter
    {
        private static readonly SolidColorBrush BlueSel = new(Color.FromRgb(0x33, 0x99, 0xFF));
        private static readonly SolidColorBrush Gray = new(Color.FromRgb(0x88, 0x88, 0x88));

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            => (value is bool b && b) ? BlueSel : Gray;

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) => false;
    }

    public class SelectedThicknessConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            => (value is bool b && b) ? new Thickness(3) : new Thickness(1);

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            => new Thickness(1);
    }

    // UIEvent.Argument を JSON文字列 <-> JsonElement/任意オブジェクト に相互変換
    public class JsonAnyToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is JsonElement je) return je.GetRawText();
            if (value == null) return "{}";
            if (value is string s) return s;
            try { return JsonSerializer.Serialize(value); }
            catch { return value.ToString() ?? "{}"; }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var text = (value as string) ?? "";
            text = string.IsNullOrWhiteSpace(text) ? "{}" : text;
            try
            {
                using var doc = JsonDocument.Parse(text);
                return doc.RootElement.Clone(); // JsonElement として保持
            }
            catch
            {
                // パースできなかった場合は文字列のまま
                return text;
            }
        }
    }
}
