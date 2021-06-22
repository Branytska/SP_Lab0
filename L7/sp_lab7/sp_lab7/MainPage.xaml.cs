using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Security.Cryptography.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Storage;
using Windows.Storage.FileProperties;

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x419

namespace sp_lab7
{
    public sealed partial class MainPage : Page
    {
        private IReadOnlyList<IStorageItem> itemsList;
        private IStorageItem parentFolder;
        private StorageFolder currentFolder;

        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            Windows.Storage.Pickers.FolderPicker folderPicker = new Windows.Storage.Pickers.FolderPicker();
            folderPicker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            folderPicker.FileTypeFilter.Add(".txt");

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            ScanDir(folder);
        }

        private async void ScanDir(StorageFolder folder)
        {
            if (folder != null)
            {
                listBox.Items.Clear();

                currentFolder = folder;

                StorageFolder parentFolder = await folder.GetParentAsync();
                ListBoxItem lbItem = new ListBoxItem();
                lbItem.Content = "Назад";
                lbItem.DoubleTapped += Navigate;
                lbItem.Tag = parentFolder;
                listBox.Items.Add(lbItem);

                itemsList = await folder.GetItemsAsync();
                foreach (var item in itemsList)
                {
                    lbItem = new ListBoxItem();
                    lbItem.Tag = item;
                    if (item is StorageFolder)
                    {
                        lbItem.Content = "[Folder]" + item.Name + "\tДата создания: " + item.DateCreated.DateTime;
                    }
                    else
                    {
                        BasicProperties properties = await item.GetBasicPropertiesAsync();
                        ulong size = properties.Size;
                        string sizestr = "B";

                        if (size > 1024)
                        {
                            sizestr = "KB";
                            size /= 1024;
                        }

                        if (size > 1024)
                        {
                            sizestr = "MB";
                            size /= 1024;
                        }

                        if (size > 1024)
                        {
                            sizestr = "GB";
                            size /= 1024;
                        }

                        lbItem.Content = "[File]" + item.Name + "\tДата создания: " + item.DateCreated.DateTime + "\tРазмер: " + size + sizestr;
                    }

                    listBox.Items.Add(lbItem);
                    lbItem.DoubleTapped += Navigate;
                }

            }
        }

        private void Navigate(object sender, DoubleTappedRoutedEventArgs e)
        {
            ListBoxItem lbItem = (ListBoxItem)sender;

            if (lbItem.Tag is StorageFolder)
            {
                ScanDir((StorageFolder)lbItem.Tag);
            }

        }

        private async void DeleteFile_OnClick(object sender, RoutedEventArgs e)
        {
            if (listBox.SelectedItem == null)
                return;

            ListBoxItem item = (ListBoxItem)listBox.SelectedItem;
            if (item.Tag is Windows.Storage.StorageFile)
            {
                Windows.Storage.StorageFile file = (Windows.Storage.StorageFile)item.Tag;
                await file.DeleteAsync();
            }
            ScanDir(currentFolder);
        }

        private async void CopyFile_OnClick(object sender, RoutedEventArgs e)
        {
            if (listBox.SelectedItem == null)
                return;

            ListBoxItem item = (ListBoxItem)listBox.SelectedItem;
            if (item.Tag is Windows.Storage.StorageFile)
            {
                Windows.Storage.StorageFile file = (Windows.Storage.StorageFile)item.Tag;
                await file.CopyAsync(currentFolder, fileName.Text);
            }
            ScanDir(currentFolder);
        }
    }
}
