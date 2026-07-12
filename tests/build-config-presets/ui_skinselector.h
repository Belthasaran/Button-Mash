/********************************************************************************
** Form generated from reading UI file 'skinselector.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SKINSELECTOR_H
#define UI_SKINSELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SkinSelector
{
public:
    QWidget *centralWidget;
    QVBoxLayout *mainVerticalLayout;
    QFrame *configFrame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *presetsRowLayout;
    QLabel *presetsLabel;
    QComboBox *presetComboBox;
    QPushButton *presetSwitchButton;
    QPushButton *presetNewButton;
    QPushButton *presetOverwriteButton;
    QPushButton *presetDeleteButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *sourceLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *changeSourceButton;
    QHBoxLayout *mirrorRowLayout;
    QCheckBox *shareMirrorCheckBox;
    QSpacerItem *mirrorSpacer;
    QPushButton *mirrorTargetsButton;
    QHBoxLayout *triggersRowLayout;
    QCheckBox *inputTriggersCheckBox;
    QSpacerItem *triggersSpacer;
    QPushButton *configureTriggersButton;
    QHBoxLayout *debugRowLayout;
    QCheckBox *debugConsoleCheckBox;
    QSpacerItem *debugSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *skinPathEdit;
    QPushButton *skinPathButton;
    QGraphicsView *previewGraphicView;
    QHBoxLayout *skinListsLayout;
    QVBoxLayout *regularSkinColumnLayout;
    QLabel *label_2;
    QListView *skinListView;
    QListView *subSkinListView;
    QHBoxLayout *pianoRowLayout;
    QCheckBox *pianoCheckBox;
    QListView *pianoSkinListView;
    QLabel *statusLabel;
    QHBoxLayout *bottomButtonsLayout;
    QSpacerItem *bottomHorizontalSpacer;
    QPushButton *skinEditorButton;
    QPushButton *startButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SkinSelector)
    {
        if (SkinSelector->objectName().isEmpty())
            SkinSelector->setObjectName(QString::fromUtf8("SkinSelector"));
        SkinSelector->resize(760, 700);
        SkinSelector->setMinimumSize(QSize(760, 700));
        centralWidget = new QWidget(SkinSelector);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mainVerticalLayout = new QVBoxLayout(centralWidget);
        mainVerticalLayout->setSpacing(8);
        mainVerticalLayout->setContentsMargins(11, 11, 11, 11);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(12, 12, 12, 12);
        configFrame = new QFrame(centralWidget);
        configFrame->setObjectName(QString::fromUtf8("configFrame"));
        configFrame->setFrameShape(QFrame::StyledPanel);
        configFrame->setMinimumSize(QSize(0, 200));
        verticalLayout = new QVBoxLayout(configFrame);
        verticalLayout->setSpacing(8);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        presetsRowLayout = new QHBoxLayout();
        presetsRowLayout->setSpacing(6);
        presetsRowLayout->setObjectName(QString::fromUtf8("presetsRowLayout"));
        presetsLabel = new QLabel(configFrame);
        presetsLabel->setObjectName(QString::fromUtf8("presetsLabel"));

        presetsRowLayout->addWidget(presetsLabel);

        presetComboBox = new QComboBox(configFrame);
        presetComboBox->setObjectName(QString::fromUtf8("presetComboBox"));
        presetComboBox->setMinimumSize(QSize(160, 0));

        presetsRowLayout->addWidget(presetComboBox);

        presetSwitchButton = new QPushButton(configFrame);
        presetSwitchButton->setObjectName(QString::fromUtf8("presetSwitchButton"));

        presetsRowLayout->addWidget(presetSwitchButton);

        presetNewButton = new QPushButton(configFrame);
        presetNewButton->setObjectName(QString::fromUtf8("presetNewButton"));

        presetsRowLayout->addWidget(presetNewButton);

        presetOverwriteButton = new QPushButton(configFrame);
        presetOverwriteButton->setObjectName(QString::fromUtf8("presetOverwriteButton"));

        presetsRowLayout->addWidget(presetOverwriteButton);

        presetDeleteButton = new QPushButton(configFrame);
        presetDeleteButton->setObjectName(QString::fromUtf8("presetDeleteButton"));

        presetsRowLayout->addWidget(presetDeleteButton);


        verticalLayout->addLayout(presetsRowLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(configFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        sourceLabel = new QLabel(configFrame);
        sourceLabel->setObjectName(QString::fromUtf8("sourceLabel"));

        horizontalLayout_2->addWidget(sourceLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        changeSourceButton = new QPushButton(configFrame);
        changeSourceButton->setObjectName(QString::fromUtf8("changeSourceButton"));

        horizontalLayout_2->addWidget(changeSourceButton);


        verticalLayout->addLayout(horizontalLayout_2);

        mirrorRowLayout = new QHBoxLayout();
        mirrorRowLayout->setSpacing(6);
        mirrorRowLayout->setObjectName(QString::fromUtf8("mirrorRowLayout"));
        shareMirrorCheckBox = new QCheckBox(configFrame);
        shareMirrorCheckBox->setObjectName(QString::fromUtf8("shareMirrorCheckBox"));

        mirrorRowLayout->addWidget(shareMirrorCheckBox);

        mirrorSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mirrorRowLayout->addItem(mirrorSpacer);

        mirrorTargetsButton = new QPushButton(configFrame);
        mirrorTargetsButton->setObjectName(QString::fromUtf8("mirrorTargetsButton"));

        mirrorRowLayout->addWidget(mirrorTargetsButton);


        verticalLayout->addLayout(mirrorRowLayout);

        triggersRowLayout = new QHBoxLayout();
        triggersRowLayout->setSpacing(6);
        triggersRowLayout->setObjectName(QString::fromUtf8("triggersRowLayout"));
        inputTriggersCheckBox = new QCheckBox(configFrame);
        inputTriggersCheckBox->setObjectName(QString::fromUtf8("inputTriggersCheckBox"));

        triggersRowLayout->addWidget(inputTriggersCheckBox);

        triggersSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        triggersRowLayout->addItem(triggersSpacer);

        configureTriggersButton = new QPushButton(configFrame);
        configureTriggersButton->setObjectName(QString::fromUtf8("configureTriggersButton"));

        triggersRowLayout->addWidget(configureTriggersButton);


        verticalLayout->addLayout(triggersRowLayout);

        debugRowLayout = new QHBoxLayout();
        debugRowLayout->setSpacing(6);
        debugRowLayout->setObjectName(QString::fromUtf8("debugRowLayout"));
        debugConsoleCheckBox = new QCheckBox(configFrame);
        debugConsoleCheckBox->setObjectName(QString::fromUtf8("debugConsoleCheckBox"));

        debugRowLayout->addWidget(debugConsoleCheckBox);

        debugSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        debugRowLayout->addItem(debugSpacer);


        verticalLayout->addLayout(debugRowLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(configFrame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        skinPathEdit = new QLineEdit(configFrame);
        skinPathEdit->setObjectName(QString::fromUtf8("skinPathEdit"));
        skinPathEdit->setReadOnly(true);

        horizontalLayout->addWidget(skinPathEdit);

        skinPathButton = new QPushButton(configFrame);
        skinPathButton->setObjectName(QString::fromUtf8("skinPathButton"));

        horizontalLayout->addWidget(skinPathButton);


        verticalLayout->addLayout(horizontalLayout);


        mainVerticalLayout->addWidget(configFrame);

        previewGraphicView = new QGraphicsView(centralWidget);
        previewGraphicView->setObjectName(QString::fromUtf8("previewGraphicView"));
        previewGraphicView->setMinimumSize(QSize(0, 100));
        previewGraphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        previewGraphicView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        mainVerticalLayout->addWidget(previewGraphicView);

        skinListsLayout = new QHBoxLayout();
        skinListsLayout->setSpacing(6);
        skinListsLayout->setObjectName(QString::fromUtf8("skinListsLayout"));
        regularSkinColumnLayout = new QVBoxLayout();
        regularSkinColumnLayout->setSpacing(6);
        regularSkinColumnLayout->setObjectName(QString::fromUtf8("regularSkinColumnLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        regularSkinColumnLayout->addWidget(label_2);

        skinListView = new QListView(centralWidget);
        skinListView->setObjectName(QString::fromUtf8("skinListView"));
        skinListView->setMinimumSize(QSize(0, 160));

        regularSkinColumnLayout->addWidget(skinListView);


        skinListsLayout->addLayout(regularSkinColumnLayout);

        subSkinListView = new QListView(centralWidget);
        subSkinListView->setObjectName(QString::fromUtf8("subSkinListView"));
        subSkinListView->setEnabled(true);
        subSkinListView->setMinimumSize(QSize(0, 160));

        skinListsLayout->addWidget(subSkinListView);


        mainVerticalLayout->addLayout(skinListsLayout);

        pianoRowLayout = new QHBoxLayout();
        pianoRowLayout->setSpacing(6);
        pianoRowLayout->setObjectName(QString::fromUtf8("pianoRowLayout"));
        pianoCheckBox = new QCheckBox(centralWidget);
        pianoCheckBox->setObjectName(QString::fromUtf8("pianoCheckBox"));
        pianoCheckBox->setEnabled(true);

        pianoRowLayout->addWidget(pianoCheckBox);

        pianoSkinListView = new QListView(centralWidget);
        pianoSkinListView->setObjectName(QString::fromUtf8("pianoSkinListView"));
        pianoSkinListView->setEnabled(false);
        pianoSkinListView->setMinimumSize(QSize(0, 100));

        pianoRowLayout->addWidget(pianoSkinListView);


        mainVerticalLayout->addLayout(pianoRowLayout);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setMinimumSize(QSize(0, 40));
        statusLabel->setWordWrap(true);

        mainVerticalLayout->addWidget(statusLabel);

        bottomButtonsLayout = new QHBoxLayout();
        bottomButtonsLayout->setSpacing(6);
        bottomButtonsLayout->setObjectName(QString::fromUtf8("bottomButtonsLayout"));
        bottomHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomButtonsLayout->addItem(bottomHorizontalSpacer);

        skinEditorButton = new QPushButton(centralWidget);
        skinEditorButton->setObjectName(QString::fromUtf8("skinEditorButton"));
        skinEditorButton->setMinimumSize(QSize(120, 36));

        bottomButtonsLayout->addWidget(skinEditorButton);

        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(false);
        startButton->setMinimumSize(QSize(120, 36));

        bottomButtonsLayout->addWidget(startButton);


        mainVerticalLayout->addLayout(bottomButtonsLayout);

        SkinSelector->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SkinSelector);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SkinSelector->setStatusBar(statusBar);

        retranslateUi(SkinSelector);

        QMetaObject::connectSlotsByName(SkinSelector);
    } // setupUi

    void retranslateUi(QMainWindow *SkinSelector)
    {
        SkinSelector->setWindowTitle(QCoreApplication::translate("SkinSelector", "Button Mash", nullptr));
        presetsLabel->setText(QCoreApplication::translate("SkinSelector", "Config Presets", nullptr));
        presetSwitchButton->setText(QCoreApplication::translate("SkinSelector", "Switch", nullptr));
        presetNewButton->setText(QCoreApplication::translate("SkinSelector", "New", nullptr));
        presetOverwriteButton->setText(QCoreApplication::translate("SkinSelector", "Overwrite", nullptr));
        presetDeleteButton->setText(QCoreApplication::translate("SkinSelector", "Delete", nullptr));
        label_3->setText(QCoreApplication::translate("SkinSelector", "Source:", nullptr));
        sourceLabel->setText(QCoreApplication::translate("SkinSelector", "<html><head/><body><p><span style=\" font-weight:600;\">SNES Classic</span></p></body></html>", nullptr));
        changeSourceButton->setText(QCoreApplication::translate("SkinSelector", "Change source", nullptr));
        shareMirrorCheckBox->setText(QCoreApplication::translate("SkinSelector", "Share or Mirror Inputs", nullptr));
        mirrorTargetsButton->setText(QCoreApplication::translate("SkinSelector", "Mirror Targets", nullptr));
        inputTriggersCheckBox->setText(QCoreApplication::translate("SkinSelector", "Enable input triggers", nullptr));
        configureTriggersButton->setText(QCoreApplication::translate("SkinSelector", "Configure Triggers", nullptr));
        debugConsoleCheckBox->setText(QCoreApplication::translate("SkinSelector", "Debug console output", nullptr));
        label->setText(QCoreApplication::translate("SkinSelector", "Skin folder", nullptr));
        skinPathButton->setText(QCoreApplication::translate("SkinSelector", "...", nullptr));
        label_2->setText(QCoreApplication::translate("SkinSelector", "Regular skins", nullptr));
        pianoCheckBox->setText(QCoreApplication::translate("SkinSelector", "Piano display Skin", nullptr));
        statusLabel->setText(QCoreApplication::translate("SkinSelector", "status label", nullptr));
        skinEditorButton->setText(QCoreApplication::translate("SkinSelector", "Skin Editor", nullptr));
        startButton->setText(QCoreApplication::translate("SkinSelector", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SkinSelector: public Ui_SkinSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SKINSELECTOR_H
