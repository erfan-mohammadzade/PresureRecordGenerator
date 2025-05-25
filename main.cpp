#include "MainWindow.h"
#include <windows.h>
#include <QApplication>
#include <QFile>
#include <QString>

bool isSystemInDarkMode();
QString loadStyle(const QString& path);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString lightStyle = loadStyle(":/Res/Light.qss");
    QString darkStyle = loadStyle(":/Res/Dark.qss");

    qApp->setStyleSheet(isSystemInDarkMode() ? darkStyle : lightStyle);

    w.show();
    return a.exec();
}

bool isSystemInDarkMode() {
    DWORD value = 1;
    DWORD dataSize = sizeof(value);
    HKEY hKey;

    if (RegOpenKeyExW(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr,
                             reinterpret_cast<LPBYTE>(&value), &dataSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return value == 0; // 0 = dark mode
        }
        RegCloseKey(hKey);
    }

    return false; // default to light mode
}

QString loadStyle(const QString& path) {
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text))
        return QString::fromUtf8(file.readAll());
    return "";
}
