// pz_lab0.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <limits.h>

using namespace std;

int main()
{
    signed char sc;
    unsigned char uc;
    char c;
    short int si;
    unsigned short int usi;
    int i;
    unsigned int ui;
    long int li;
    unsigned long int uli;

    cout << "Type \t\t\t| Max \t\t| Min \t\t| Size" << "\n";
    cout << "signed char \t\t| " << SCHAR_MAX << " \t\t| " << SCHAR_MIN << " \t\t| " << sizeof(sc) << " bytes\n";
    cout << "unsigned char \t\t| " << UCHAR_MAX << " \t\t| " << 0 << " \t\t| " << sizeof(uc) << " bytes\n";
    cout << "char \t\t\t| " << CHAR_MAX << " \t\t| " << CHAR_MIN << " \t\t| " << sizeof(c) << " bytes\n";
    cout << "short int \t\t| " << SHRT_MAX << " \t| " << SHRT_MIN << " \t| " << sizeof(si) << " bytes\n";
    cout << "unsigned short int \t| " << USHRT_MAX << " \t| " << 0 << " \t\t| " << sizeof(usi) << " bytes\n";
    cout << "int \t\t\t| " << INT_MAX << " \t| " << INT_MIN << " \t| " << sizeof(i) << " bytes\n";
    cout << "unsidnet int \t\t| " << UINT_MAX << " \t| " << 0 << " \t\t| " << sizeof(ui) << " bytes\n";
    cout << "long int \t\t| " << LONG_MAX << " \t| " << LONG_MIN << " \t| " << sizeof(li) << " bytes\n";
    cout << "unsigment long ing \t| " << ULONG_MAX << " \t| " << 0 << " \t\t| " << sizeof(c) << " bytes\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
