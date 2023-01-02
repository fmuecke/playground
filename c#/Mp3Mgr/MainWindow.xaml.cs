using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;


namespace Mp3Mgr
{
    //public class ValToForegroundConverter : IValueConverter
    //{

    //    public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
    //    {
    //        // Put logic to check date is a Sunday and return foreground 
    //        return new SolidColorBrush(Colors.Red);
    //    }

    //    public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
    //    {
    //        return null;

    //    }
    //}
    
    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private ObservableCollection<Mp3Album> _directories = new ObservableCollection<Mp3Album>();

        private bool ValidateDirectory(string path)
        {
            if (string.IsNullOrEmpty(path) || !Directory.Exists(path))
            {
                return false;
            }

            return true;
        }

        private static void CheckBoxesChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var that = d as MainWindow;
            if (that != null)
            {
                that.UpdateViewFilter();
            }
        }

        private static void SelectedAlbumChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var that = d as MainWindow;
            if (that != null)
            {
                //that.UpdateViewFilter();
            }
        }

        private void NotifyPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public readonly static DependencyProperty ShowOkProperty =
                DependencyProperty.Register("ShowOk", typeof(bool), typeof(MainWindow), 
                    new UIPropertyMetadata(true, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty ShowPathTooShortProperty =
                DependencyProperty.Register("ShowPathTooShort", typeof(bool), typeof(MainWindow),
                    new UIPropertyMetadata(false, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty ShowIncorrectFileNameProperty =
            DependencyProperty.Register("ShowIncorrectFileName", typeof(bool), typeof(MainWindow),
                new UIPropertyMetadata(false, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty ShowArtistFolderHasUnderscoresProperty =
            DependencyProperty.Register("ShowArtistFolderHasUnderscores", typeof(bool), typeof(MainWindow),
                new UIPropertyMetadata(false, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty ShowAlbumFoldereHasUnderscoresProperty =
            DependencyProperty.Register("ShowAlbumFoldereHasUnderscores", typeof(bool), typeof(MainWindow),
                new UIPropertyMetadata(false, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty ShowFileNameHasUnderscoresProperty =
            DependencyProperty.Register("ShowFileNameHasUnderscores", typeof(bool), typeof(MainWindow),
                new UIPropertyMetadata(false, new PropertyChangedCallback(CheckBoxesChanged)));

        public readonly static DependencyProperty SelectedAlbumProperty =
            DependencyProperty.Register("SelectedAlbum", typeof(Mp3Album), typeof(MainWindow),
                new UIPropertyMetadata(null, new PropertyChangedCallback(SelectedAlbumChanged)));
        
        
        public bool ShowOk
        {
            get { return (bool)GetValue(ShowOkProperty); }
            set { SetValue(ShowOkProperty, value); }
        }

        public bool ShowPathTooShort
        {
            get { return (bool)GetValue(ShowPathTooShortProperty); }
            set { SetValue(ShowPathTooShortProperty, value); }
        }

        public bool ShowIncorrectFileName
        {
            get { return (bool)GetValue(ShowIncorrectFileNameProperty); }
            set { SetValue(ShowIncorrectFileNameProperty, value); }
        }

        public bool ShowArtistFolderHasUnderscores
        {
            get { return (bool)GetValue(ShowArtistFolderHasUnderscoresProperty); }
            set { SetValue(ShowArtistFolderHasUnderscoresProperty, value); }
        }

        public bool ShowAlbumFoldereHasUnderscores
        {
            get { return (bool)GetValue(ShowAlbumFoldereHasUnderscoresProperty); }
            set { SetValue(ShowAlbumFoldereHasUnderscoresProperty, value); }
        }

        public bool ShowFileNameHasUnderscores
        {
            get { return (bool)GetValue(ShowFileNameHasUnderscoresProperty); }
            set { SetValue(ShowFileNameHasUnderscoresProperty, value); }
        }

        public Mp3Album SelectedAlbum
        {
            get { return (Mp3Album)GetValue(SelectedAlbumProperty); }
            set 
            {
                //if (value != null)
                {
                    SetValue(SelectedAlbumProperty, value);
                    NotifyPropertyChanged("SelectedAlbum");
                }
            
            }
        }

        private void ValidateBaseDirectory()
        {
            if (button_Scan != null && textBox_BaseDirectory != null)
            {
                if (!ValidateDirectory(textBox_BaseDirectory.Text))
                {
                    textBox_BaseDirectory.Foreground = Brushes.Red;
                    button_Scan.IsEnabled = false;
                }
                else
                {
                    textBox_BaseDirectory.Foreground = Brushes.Black;
                    button_Scan.IsEnabled = true;
                }
            }
        }

        public ObservableCollection<Mp3Album> Directories
        {
            get { return _directories; }
            set { _directories = value; }
        }

        public MainWindow()
        {
            InitializeComponent();

        }

        private void button_BrowseDirectory_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new System.Windows.Forms.FolderBrowserDialog();
            System.Windows.Forms.DialogResult result = dialog.ShowDialog();
        }

        private void button_Scan_Click(object sender, RoutedEventArgs e)
        {
            System.IO.DirectoryInfo dirInfo = new System.IO.DirectoryInfo(textBox_BaseDirectory.Text);

            UpdateViewFilter();

            _directories.Clear();

            try
            {
                foreach (var dir in dirInfo.GetDirectories("*", SearchOption.AllDirectories))
                {
                    Mp3Album album = new Mp3Album(textBox_BaseDirectory.Text, dir.FullName);
                    _directories.Add(album);
                }
            }
            catch (UnauthorizedAccessException) {}
        }

        private void textBox_BaseDirectory_TextChanged(object sender, TextChangedEventArgs e)
        {
            ValidateBaseDirectory();
        }

        private void UpdateViewFilter()
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(folderView.ItemsSource);
            view.Filter = (x) =>
            {
                if (!ShowOk && ((Mp3Album)x).IsOk)
                    return false;

                if (!ShowIncorrectFileName && ((Mp3Album)x).CheckResult == PathChecker.CheckResult.IncorrectFileName.ToString())
                    return false;

                if (!ShowPathTooShort && ((Mp3Album)x).CheckResult == PathChecker.CheckResult.PathTooShort.ToString())
                    return false;

                if (!ShowArtistFolderHasUnderscores && ((Mp3Album)x).CheckResult == PathChecker.CheckResult.ArtistFolderHasUnderscores.ToString())
                    return false;

                if (!ShowAlbumFoldereHasUnderscores && ((Mp3Album)x).CheckResult == PathChecker.CheckResult.AlbumFolderHasUnderscores.ToString())
                    return false;

                if (!ShowFileNameHasUnderscores && ((Mp3Album)x).CheckResult == PathChecker.CheckResult.FileNameHasUnderscores.ToString())
                    return false;

                return true;
            };
        }

        private void fileView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (sender != null && sender == fileView)
            {
                if (e.ChangedButton == MouseButton.Left)
                {
                    var x = e.OriginalSource as ListViewItem;
                    
                     DependencyObject obj = (DependencyObject)e.OriginalSource;

                     while (obj != null && obj != fileView)
                     {
                         if (obj.GetType() == typeof(ListViewItem))
                         {
                             Mp3File file = ((ListViewItem)obj).Content as Mp3File;

                             if (null != file)
                             {
                                 Dialogs.EditValueDialog evd = new Dialogs.EditValueDialog(file.FileName) 
                                 { Owner = this };

                                 bool? isOk = evd.ShowDialog();
                                 if (isOk != null && isOk == true)
                                 {
                                     try
                                     {
                                         System.IO.File.Move(SelectedAlbum.Path + "\\" + file.FileName, SelectedAlbum.Path + "\\" + evd.Value);
                                         file.FileName = evd.Value;
                                     }
                                     catch (Exception ex)
                                     {
                                         MessageBox.Show(this, ex.Message, "Error renaming file");
                                     }
                                 }

                                 e.Handled = true;
                             }

                             break;
                         }

                         obj = VisualTreeHelper.GetParent(obj);
                    }
                }
            }
        }

        private void folderView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e != null && e.AddedItems.Count > 0)
            {
                SelectedAlbum = (Mp3Album)e.AddedItems[0];
            }
            else
            {
                SelectedAlbum = null;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
