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

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=234238

namespace sp_lab6
{
	static class Constants
	{
		public const int LENGTH = 0;
		public const int WEIGHT = 1;
		public const int SIZE = 2;

	}
	/// <summary>
	/// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
	/// </summary>
	public sealed partial class Calculator : Page
	{
		public Calculator()
		{
			this.InitializeComponent();
			Operations_SelectionChanged(null, null);
		}

        private void HyperlinkButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }

        private void ButtonBase_OnClick(object sender, RoutedEventArgs e)
        {
            int a;

            Result1.Text = "";
            Result2.Text = "";
            Result3.Text = "";
            Result4.Text = "";

            if (!Int32.TryParse(A.Text, out a))
            {
                Result1.Text = "Operand A is not number!";
                return;
            }

            switch (Operations.SelectedIndex)
            {
                case Constants.LENGTH:
                    Result1.Text = (a / 3.281).ToString();
                    Result2.Text = (a * 39.37).ToString();
                    Result3.Text = (a * 1.094).ToString();
                    Result4.Text = (a / (double)1609).ToString();
                    break;
                case Constants.WEIGHT:
                    Result1.Text = (a * 2.205).ToString();
                    Result2.Text = (a * 35.274).ToString();
                    break;
                case Constants.SIZE:
                    Result1.Text = (a * 2.103).ToString();
                    Result2.Text = (a / 3.785).ToString();
                    break;
            }
        }

        private void Operations_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            if (ResultText1 != null & ResultText2 != null & ResultText3 != null & ResultText4 != null)
            {
                Result1.Text = "";
                Result2.Text = "";
                Result3.Text = "";
                Result4.Text = "";

                switch (Operations.SelectedIndex)
                {
                    case Constants.LENGTH:
                        Input.Text = "Метры";
                        ResultText1.Text = "Фут";
                        ResultText2.Text = "Дюйм";
                        ResultText3.Text = "Ярды";
                        ResultText4.Text = "Мили";
                        ResultText3.Visibility = Visibility.Visible;
                        ResultText4.Visibility = Visibility.Visible;
                        break;
                    case Constants.WEIGHT:
                        Input.Text = "Кг";
                        ResultText1.Text = "Фунты";
                        ResultText2.Text = "Унции";
                        ResultText3.Visibility = Visibility.Collapsed;
                        ResultText4.Visibility = Visibility.Collapsed;
                        break;
                    case Constants.SIZE:
                        Input.Text = "Литры";
                        ResultText1.Text = "Пинты";
                        ResultText2.Text = "Галлоны";
                        ResultText3.Visibility = Visibility.Collapsed;
                        ResultText4.Visibility = Visibility.Collapsed;
                        break;
                }
            }
        }
    }
}
