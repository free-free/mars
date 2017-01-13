#ifndef SPSETTINGSDIALOG_H
#define SPSETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include "marstypes.h"

namespace Ui {
class SPSettingsDialog;
}

class QIntValidator;

class SPSettingsDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SPSettingsDialog(QWidget *parent = 0);
    ~SPSettingsDialog();
    SerialPortSettings settings() const;
    int exec();

signals:
    void updated(SerialPortSettings settings);

private slots:
    void showSerialPortInfo(int index);
    void applySerialPortConfig();
    void checkCustomBaudRatePolicy(int index);
    void checkCustomDevicePathPolicy(int index);


private:
    void fillSerialPortParameters();
    void fillSerialPortInfo();
    void storeSettings();

private:
    Ui::SPSettingsDialog *ui;
    SerialPortSettings currentSettings;
    QIntValidator * intValidator;

};

#endif // SPSETTINGSDIALOG_H
