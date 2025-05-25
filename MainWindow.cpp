#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_outputFilePath =  QDir::homePath();
    ui->lineEditPath->setText(m_outputFilePath);
    ui->dateTimeEditStartDate->setDateTime(QDateTime::currentDateTime());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::generateRowText(const QDateTime &time)
{
    int val1 = QRandomGenerator::global()->bounded(ui->lineEditMinHRV->text().toInt(), ui->lineEditMaxHRV->text().toInt());   // Heart rate
    int val2 = QRandomGenerator::global()->bounded(ui->lineEditMinSBP->text().toInt(), ui->lineEditMaxSBP->text().toInt()); // Systolic
    int val3 = QRandomGenerator::global()->bounded(ui->lineEditMinDBP->text().toInt(), ui->lineEditMaxDBP->text().toInt());   // Diastolic
    int val4 = QRandomGenerator::global()->bounded(1, 4);     // e.g., Status
    int val5 = QRandomGenerator::global()->bounded(0, 2);     // e.g., Flag

    return QString("%1 %2 %3 %4 %5 %6")
        .arg(time.toString("yyyy-MM-dd HH:mm:ss"))
        .arg(val1)
        .arg(val2)
        .arg(val3)
        .arg(val4)
        .arg(val5);
}

bool MainWindow::saveRecord(const QDomDocument &data)
{
    QFile file(m_outputFilePath + "/" + ui->lineEditFileName->text() +".xml");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Encoding::Utf8);
        stream << data.toString(2); // Indent by 2 spaces
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::generateRecord()
{
    QDomDocument doc("Record");
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QDomElement recordInfo = doc.createElement("RecordInfo");
    root.appendChild(recordInfo);

    QDateTime baseTime = ui->dateTimeEditStartDate->dateTime();
    for (int i = 0; i < ui->lineEditRecordNumber->text().toInt(); ++i) {
        baseTime = baseTime.addSecs(QRandomGenerator::global()->bounded(120, 900)); // 2-15 minutes
        QDomElement row = doc.createElement("row");
        QDomText rowText = doc.createTextNode(generateRowText(baseTime));
        row.appendChild(rowText);
        recordInfo.appendChild(row);
    }

    QDomElement patientInfo = doc.createElement("PatientInfo");
    root.appendChild(patientInfo);

    auto appendTag = [&](const QString& tag, const QString& value) {
        QDomElement element = doc.createElement(tag);
        QDomText text = doc.createTextNode(value);
        element.appendChild(text);
        patientInfo.appendChild(element);
    };

    appendTag("PatientID", ui->lineEditPID->text());
    appendTag("Sex", ui->lineEditAge->text());
    appendTag("Name", ui->lineEditFirstName->text() + ui->lineEditLastName->text());
    appendTag("Age", ui->lineEditAge->text());
    appendTag("AgeUnit", "Y");
    appendTag("ExamItem", "16");
    appendTag("examdeptCode", "1");
    appendTag("ResultStatus", "2");

    // Write to file
    if (saveRecord(doc))
    {
        QMessageBox::information(this, "Success", "Record Generated");
    }
    else
    {
        QMessageBox::critical(this, "Failed", "Record Failed Generated");
    }
}

void MainWindow::on_pushButtonGenerateDate_clicked()
{
    if(ui->lineEditFileName->text().isEmpty() || ui->lineEditPath->text().isEmpty())
    {
        QMessageBox::critical(this, "Failed", "Record Failed Generated");
        return;
    }
    generateRecord();
}


void MainWindow::on_pushButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty()) m_outputFilePath = folderPath;; ui->lineEditPath->setText(m_outputFilePath);
}

