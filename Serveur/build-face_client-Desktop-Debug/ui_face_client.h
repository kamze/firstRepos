/********************************************************************************
** Form generated from reading UI file 'face_client.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACE_CLIENT_H
#define UI_FACE_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_face_client
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *face_client)
    {
        if (face_client->objectName().isEmpty())
            face_client->setObjectName(QStringLiteral("face_client"));
        face_client->resize(400, 300);
        menuBar = new QMenuBar(face_client);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        face_client->setMenuBar(menuBar);
        mainToolBar = new QToolBar(face_client);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        face_client->addToolBar(mainToolBar);
        centralWidget = new QWidget(face_client);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        face_client->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(face_client);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        face_client->setStatusBar(statusBar);

        retranslateUi(face_client);

        QMetaObject::connectSlotsByName(face_client);
    } // setupUi

    void retranslateUi(QMainWindow *face_client)
    {
        face_client->setWindowTitle(QApplication::translate("face_client", "face_client", 0));
    } // retranslateUi

};

namespace Ui {
    class face_client: public Ui_face_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACE_CLIENT_H
