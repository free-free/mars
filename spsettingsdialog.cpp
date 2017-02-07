/************************************************************************
 * CopyRights (C): Thogo tech all rights reserved!
 *
 *@file: spsettingsdialog.cpp
 *@description: this file contains the implementation of serial port config window class
 *@version: 0.1
 *@author: jell
 *@date: 2017/02/07
 *
 *
 */
#include "spsettingsdialog.h"
#include "ui_spsettingsdialog.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

SPSettingsDialog::SPSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SPSettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("串口配置"));
    intValidator = new QIntValidator(0, 4000000, this);
    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->spConfigApplyBtn,&QPushButton::clicked,
            this,&SPSettingsDialog::applySerialPortConfig
            );
    connect(ui->serialPortInfoListBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&SPSettingsDialog::showSerialPortInfo
            );
    connect(ui->baudRateBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&SPSettingsDialog::checkCustomBaudRatePolicy
            );
    connect(ui->serialPortInfoListBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&SPSettingsDialog::checkCustomDevicePathPolicy
            );

    fillSerialPortParameters();
    fillSerialPortInfo();
    storeSettings();
}


SPSettingsDialog::~SPSettingsDialog()
{
    delete ui;
}


void SPSettingsDialog::showSerialPortInfo(int index)
{
    if (index == -1)
        return ;
    QStringList list = ui->serialPortInfoListBox->itemData(index).toStringList();
    ui->descriptionLabel->setText(tr("描述: %1").arg(list.count()>1?list.at(1):tr("N/A")));
    ui->manufactureLabel->setText(tr("厂商: %1").arg(list.count()>2?list.at(2):tr("N/A")));
    ui->serialPortNumberLabel->setText(tr("串口号: %1").arg(list.count()>3?list.at(3):tr("N/A")));
    ui->locationLabel->setText(tr("位置: %1").arg(list.count()>4?list.at(4):tr("N/A")));
    ui->VendIdLabel->setText(tr("厂商ID: %1").arg(list.count()>5?list.at(5):tr("N/A")));
    ui->productIdLabel->setText(tr("产品ID: %1").arg(list.count()>6?list.at(6):tr("N/A")));
}


void SPSettingsDialog::applySerialPortConfig()
{
    storeSettings();
    emit updated(currentSettings);
    hide();
}


void SPSettingsDialog::checkCustomBaudRatePolicy(int index)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(index).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if(isCustomBaudRate)
    {
        ui->baudRateBox->clearEditText();
        QLineEdit * edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);

    }
}


void SPSettingsDialog::checkCustomDevicePathPolicy(int index)
{
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(index).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if(isCustomPath)
    {
        ui->serialPortInfoListBox->clearEditText();

    }
}

void SPSettingsDialog::fillSerialPortParameters()
{
    /* fill serial port baudrate box */
    ui->baudRateBox->addItem(QStringLiteral("1200"),QSerialPort::Baud1200);
    ui->baudRateBox->addItem(QStringLiteral("2400"),QSerialPort::Baud2400);
    ui->baudRateBox->addItem(QStringLiteral("4800"),QSerialPort::Baud4800);
    ui->baudRateBox->addItem(QStringLiteral("9600"),QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"),QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"),QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);
    ui->baudRateBox->setCurrentIndex(3);

    /* fill serial port databit box */
    ui->dataBitBox->addItem(QStringLiteral("5"),QSerialPort::Data5);
    ui->dataBitBox->addItem(QStringLiteral("6"),QSerialPort::Data6);
    ui->dataBitBox->addItem(QStringLiteral("7"),QSerialPort::Data7);
    ui->dataBitBox->addItem(QStringLiteral("8"),QSerialPort::Data8);
    ui->dataBitBox->setCurrentIndex(3);
    /* fill serial port stopbit box */
    ui->stopBitBox->addItem(QStringLiteral("1"),QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitBox->addItem(tr("1.5"),QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitBox->addItem(QStringLiteral("2"),QSerialPort::TwoStop);

    /* fill serial port parity */
    ui->checkBitBox->addItem(tr("None"),QSerialPort::NoParity);
    ui->checkBitBox->addItem(tr("Odd"),QSerialPort::OddParity);
    ui->checkBitBox->addItem(tr("Even"),QSerialPort::EvenParity);
    ui->checkBitBox->addItem(tr("Mark"),QSerialPort::MarkParity);
    ui->checkBitBox->addItem(tr("Space"),QSerialPort::SpaceParity);

    /* fill serial port flowcontrol box */
    ui->flowControlBox->addItem(tr("None"),QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"),QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"),QSerialPort::SoftwareControl);
}


void SPSettingsDialog::fillSerialPortInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty()?description:tr("N/A"))
             << (!manufacturer.isEmpty()?manufacturer:tr("N/A"))
             << (!serialNumber.isEmpty()?serialNumber:tr("N/A"))
             << info.systemLocation()
             << (info.vendorIdentifier()?QString::number(info.vendorIdentifier(),16):tr("N/A"))
             << (info.productIdentifier()?QString::number(info.productIdentifier(),16):tr("N/A"));
        ui->serialPortInfoListBox->addItem(list.first(),list);
    }
    ui->serialPortInfoListBox->addItem(tr("Custom"));

}


void SPSettingsDialog::storeSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();
    if(ui->baudRateBox->currentIndex()== 4)
    {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt()
                    );
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitBox->itemData(ui->stopBitBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->checkBitBox->itemData(ui->checkBitBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->checkBitBox->currentText();

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitBox->itemData(ui->dataBitBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

}


SerialPortSettings SPSettingsDialog::settings() const
{
    return currentSettings;
}


int SPSettingsDialog::exec()
{
    fillSerialPortInfo();
    storeSettings();
    return QDialog::exec();
}
