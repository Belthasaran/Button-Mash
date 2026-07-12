/********************************************************************************
** Form generated from reading UI file 'inputsourceselector.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTSOURCESELECTOR_H
#define UI_INPUTSOURCESELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InputSourceSelector
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *refreshButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *snesClassicStuffRadioButton;
    QRadioButton *snesClassicRadioButton;
    QHBoxLayout *horizontalLayout_7;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QRadioButton *arduinoRadioButton;
    QHBoxLayout *horizontalLayout;
    QLabel *arduinoPortLabel;
    QComboBox *arduinoComComboBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *arduiType1Radio;
    QRadioButton *arduiType2Radio;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *xinputRadioButton;
    QComboBox *xinputComboBox;
    QPushButton *mappingButton;
    QRadioButton *remoteRadioButton;
    QHBoxLayout *remoteProtoLayout;
    QLabel *remoteProtoLabel;
    QComboBox *remoteProtocolCombo;
    QHBoxLayout *remotePortLayout;
    QLabel *remotePortLabel;
    QSpinBox *remotePortSpin;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *usb2snesRadioButton;
    QComboBox *usb2snesComboBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *usb2gameLabel;
    QComboBox *usb2gameComboBox;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_2;
    QSpinBox *delaiSpinBox;
    QSpacerItem *horizontalSpacer_5;
    QLabel *infoLabel;
    QDialogButtonBox *buttonBox;
    QButtonGroup *sourceRadioGroup;
    QButtonGroup *arduinoRadioGroup;

    void setupUi(QDialog *InputSourceSelector)
    {
        if (InputSourceSelector->objectName().isEmpty())
            InputSourceSelector->setObjectName(QString::fromUtf8("InputSourceSelector"));
        InputSourceSelector->resize(398, 365);
        verticalLayout_3 = new QVBoxLayout(InputSourceSelector);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(InputSourceSelector);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        refreshButton = new QPushButton(InputSourceSelector);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));

        horizontalLayout_4->addWidget(refreshButton);


        verticalLayout_3->addLayout(horizontalLayout_4);

        groupBox = new QGroupBox(InputSourceSelector);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        snesClassicStuffRadioButton = new QRadioButton(groupBox);
        sourceRadioGroup = new QButtonGroup(InputSourceSelector);
        sourceRadioGroup->setObjectName(QString::fromUtf8("sourceRadioGroup"));
        sourceRadioGroup->addButton(snesClassicStuffRadioButton);
        snesClassicStuffRadioButton->setObjectName(QString::fromUtf8("snesClassicStuffRadioButton"));
        snesClassicStuffRadioButton->setEnabled(false);

        verticalLayout_4->addWidget(snesClassicStuffRadioButton);

        snesClassicRadioButton = new QRadioButton(groupBox);
        sourceRadioGroup->addButton(snesClassicRadioButton);
        snesClassicRadioButton->setObjectName(QString::fromUtf8("snesClassicRadioButton"));
        snesClassicRadioButton->setEnabled(false);

        verticalLayout_4->addWidget(snesClassicRadioButton);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout_6 = new QHBoxLayout(frame);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        arduinoRadioButton = new QRadioButton(frame);
        sourceRadioGroup->addButton(arduinoRadioButton);
        arduinoRadioButton->setObjectName(QString::fromUtf8("arduinoRadioButton"));
        arduinoRadioButton->setEnabled(false);

        verticalLayout->addWidget(arduinoRadioButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        arduinoPortLabel = new QLabel(frame);
        arduinoPortLabel->setObjectName(QString::fromUtf8("arduinoPortLabel"));
        arduinoPortLabel->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(arduinoPortLabel->sizePolicy().hasHeightForWidth());
        arduinoPortLabel->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(arduinoPortLabel);

        arduinoComComboBox = new QComboBox(frame);
        arduinoComComboBox->setObjectName(QString::fromUtf8("arduinoComComboBox"));
        arduinoComComboBox->setEnabled(false);

        horizontalLayout->addWidget(arduinoComComboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        arduiType1Radio = new QRadioButton(frame);
        arduinoRadioGroup = new QButtonGroup(InputSourceSelector);
        arduinoRadioGroup->setObjectName(QString::fromUtf8("arduinoRadioGroup"));
        arduinoRadioGroup->addButton(arduiType1Radio);
        arduiType1Radio->setObjectName(QString::fromUtf8("arduiType1Radio"));
        arduiType1Radio->setEnabled(false);

        horizontalLayout_2->addWidget(arduiType1Radio);

        arduiType2Radio = new QRadioButton(frame);
        arduinoRadioGroup->addButton(arduiType2Radio);
        arduiType2Radio->setObjectName(QString::fromUtf8("arduiType2Radio"));
        arduiType2Radio->setEnabled(false);

        horizontalLayout_2->addWidget(arduiType2Radio);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_6->addLayout(verticalLayout);


        horizontalLayout_7->addWidget(frame);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_7);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(InputSourceSelector);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_2);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        xinputRadioButton = new QRadioButton(groupBox_2);
        sourceRadioGroup->addButton(xinputRadioButton);
        xinputRadioButton->setObjectName(QString::fromUtf8("xinputRadioButton"));
        xinputRadioButton->setEnabled(false);

        verticalLayout_2->addWidget(xinputRadioButton);

        xinputComboBox = new QComboBox(groupBox_2);
        xinputComboBox->setObjectName(QString::fromUtf8("xinputComboBox"));
        xinputComboBox->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(xinputComboBox->sizePolicy().hasHeightForWidth());
        xinputComboBox->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(xinputComboBox);

        mappingButton = new QPushButton(groupBox_2);
        mappingButton->setObjectName(QString::fromUtf8("mappingButton"));
        mappingButton->setEnabled(false);

        verticalLayout_2->addWidget(mappingButton);

        remoteRadioButton = new QRadioButton(groupBox_2);
        sourceRadioGroup->addButton(remoteRadioButton);
        remoteRadioButton->setObjectName(QString::fromUtf8("remoteRadioButton"));

        verticalLayout_2->addWidget(remoteRadioButton);

        remoteProtoLayout = new QHBoxLayout();
        remoteProtoLayout->setObjectName(QString::fromUtf8("remoteProtoLayout"));
        remoteProtoLabel = new QLabel(groupBox_2);
        remoteProtoLabel->setObjectName(QString::fromUtf8("remoteProtoLabel"));

        remoteProtoLayout->addWidget(remoteProtoLabel);

        remoteProtocolCombo = new QComboBox(groupBox_2);
        remoteProtocolCombo->addItem(QString());
        remoteProtocolCombo->addItem(QString());
        remoteProtocolCombo->setObjectName(QString::fromUtf8("remoteProtocolCombo"));

        remoteProtoLayout->addWidget(remoteProtocolCombo);


        verticalLayout_2->addLayout(remoteProtoLayout);

        remotePortLayout = new QHBoxLayout();
        remotePortLayout->setObjectName(QString::fromUtf8("remotePortLayout"));
        remotePortLabel = new QLabel(groupBox_2);
        remotePortLabel->setObjectName(QString::fromUtf8("remotePortLabel"));

        remotePortLayout->addWidget(remotePortLabel);

        remotePortSpin = new QSpinBox(groupBox_2);
        remotePortSpin->setObjectName(QString::fromUtf8("remotePortSpin"));
        remotePortSpin->setMaximum(65535);
        remotePortSpin->setMinimum(1);
        remotePortSpin->setValue(27151);

        remotePortLayout->addWidget(remotePortSpin);


        verticalLayout_2->addLayout(remotePortLayout);


        horizontalLayout_8->addLayout(verticalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        usb2snesRadioButton = new QRadioButton(groupBox_2);
        sourceRadioGroup->addButton(usb2snesRadioButton);
        usb2snesRadioButton->setObjectName(QString::fromUtf8("usb2snesRadioButton"));
        usb2snesRadioButton->setEnabled(false);

        horizontalLayout_3->addWidget(usb2snesRadioButton);

        usb2snesComboBox = new QComboBox(groupBox_2);
        usb2snesComboBox->setObjectName(QString::fromUtf8("usb2snesComboBox"));
        usb2snesComboBox->setEnabled(false);

        horizontalLayout_3->addWidget(usb2snesComboBox);


        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        usb2gameLabel = new QLabel(groupBox_2);
        usb2gameLabel->setObjectName(QString::fromUtf8("usb2gameLabel"));

        horizontalLayout_5->addWidget(usb2gameLabel);

        usb2gameComboBox = new QComboBox(groupBox_2);
        usb2gameComboBox->setObjectName(QString::fromUtf8("usb2gameComboBox"));

        horizontalLayout_5->addWidget(usb2gameComboBox);


        verticalLayout_5->addLayout(horizontalLayout_5);


        horizontalLayout_8->addLayout(verticalLayout_5);


        verticalLayout_3->addWidget(groupBox_2);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);

        label_2 = new QLabel(InputSourceSelector);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_9->addWidget(label_2);

        delaiSpinBox = new QSpinBox(InputSourceSelector);
        delaiSpinBox->setObjectName(QString::fromUtf8("delaiSpinBox"));
        delaiSpinBox->setMaximum(10000);

        horizontalLayout_9->addWidget(delaiSpinBox);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout_9);

        infoLabel = new QLabel(InputSourceSelector);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(infoLabel);

        buttonBox = new QDialogButtonBox(InputSourceSelector);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(InputSourceSelector);

        QMetaObject::connectSlotsByName(InputSourceSelector);
    } // setupUi

    void retranslateUi(QDialog *InputSourceSelector)
    {
        InputSourceSelector->setWindowTitle(QCoreApplication::translate("InputSourceSelector", "Input source selector dialog", nullptr));
        label->setText(QCoreApplication::translate("InputSourceSelector", "<html><head/><body><p>Only available source will be selectable<br/>Hit the refresh button if the first detection fails</p></body></html>", nullptr));
        refreshButton->setText(QCoreApplication::translate("InputSourceSelector", "Refresh", nullptr));
        groupBox->setTitle(QCoreApplication::translate("InputSourceSelector", "Console", nullptr));
        snesClassicStuffRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "SNES Classic (with mod)", nullptr));
        snesClassicRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "SNES Classic", nullptr));
        arduinoRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "SNES with Arduino", nullptr));
        arduinoPortLabel->setText(QCoreApplication::translate("InputSourceSelector", "Port :", nullptr));
        arduiType1Radio->setText(QCoreApplication::translate("InputSourceSelector", "Type 1 (Nintendo Spy firmware)", nullptr));
        arduiType2Radio->setText(QCoreApplication::translate("InputSourceSelector", "Type 2", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("InputSourceSelector", "Emulator and special", nullptr));
        xinputRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "PC (XInput) ", nullptr));
        mappingButton->setText(QCoreApplication::translate("InputSourceSelector", "Set mapping", nullptr));
        remoteRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "Remote server", nullptr));
        remoteProtoLabel->setText(QCoreApplication::translate("InputSourceSelector", "Protocol", nullptr));
        remoteProtocolCombo->setItemText(0, QCoreApplication::translate("InputSourceSelector", "ButtonMash", nullptr));
        remoteProtocolCombo->setItemText(1, QCoreApplication::translate("InputSourceSelector", "RetroArchRemotePad", nullptr));

        remotePortLabel->setText(QCoreApplication::translate("InputSourceSelector", "Listen port", nullptr));
        usb2snesRadioButton->setText(QCoreApplication::translate("InputSourceSelector", "Usb2Snes", nullptr));
        usb2gameLabel->setText(QCoreApplication::translate("InputSourceSelector", "Game :", nullptr));
        label_2->setText(QCoreApplication::translate("InputSourceSelector", "Delai :", nullptr));
        delaiSpinBox->setSuffix(QCoreApplication::translate("InputSourceSelector", " ms", nullptr));
        infoLabel->setText(QCoreApplication::translate("InputSourceSelector", "Why am I here?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InputSourceSelector: public Ui_InputSourceSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTSOURCESELECTOR_H
