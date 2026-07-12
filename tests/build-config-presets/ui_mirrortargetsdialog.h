/********************************************************************************
** Form generated from reading UI file 'mirrortargetsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIRRORTARGETSDIALOG_H
#define UI_MIRRORTARGETSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MirrorTargetsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *sessionLabel;
    QGroupBox *serversGroup;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *targetsTable;
    QHBoxLayout *horizontalLayout;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *clearButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *logsGroup;
    QFormLayout *formLayout;
    QCheckBox *binCheck;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *binPathEdit;
    QPushButton *browseBinButton;
    QCheckBox *fullTextCheck;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *fullTextPathEdit;
    QPushButton *browseFullTextButton;
    QCheckBox *syncTextCheck;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *syncTextPathEdit;
    QPushButton *browseSyncTextButton;
    QCheckBox *lastNCheck;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lastNNLabel;
    QSpinBox *lastNSpin;
    QLineEdit *lastNPathEdit;
    QPushButton *browseLastNButton;
    QGroupBox *browserSourceGroup;
    QVBoxLayout *browserSourceLayout;
    QHBoxLayout *browserSourceEnableLayout;
    QCheckBox *browserSourceCheck;
    QLabel *browserSourcePortLabel;
    QSpinBox *browserSourcePortSpin;
    QHBoxLayout *browserSourceUrlsLayout;
    QLabel *browserSourceMainUrlLabel;
    QLineEdit *browserSourceMainUrlEdit;
    QPushButton *copyMainUrlButton;
    QHBoxLayout *browserSourceStackedUrlsLayout;
    QLabel *browserSourceStackedUrlLabel;
    QLineEdit *browserSourceStackedUrlEdit;
    QPushButton *copyStackedUrlButton;
    QHBoxLayout *browserSourcePianoUrlsLayout;
    QLabel *browserSourcePianoUrlLabel;
    QLineEdit *browserSourcePianoUrlEdit;
    QPushButton *copyPianoUrlButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MirrorTargetsDialog)
    {
        if (MirrorTargetsDialog->objectName().isEmpty())
            MirrorTargetsDialog->setObjectName(QString::fromUtf8("MirrorTargetsDialog"));
        MirrorTargetsDialog->resize(620, 520);
        verticalLayout = new QVBoxLayout(MirrorTargetsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        sessionLabel = new QLabel(MirrorTargetsDialog);
        sessionLabel->setObjectName(QString::fromUtf8("sessionLabel"));
        sessionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

        verticalLayout->addWidget(sessionLabel);

        serversGroup = new QGroupBox(MirrorTargetsDialog);
        serversGroup->setObjectName(QString::fromUtf8("serversGroup"));
        verticalLayout_2 = new QVBoxLayout(serversGroup);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        targetsTable = new QTableWidget(serversGroup);
        if (targetsTable->columnCount() < 4)
            targetsTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        targetsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        targetsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        targetsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        targetsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        targetsTable->setObjectName(QString::fromUtf8("targetsTable"));
        targetsTable->setColumnCount(4);
        targetsTable->setRowCount(0);

        verticalLayout_2->addWidget(targetsTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addButton = new QPushButton(serversGroup);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        horizontalLayout->addWidget(addButton);

        removeButton = new QPushButton(serversGroup);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        horizontalLayout->addWidget(removeButton);

        clearButton = new QPushButton(serversGroup);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        horizontalLayout->addWidget(clearButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(serversGroup);

        logsGroup = new QGroupBox(MirrorTargetsDialog);
        logsGroup->setObjectName(QString::fromUtf8("logsGroup"));
        formLayout = new QFormLayout(logsGroup);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        binCheck = new QCheckBox(logsGroup);
        binCheck->setObjectName(QString::fromUtf8("binCheck"));

        formLayout->setWidget(0, QFormLayout::LabelRole, binCheck);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        binPathEdit = new QLineEdit(logsGroup);
        binPathEdit->setObjectName(QString::fromUtf8("binPathEdit"));

        horizontalLayout_2->addWidget(binPathEdit);

        browseBinButton = new QPushButton(logsGroup);
        browseBinButton->setObjectName(QString::fromUtf8("browseBinButton"));

        horizontalLayout_2->addWidget(browseBinButton);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        fullTextCheck = new QCheckBox(logsGroup);
        fullTextCheck->setObjectName(QString::fromUtf8("fullTextCheck"));

        formLayout->setWidget(1, QFormLayout::LabelRole, fullTextCheck);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        fullTextPathEdit = new QLineEdit(logsGroup);
        fullTextPathEdit->setObjectName(QString::fromUtf8("fullTextPathEdit"));

        horizontalLayout_3->addWidget(fullTextPathEdit);

        browseFullTextButton = new QPushButton(logsGroup);
        browseFullTextButton->setObjectName(QString::fromUtf8("browseFullTextButton"));

        horizontalLayout_3->addWidget(browseFullTextButton);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);

        syncTextCheck = new QCheckBox(logsGroup);
        syncTextCheck->setObjectName(QString::fromUtf8("syncTextCheck"));

        formLayout->setWidget(2, QFormLayout::LabelRole, syncTextCheck);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        syncTextPathEdit = new QLineEdit(logsGroup);
        syncTextPathEdit->setObjectName(QString::fromUtf8("syncTextPathEdit"));

        horizontalLayout_4->addWidget(syncTextPathEdit);

        browseSyncTextButton = new QPushButton(logsGroup);
        browseSyncTextButton->setObjectName(QString::fromUtf8("browseSyncTextButton"));

        horizontalLayout_4->addWidget(browseSyncTextButton);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_4);

        lastNCheck = new QCheckBox(logsGroup);
        lastNCheck->setObjectName(QString::fromUtf8("lastNCheck"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lastNCheck);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lastNNLabel = new QLabel(logsGroup);
        lastNNLabel->setObjectName(QString::fromUtf8("lastNNLabel"));

        horizontalLayout_5->addWidget(lastNNLabel);

        lastNSpin = new QSpinBox(logsGroup);
        lastNSpin->setObjectName(QString::fromUtf8("lastNSpin"));
        lastNSpin->setMinimum(1);
        lastNSpin->setMaximum(500);
        lastNSpin->setValue(10);

        horizontalLayout_5->addWidget(lastNSpin);

        lastNPathEdit = new QLineEdit(logsGroup);
        lastNPathEdit->setObjectName(QString::fromUtf8("lastNPathEdit"));

        horizontalLayout_5->addWidget(lastNPathEdit);

        browseLastNButton = new QPushButton(logsGroup);
        browseLastNButton->setObjectName(QString::fromUtf8("browseLastNButton"));

        horizontalLayout_5->addWidget(browseLastNButton);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_5);


        verticalLayout->addWidget(logsGroup);

        browserSourceGroup = new QGroupBox(MirrorTargetsDialog);
        browserSourceGroup->setObjectName(QString::fromUtf8("browserSourceGroup"));
        browserSourceLayout = new QVBoxLayout(browserSourceGroup);
        browserSourceLayout->setObjectName(QString::fromUtf8("browserSourceLayout"));
        browserSourceEnableLayout = new QHBoxLayout();
        browserSourceEnableLayout->setObjectName(QString::fromUtf8("browserSourceEnableLayout"));
        browserSourceCheck = new QCheckBox(browserSourceGroup);
        browserSourceCheck->setObjectName(QString::fromUtf8("browserSourceCheck"));

        browserSourceEnableLayout->addWidget(browserSourceCheck);

        browserSourcePortLabel = new QLabel(browserSourceGroup);
        browserSourcePortLabel->setObjectName(QString::fromUtf8("browserSourcePortLabel"));

        browserSourceEnableLayout->addWidget(browserSourcePortLabel);

        browserSourcePortSpin = new QSpinBox(browserSourceGroup);
        browserSourcePortSpin->setObjectName(QString::fromUtf8("browserSourcePortSpin"));
        browserSourcePortSpin->setMinimum(1024);
        browserSourcePortSpin->setMaximum(65535);
        browserSourcePortSpin->setValue(28765);

        browserSourceEnableLayout->addWidget(browserSourcePortSpin);


        browserSourceLayout->addLayout(browserSourceEnableLayout);

        browserSourceUrlsLayout = new QHBoxLayout();
        browserSourceUrlsLayout->setObjectName(QString::fromUtf8("browserSourceUrlsLayout"));
        browserSourceMainUrlLabel = new QLabel(browserSourceGroup);
        browserSourceMainUrlLabel->setObjectName(QString::fromUtf8("browserSourceMainUrlLabel"));

        browserSourceUrlsLayout->addWidget(browserSourceMainUrlLabel);

        browserSourceMainUrlEdit = new QLineEdit(browserSourceGroup);
        browserSourceMainUrlEdit->setObjectName(QString::fromUtf8("browserSourceMainUrlEdit"));
        browserSourceMainUrlEdit->setReadOnly(true);

        browserSourceUrlsLayout->addWidget(browserSourceMainUrlEdit);

        copyMainUrlButton = new QPushButton(browserSourceGroup);
        copyMainUrlButton->setObjectName(QString::fromUtf8("copyMainUrlButton"));

        browserSourceUrlsLayout->addWidget(copyMainUrlButton);


        browserSourceLayout->addLayout(browserSourceUrlsLayout);

        browserSourceStackedUrlsLayout = new QHBoxLayout();
        browserSourceStackedUrlsLayout->setObjectName(QString::fromUtf8("browserSourceStackedUrlsLayout"));
        browserSourceStackedUrlLabel = new QLabel(browserSourceGroup);
        browserSourceStackedUrlLabel->setObjectName(QString::fromUtf8("browserSourceStackedUrlLabel"));

        browserSourceStackedUrlsLayout->addWidget(browserSourceStackedUrlLabel);

        browserSourceStackedUrlEdit = new QLineEdit(browserSourceGroup);
        browserSourceStackedUrlEdit->setObjectName(QString::fromUtf8("browserSourceStackedUrlEdit"));
        browserSourceStackedUrlEdit->setReadOnly(true);

        browserSourceStackedUrlsLayout->addWidget(browserSourceStackedUrlEdit);

        copyStackedUrlButton = new QPushButton(browserSourceGroup);
        copyStackedUrlButton->setObjectName(QString::fromUtf8("copyStackedUrlButton"));

        browserSourceStackedUrlsLayout->addWidget(copyStackedUrlButton);


        browserSourceLayout->addLayout(browserSourceStackedUrlsLayout);

        browserSourcePianoUrlsLayout = new QHBoxLayout();
        browserSourcePianoUrlsLayout->setObjectName(QString::fromUtf8("browserSourcePianoUrlsLayout"));
        browserSourcePianoUrlLabel = new QLabel(browserSourceGroup);
        browserSourcePianoUrlLabel->setObjectName(QString::fromUtf8("browserSourcePianoUrlLabel"));

        browserSourcePianoUrlsLayout->addWidget(browserSourcePianoUrlLabel);

        browserSourcePianoUrlEdit = new QLineEdit(browserSourceGroup);
        browserSourcePianoUrlEdit->setObjectName(QString::fromUtf8("browserSourcePianoUrlEdit"));
        browserSourcePianoUrlEdit->setReadOnly(true);

        browserSourcePianoUrlsLayout->addWidget(browserSourcePianoUrlEdit);

        copyPianoUrlButton = new QPushButton(browserSourceGroup);
        copyPianoUrlButton->setObjectName(QString::fromUtf8("copyPianoUrlButton"));

        browserSourcePianoUrlsLayout->addWidget(copyPianoUrlButton);


        browserSourceLayout->addLayout(browserSourcePianoUrlsLayout);


        verticalLayout->addWidget(browserSourceGroup);

        buttonBox = new QDialogButtonBox(MirrorTargetsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(MirrorTargetsDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), MirrorTargetsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MirrorTargetsDialog);
    } // setupUi

    void retranslateUi(QDialog *MirrorTargetsDialog)
    {
        MirrorTargetsDialog->setWindowTitle(QCoreApplication::translate("MirrorTargetsDialog", "Mirror Targets", nullptr));
        sessionLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "Session ID: \342\200\224", nullptr));
        serversGroup->setTitle(QCoreApplication::translate("MirrorTargetsDialog", "Remote servers", nullptr));
        QTableWidgetItem *___qtablewidgetitem = targetsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MirrorTargetsDialog", "On", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = targetsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MirrorTargetsDialog", "Protocol", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = targetsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MirrorTargetsDialog", "IP Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = targetsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MirrorTargetsDialog", "Port", nullptr));
        addButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Add", nullptr));
        removeButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Remove", nullptr));
        clearButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Clear", nullptr));
        logsGroup->setTitle(QCoreApplication::translate("MirrorTargetsDialog", "Input logs", nullptr));
        binCheck->setText(QCoreApplication::translate("MirrorTargetsDialog", "Input Bin Log", nullptr));
        browseBinButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "...", nullptr));
        fullTextCheck->setText(QCoreApplication::translate("MirrorTargetsDialog", "Input Text Log \342\200\224 Full", nullptr));
        browseFullTextButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "...", nullptr));
        syncTextCheck->setText(QCoreApplication::translate("MirrorTargetsDialog", "Text Log \342\200\224 Sync Headers Only", nullptr));
        browseSyncTextButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "...", nullptr));
        lastNCheck->setText(QCoreApplication::translate("MirrorTargetsDialog", "Last N dashboard", nullptr));
        lastNNLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "N", nullptr));
        lastNPathEdit->setPlaceholderText(QCoreApplication::translate("MirrorTargetsDialog", "base filename (no extension)", nullptr));
        browseLastNButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "...", nullptr));
        browserSourceGroup->setTitle(QCoreApplication::translate("MirrorTargetsDialog", "OBS Browser Source", nullptr));
        browserSourceCheck->setText(QCoreApplication::translate("MirrorTargetsDialog", "Enable localhost browser source server", nullptr));
        browserSourcePortLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "Port", nullptr));
        browserSourceMainUrlLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "Main:", nullptr));
        copyMainUrlButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Copy", nullptr));
        browserSourceStackedUrlLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "Stacked:", nullptr));
        copyStackedUrlButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Copy", nullptr));
        browserSourcePianoUrlLabel->setText(QCoreApplication::translate("MirrorTargetsDialog", "Piano:", nullptr));
        copyPianoUrlButton->setText(QCoreApplication::translate("MirrorTargetsDialog", "Copy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MirrorTargetsDialog: public Ui_MirrorTargetsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIRRORTARGETSDIALOG_H
