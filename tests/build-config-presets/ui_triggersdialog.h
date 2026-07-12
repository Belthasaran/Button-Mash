/********************************************************************************
** Form generated from reading UI file 'triggersdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIGGERSDIALOG_H
#define UI_TRIGGERSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TriggersDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *infoLabel;
    QGroupBox *focusGroup;
    QVBoxLayout *focusLayout;
    QLabel *titleHint;
    QPlainTextEdit *windowTitlesEdit;
    QLabel *classHint;
    QPlainTextEdit *windowClassesEdit;
    QCheckBox *linuxGamepadCheck;
    QLabel *bindHint;
    QTableWidget *bindingsTable;
    QHBoxLayout *extraBtnRow;
    QPushButton *addExtraButton;
    QPushButton *removeExtraButton;
    QSpacerItem *extraSpacer;
    QLabel *setupHint;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TriggersDialog)
    {
        if (TriggersDialog->objectName().isEmpty())
            TriggersDialog->setObjectName(QString::fromUtf8("TriggersDialog"));
        TriggersDialog->resize(720, 520);
        verticalLayout = new QVBoxLayout(TriggersDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        infoLabel = new QLabel(TriggersDialog);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setWordWrap(true);

        verticalLayout->addWidget(infoLabel);

        focusGroup = new QGroupBox(TriggersDialog);
        focusGroup->setObjectName(QString::fromUtf8("focusGroup"));
        focusLayout = new QVBoxLayout(focusGroup);
        focusLayout->setObjectName(QString::fromUtf8("focusLayout"));
        titleHint = new QLabel(focusGroup);
        titleHint->setObjectName(QString::fromUtf8("titleHint"));

        focusLayout->addWidget(titleHint);

        windowTitlesEdit = new QPlainTextEdit(focusGroup);
        windowTitlesEdit->setObjectName(QString::fromUtf8("windowTitlesEdit"));
        windowTitlesEdit->setMaximumSize(QSize(16777215, 80));

        focusLayout->addWidget(windowTitlesEdit);

        classHint = new QLabel(focusGroup);
        classHint->setObjectName(QString::fromUtf8("classHint"));

        focusLayout->addWidget(classHint);

        windowClassesEdit = new QPlainTextEdit(focusGroup);
        windowClassesEdit->setObjectName(QString::fromUtf8("windowClassesEdit"));
        windowClassesEdit->setMaximumSize(QSize(16777215, 60));

        focusLayout->addWidget(windowClassesEdit);


        verticalLayout->addWidget(focusGroup);

        linuxGamepadCheck = new QCheckBox(TriggersDialog);
        linuxGamepadCheck->setObjectName(QString::fromUtf8("linuxGamepadCheck"));

        verticalLayout->addWidget(linuxGamepadCheck);

        bindHint = new QLabel(TriggersDialog);
        bindHint->setObjectName(QString::fromUtf8("bindHint"));

        verticalLayout->addWidget(bindHint);

        bindingsTable = new QTableWidget(TriggersDialog);
        if (bindingsTable->columnCount() < 6)
            bindingsTable->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        bindingsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        bindingsTable->setObjectName(QString::fromUtf8("bindingsTable"));
        bindingsTable->setColumnCount(6);

        verticalLayout->addWidget(bindingsTable);

        extraBtnRow = new QHBoxLayout();
        extraBtnRow->setObjectName(QString::fromUtf8("extraBtnRow"));
        addExtraButton = new QPushButton(TriggersDialog);
        addExtraButton->setObjectName(QString::fromUtf8("addExtraButton"));

        extraBtnRow->addWidget(addExtraButton);

        removeExtraButton = new QPushButton(TriggersDialog);
        removeExtraButton->setObjectName(QString::fromUtf8("removeExtraButton"));

        extraBtnRow->addWidget(removeExtraButton);

        extraSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        extraBtnRow->addItem(extraSpacer);


        verticalLayout->addLayout(extraBtnRow);

        setupHint = new QLabel(TriggersDialog);
        setupHint->setObjectName(QString::fromUtf8("setupHint"));
        setupHint->setWordWrap(true);

        verticalLayout->addWidget(setupHint);

        buttonBox = new QDialogButtonBox(TriggersDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(TriggersDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TriggersDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TriggersDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TriggersDialog);
    } // setupUi

    void retranslateUi(QDialog *TriggersDialog)
    {
        TriggersDialog->setWindowTitle(QCoreApplication::translate("TriggersDialog", "Configure Input Triggers", nullptr));
        infoLabel->setText(QCoreApplication::translate("TriggersDialog", "Keyboard maps only inject while a matching window is focused. If focus is lost, all held mapped keys are released. Macro actions (Run program) fire on button press regardless of focus. RetroArch exclusivity is configured by you \342\200\224 see help text below.", nullptr));
        focusGroup->setTitle(QCoreApplication::translate("TriggersDialog", "Focus windows (keyboard maps)", nullptr));
        titleHint->setText(QCoreApplication::translate("TriggersDialog", "Window title substrings (one per line, case-insensitive):", nullptr));
        classHint->setText(QCoreApplication::translate("TriggersDialog", "Window class substrings (Windows; one per line):", nullptr));
        linuxGamepadCheck->setText(QCoreApplication::translate("TriggersDialog", "Linux: also expose a virtual gamepad via uinput (Button Mash Gamepad)", nullptr));
        bindHint->setText(QCoreApplication::translate("TriggersDialog", "Bindings (SNES button or Extra \342\206\222 Keyboard keyCode / Run program):", nullptr));
        QTableWidgetItem *___qtablewidgetitem = bindingsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TriggersDialog", "Source", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = bindingsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TriggersDialog", "Action", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = bindingsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TriggersDialog", "Key code", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = bindingsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TriggersDialog", "Key label", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = bindingsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TriggersDialog", "Program", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = bindingsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TriggersDialog", "Args", nullptr));
        addExtraButton->setText(QCoreApplication::translate("TriggersDialog", "Add Extra / Macro row", nullptr));
        removeExtraButton->setText(QCoreApplication::translate("TriggersDialog", "Remove selected", nullptr));
        setupHint->setText(QCoreApplication::translate("TriggersDialog", "User setup: For Network RetroPad, enable it in RetroArch and point Share\342\206\222Mirror Target RetroArchRemotePad at 127.0.0.1:55400. For keyboard, bind RetroArch keys to match Key label/codes. On Linux, grant /dev/uinput (udev rule GROUP=input). This dialog does not change RetroArch for you.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TriggersDialog: public Ui_TriggersDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIGGERSDIALOG_H
