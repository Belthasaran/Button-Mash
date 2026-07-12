/********************************************************************************
** Form generated from reading UI file 'mapbuttondialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPBUTTONDIALOG_H
#define UI_MAPBUTTONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MapButtonDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_2;
    QLabel *nameLabel;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_14;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *upButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *rightButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *downButton;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *leftButton;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *startButton;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *selectButton;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *yButton;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *bButton;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *aButton;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *xButton;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *lButton;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *rButton;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;
    QButtonGroup *mappingButtonGroup;

    void setupUi(QDialog *MapButtonDialog)
    {
        if (MapButtonDialog->objectName().isEmpty())
            MapButtonDialog->setObjectName(QString::fromUtf8("MapButtonDialog"));
        MapButtonDialog->resize(521, 422);
        verticalLayout_3 = new QVBoxLayout(MapButtonDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_13 = new QLabel(MapButtonDialog);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/img/Skins/Default/snes.png")));
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_13);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        nameLabel = new QLabel(MapButtonDialog);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(nameLabel);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_14);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(MapButtonDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        upButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup = new QButtonGroup(MapButtonDialog);
        mappingButtonGroup->setObjectName(QString::fromUtf8("mappingButtonGroup"));
        mappingButtonGroup->setExclusive(true);
        mappingButtonGroup->addButton(upButton);
        upButton->setObjectName(QString::fromUtf8("upButton"));
        upButton->setCheckable(false);
        upButton->setFlat(true);

        horizontalLayout->addWidget(upButton);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(MapButtonDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_8);

        rightButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(rightButton);
        rightButton->setObjectName(QString::fromUtf8("rightButton"));
        rightButton->setCheckable(false);
        rightButton->setFlat(true);

        horizontalLayout_2->addWidget(rightButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(MapButtonDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);

        downButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(downButton);
        downButton->setObjectName(QString::fromUtf8("downButton"));
        downButton->setCheckable(false);
        downButton->setFlat(true);

        horizontalLayout_3->addWidget(downButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(MapButtonDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        leftButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(leftButton);
        leftButton->setObjectName(QString::fromUtf8("leftButton"));
        leftButton->setCheckable(false);
        leftButton->setFlat(true);

        horizontalLayout_4->addWidget(leftButton);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(MapButtonDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        startButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(startButton);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setCheckable(false);
        startButton->setFlat(true);

        horizontalLayout_5->addWidget(startButton);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(MapButtonDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_12);

        selectButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(selectButton);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setCheckable(false);
        selectButton->setFlat(true);

        horizontalLayout_6->addWidget(selectButton);


        verticalLayout_2->addLayout(horizontalLayout_6);


        horizontalLayout_13->addLayout(verticalLayout_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(MapButtonDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setTextFormat(Qt::RichText);

        horizontalLayout_7->addWidget(label_7);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        yButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(yButton);
        yButton->setObjectName(QString::fromUtf8("yButton"));
        yButton->setCheckable(false);
        yButton->setFlat(true);

        horizontalLayout_7->addWidget(yButton);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(MapButtonDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setTextFormat(Qt::RichText);

        horizontalLayout_8->addWidget(label_8);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        bButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(bButton);
        bButton->setObjectName(QString::fromUtf8("bButton"));
        bButton->setCheckable(false);
        bButton->setFlat(true);

        horizontalLayout_8->addWidget(bButton);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_9 = new QLabel(MapButtonDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_9->addWidget(label_9);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_13);

        aButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(aButton);
        aButton->setObjectName(QString::fromUtf8("aButton"));
        aButton->setCheckable(false);
        aButton->setFlat(true);

        horizontalLayout_9->addWidget(aButton);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_10 = new QLabel(MapButtonDialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_10->addWidget(label_10);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_4);

        xButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(xButton);
        xButton->setObjectName(QString::fromUtf8("xButton"));
        xButton->setCheckable(false);
        xButton->setFlat(true);

        horizontalLayout_10->addWidget(xButton);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_11 = new QLabel(MapButtonDialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_11->addWidget(label_11);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_5);

        lButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(lButton);
        lButton->setObjectName(QString::fromUtf8("lButton"));
        lButton->setCheckable(false);
        lButton->setFlat(true);

        horizontalLayout_11->addWidget(lButton);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_12 = new QLabel(MapButtonDialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_12->addWidget(label_12);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_6);

        rButton = new QPushButton(MapButtonDialog);
        mappingButtonGroup->addButton(rButton);
        rButton->setObjectName(QString::fromUtf8("rButton"));
        rButton->setCheckable(false);
        rButton->setFlat(true);

        horizontalLayout_12->addWidget(rButton);


        verticalLayout->addLayout(horizontalLayout_12);


        horizontalLayout_13->addLayout(verticalLayout);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_15);


        verticalLayout_3->addLayout(horizontalLayout_13);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(MapButtonDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(MapButtonDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MapButtonDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MapButtonDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MapButtonDialog);
    } // setupUi

    void retranslateUi(QDialog *MapButtonDialog)
    {
        MapButtonDialog->setWindowTitle(QCoreApplication::translate("MapButtonDialog", "Controller Mapping Config", nullptr));
        label_13->setText(QString());
        nameLabel->setText(QCoreApplication::translate("MapButtonDialog", "Controller Name", nullptr));
        label->setText(QCoreApplication::translate("MapButtonDialog", "UP", nullptr));
        upButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_2->setText(QCoreApplication::translate("MapButtonDialog", "RIGHT", nullptr));
        rightButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_3->setText(QCoreApplication::translate("MapButtonDialog", "DOWN", nullptr));
        downButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_4->setText(QCoreApplication::translate("MapButtonDialog", "LEFT", nullptr));
        leftButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_5->setText(QCoreApplication::translate("MapButtonDialog", "Start", nullptr));
        startButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_6->setText(QCoreApplication::translate("MapButtonDialog", "Select", nullptr));
        selectButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_7->setText(QCoreApplication::translate("MapButtonDialog", "<span style=\" color:#005500;\">Y</span>", nullptr));
        yButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_8->setText(QCoreApplication::translate("MapButtonDialog", "<span style=\" color:#515100;\">B</span>", nullptr));
        bButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_9->setText(QCoreApplication::translate("MapButtonDialog", "<span style=\" color:#550000;\">A</span>", nullptr));
        aButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_10->setText(QCoreApplication::translate("MapButtonDialog", "<html><head/><body><p><span style=\" color:#00007f;\">X </span></p></body></html>", nullptr));
        xButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_11->setText(QCoreApplication::translate("MapButtonDialog", "L", nullptr));
        lButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
        label_12->setText(QCoreApplication::translate("MapButtonDialog", "R", nullptr));
        rButton->setText(QCoreApplication::translate("MapButtonDialog", "Click to set", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapButtonDialog: public Ui_MapButtonDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPBUTTONDIALOG_H
