using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x419

namespace sp_lab6
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Hub_SectionHeaderClick(object sender, HubSectionHeaderClickEventArgs e)
        {
            switch (e.Section.Name)
            {
                case "About":
                    this.Frame.Navigate(typeof(About));
                    break;
                case "Transfer":
                    this.Frame.Navigate(typeof(Calculator));
                    break;
                default:
                    this.Frame.Navigate(typeof(About));
                    break;

            }
        }
    }
}
