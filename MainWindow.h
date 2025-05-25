#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDomDocument>
#include <QRandomGenerator>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButtonGenerateDate_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString generateRowText(const QDateTime& time);
    QString m_outputFilePath;
    bool saveRecord(const QDomDocument &data);
    void generateRecord();
};
#endif // MAINWINDOW_H
