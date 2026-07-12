/********************************************************************************
** Form generated from reading UI file 'configurationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURATIONDIALOG_H
#define UI_CONFIGURATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ConfigurationDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *delayRadioButton;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *delaySpinBox;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *ConfigurationDialog)
    {
        if (ConfigurationDialog->objectName().isEmpty())
            ConfigurationDialog->setObjectName(QString::fromUtf8("ConfigurationDialog"));
        ConfigurationDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(ConfigurationDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(ConfigurationDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 40, 291, 53));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        delayRadioButton = new QRadioButton(groupBox);
        delayRadioButton->setObjectName(QString::fromUtf8("delayRadioButton"));

        horizontalLayout->addWidget(delayRadioButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        delaySpinBox = new QSpinBox(groupBox);
        delaySpinBox->setObjectName(QString::fromUtf8("delaySpinBox"));
        delaySpinBox->setMaximum(5000);
        delaySpinBox->setSingleStep(10);

        horizontalLayout->addWidget(delaySpinBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(ConfigurationDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigurationDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigurationDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigurationDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigurationDialog)
    {
        ConfigurationDialog->setWindowTitle(QCoreApplication::translate("ConfigurationDialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ConfigurationDialog", "Delay", nullptr));
        delayRadioButton->setText(QCoreApplication::translate("ConfigurationDialog", "Add a delay", nullptr));
        delaySpinBox->setSuffix(QCoreApplication::translate("ConfigurationDialog", " ms", nullptr));
        delaySpinBox->setPrefix(QString());
    } // retranslateUi

};

namespace Ui {
    class ConfigurationDialog: public Ui_ConfigurationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURATIONDIALOG_H
