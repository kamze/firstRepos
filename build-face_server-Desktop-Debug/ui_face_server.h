/********************************************************************************
** Form generated from reading UI file 'face_server.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACE_SERVER_H
#define UI_FACE_SERVER_H

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

class Ui_face_server
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *face_server)
    {
        if (face_server->objectName().isEmpty())
            face_server->setObjectName(QStringLiteral("face_server"));
        face_server->resize(445, 339);
        centralWidget = new QWidget(face_server);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        face_server->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(face_server);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 445, 27));
        face_server->setMenuBar(menuBar);
        mainToolBar = new QToolBar(face_server);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        face_server->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(face_server);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        face_server->setStatusBar(statusBar);

        retranslateUi(face_server);

        QMetaObject::connectSlotsByName(face_server);
    } // setupUi

    void retranslateUi(QMainWindow *face_server)
    {
        face_server->setWindowTitle(QApplication::translate("face_server", "face_server", 0));
    } // retranslateUi

};

namespace Ui {
    class face_server: public Ui_face_server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACE_SERVER_H
