/********************************************************************************
** Form generated from reading UI file 'inputdisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTDISPLAY_H
#define UI_INPUTDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InputDisplay
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QLabel *pianoLabel;
    QLabel *pianoTagLabel;

    void setupUi(QWidget *InputDisplay)
    {
        if (InputDisplay->objectName().isEmpty())
            InputDisplay->setObjectName(QString::fromUtf8("InputDisplay"));
        InputDisplay->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InputDisplay->sizePolicy().hasHeightForWidth());
        InputDisplay->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(InputDisplay);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        graphicsView = new QGraphicsView(InputDisplay);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setEnabled(true);
        graphicsView->setAcceptDrops(false);
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setLineWidth(0);

        verticalLayout->addWidget(graphicsView);

        pianoLabel = new QLabel(InputDisplay);
        pianoLabel->setObjectName(QString::fromUtf8("pianoLabel"));

        verticalLayout->addWidget(pianoLabel);

        pianoTagLabel = new QLabel(InputDisplay);
        pianoTagLabel->setObjectName(QString::fromUtf8("pianoTagLabel"));

        verticalLayout->addWidget(pianoTagLabel);


        retranslateUi(InputDisplay);

        QMetaObject::connectSlotsByName(InputDisplay);
    } // setupUi

    void retranslateUi(QWidget *InputDisplay)
    {
        InputDisplay->setWindowTitle(QCoreApplication::translate("InputDisplay", "My Input Display", nullptr));
        pianoLabel->setText(QString());
        pianoTagLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InputDisplay: public Ui_InputDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTDISPLAY_H
