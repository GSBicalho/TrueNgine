/********************************************************************************
** Form generated from reading UI file 'PropertiesWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIESWINDOW_H
#define UI_PROPERTIESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PropertiesWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionSet_Cut_Number;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QScrollArea *mainScrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOptions;

    void setupUi(QMainWindow *PropertiesWindowClass)
    {
        if (PropertiesWindowClass->objectName().isEmpty())
            PropertiesWindowClass->setObjectName(QStringLiteral("PropertiesWindowClass"));
        PropertiesWindowClass->resize(300, 500);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PropertiesWindowClass->sizePolicy().hasHeightForWidth());
        PropertiesWindowClass->setSizePolicy(sizePolicy);
        PropertiesWindowClass->setMinimumSize(QSize(300, 0));
        PropertiesWindowClass->setMaximumSize(QSize(300, 16777215));
        actionOpen = new QAction(PropertiesWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSet_Cut_Number = new QAction(PropertiesWindowClass);
        actionSet_Cut_Number->setObjectName(QStringLiteral("actionSet_Cut_Number"));
        centralWidget = new QWidget(PropertiesWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        mainScrollArea = new QScrollArea(centralWidget);
        mainScrollArea->setObjectName(QStringLiteral("mainScrollArea"));
        mainScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 298, 477));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mainScrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(mainScrollArea);

        PropertiesWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PropertiesWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 300, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        PropertiesWindowClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuFile->addAction(actionOpen);
        menuOptions->addAction(actionSet_Cut_Number);

        retranslateUi(PropertiesWindowClass);

        QMetaObject::connectSlotsByName(PropertiesWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *PropertiesWindowClass)
    {
        PropertiesWindowClass->setWindowTitle(QApplication::translate("PropertiesWindowClass", "PropertiesWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("PropertiesWindowClass", "Open", Q_NULLPTR));
        actionSet_Cut_Number->setText(QApplication::translate("PropertiesWindowClass", "Set Cut Number", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("PropertiesWindowClass", "File", Q_NULLPTR));
        menuOptions->setTitle(QApplication::translate("PropertiesWindowClass", "Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWindowClass: public Ui_PropertiesWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWINDOW_H
