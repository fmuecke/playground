/********************************************************************************
** Form generated from reading ui file 'dialog.ui'
**
** Created: Sun 31. May 17:39:34 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogClass
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_main;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_dir;
    QPushButton *pushButton_dir;
    QListWidget *listWidget;
    QLabel *label_fileInfo;
    QGroupBox *groupBox_appToRun;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_exeFile;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_createProcess;
    QRadioButton *radioButton_shellExecute;
    QRadioButton *radioButton_loadLibrary;
    QGroupBox *groupBox_output;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_create;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer;
    QWidget *tab_options;
    QLabel *label;
    QLabel *label_4;

    void setupUi(QDialog *DialogClass)
    {
        if (DialogClass->objectName().isEmpty())
            DialogClass->setObjectName(QString::fromUtf8("DialogClass"));
        DialogClass->setWindowModality(Qt::ApplicationModal);
        DialogClass->setEnabled(true);
        DialogClass->resize(360, 524);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(DialogClass->sizePolicy().hasHeightForWidth());
        DialogClass->setSizePolicy(sizePolicy);
        DialogClass->setContextMenuPolicy(Qt::DefaultContextMenu);
        DialogClass->setAcceptDrops(true);
        DialogClass->setAutoFillBackground(false);
        DialogClass->setSizeGripEnabled(true);
        DialogClass->setModal(true);
        verticalLayout = new QVBoxLayout(DialogClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setMargin(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(DialogClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setContextMenuPolicy(Qt::NoContextMenu);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setTabsClosable(false);
        tab_main = new QWidget();
        tab_main->setObjectName(QString::fromUtf8("tab_main"));
        verticalLayout_3 = new QVBoxLayout(tab_main);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(tab_main);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(5);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit_dir = new QLineEdit(groupBox);
        lineEdit_dir->setObjectName(QString::fromUtf8("lineEdit_dir"));
        lineEdit_dir->setReadOnly(true);

        horizontalLayout_4->addWidget(lineEdit_dir);

        pushButton_dir = new QPushButton(groupBox);
        pushButton_dir->setObjectName(QString::fromUtf8("pushButton_dir"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_dir->sizePolicy().hasHeightForWidth());
        pushButton_dir->setSizePolicy(sizePolicy1);
        pushButton_dir->setLayoutDirection(Qt::LeftToRight);
        pushButton_dir->setCheckable(false);
        pushButton_dir->setDefault(false);
        pushButton_dir->setFlat(false);

        horizontalLayout_4->addWidget(pushButton_dir);


        verticalLayout_4->addLayout(horizontalLayout_4);

        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(150, 200));
        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        listWidget->setAcceptDrops(true);
        listWidget->setFrameShape(QFrame::StyledPanel);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setDragDropMode(QAbstractItemView::DropOnly);
        listWidget->setAlternatingRowColors(true);
        listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget->setSortingEnabled(false);

        verticalLayout_4->addWidget(listWidget);

        label_fileInfo = new QLabel(groupBox);
        label_fileInfo->setObjectName(QString::fromUtf8("label_fileInfo"));
        label_fileInfo->setEnabled(true);
        label_fileInfo->setFrameShape(QFrame::NoFrame);
        label_fileInfo->setScaledContents(false);

        verticalLayout_4->addWidget(label_fileInfo);


        verticalLayout_3->addWidget(groupBox);

        groupBox_appToRun = new QGroupBox(tab_main);
        groupBox_appToRun->setObjectName(QString::fromUtf8("groupBox_appToRun"));
        groupBox_appToRun->setEnabled(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_appToRun);
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setMargin(5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(groupBox_appToRun);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit_exeFile = new QLineEdit(groupBox_appToRun);
        lineEdit_exeFile->setObjectName(QString::fromUtf8("lineEdit_exeFile"));
        lineEdit_exeFile->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit_exeFile);


        verticalLayout_2->addLayout(horizontalLayout);

        label_3 = new QLabel(groupBox_appToRun);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFrameShape(QFrame::NoFrame);
        label_3->setFrameShadow(QFrame::Plain);
        label_3->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        verticalLayout_2->addWidget(label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton_createProcess = new QRadioButton(groupBox_appToRun);
        radioButton_createProcess->setObjectName(QString::fromUtf8("radioButton_createProcess"));
        radioButton_createProcess->setChecked(true);

        horizontalLayout_2->addWidget(radioButton_createProcess);

        radioButton_shellExecute = new QRadioButton(groupBox_appToRun);
        radioButton_shellExecute->setObjectName(QString::fromUtf8("radioButton_shellExecute"));

        horizontalLayout_2->addWidget(radioButton_shellExecute);

        radioButton_loadLibrary = new QRadioButton(groupBox_appToRun);
        radioButton_loadLibrary->setObjectName(QString::fromUtf8("radioButton_loadLibrary"));

        horizontalLayout_2->addWidget(radioButton_loadLibrary);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox_appToRun);

        groupBox_output = new QGroupBox(tab_main);
        groupBox_output->setObjectName(QString::fromUtf8("groupBox_output"));
        groupBox_output->setEnabled(false);
        horizontalLayout_3 = new QHBoxLayout(groupBox_output);
        horizontalLayout_3->setSpacing(5);
        horizontalLayout_3->setMargin(5);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_create = new QPushButton(groupBox_output);
        pushButton_create->setObjectName(QString::fromUtf8("pushButton_create"));

        horizontalLayout_3->addWidget(pushButton_create);

        progressBar = new QProgressBar(groupBox_output);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy2);
        progressBar->setValue(24);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);
        progressBar->setTextDirection(QProgressBar::TopToBottom);
        progressBar->setFormat(QString::fromUtf8("%p%"));

        horizontalLayout_3->addWidget(progressBar);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(groupBox_output);

        tabWidget->addTab(tab_main, QString());
        tab_options = new QWidget();
        tab_options->setObjectName(QString::fromUtf8("tab_options"));
        label = new QLabel(tab_options);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 160, 191, 16));
        tabWidget->addTab(tab_options, QString());

        verticalLayout->addWidget(tabWidget);

        label_4 = new QLabel(DialogClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setEnabled(false);

        verticalLayout->addWidget(label_4);


        retranslateUi(DialogClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogClass);
    } // setupUi

    void retranslateUi(QDialog *DialogClass)
    {
        DialogClass->setWindowTitle(QApplication::translate("DialogClass", "PeSfx Creator", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DialogClass->setToolTip(QApplication::translate("DialogClass", "TOOOL TIP", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        DialogClass->setStatusTip(QApplication::translate("DialogClass", "Status Tip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        DialogClass->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        DialogClass->setStyleSheet(QString());
        groupBox->setTitle(QApplication::translate("DialogClass", "Select Base Directory", 0, QApplication::UnicodeUTF8));
        pushButton_dir->setText(QApplication::translate("DialogClass", "Browse", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listWidget->setToolTip(QApplication::translate("DialogClass", "Use the context menu or drag & drop to add files.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_fileInfo->setStyleSheet(QApplication::translate("DialogClass", "color: rgb(96,96,96);", 0, QApplication::UnicodeUTF8));
        label_fileInfo->setText(QApplication::translate("DialogClass", "0 files selected", 0, QApplication::UnicodeUTF8));
        groupBox_appToRun->setTitle(QApplication::translate("DialogClass", "Application to Start", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogClass", "File", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogClass", "Use the following method to start the application:", 0, QApplication::UnicodeUTF8));
        radioButton_createProcess->setText(QApplication::translate("DialogClass", "CreateProcess", 0, QApplication::UnicodeUTF8));
        radioButton_shellExecute->setText(QApplication::translate("DialogClass", "ShellExecute", 0, QApplication::UnicodeUTF8));
        radioButton_loadLibrary->setText(QApplication::translate("DialogClass", "LoadLibrary", 0, QApplication::UnicodeUTF8));
        groupBox_output->setTitle(QApplication::translate("DialogClass", "Output File", 0, QApplication::UnicodeUTF8));
        pushButton_create->setText(QApplication::translate("DialogClass", "Create", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_main), QApplication::translate("DialogClass", "Create", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogClass", "Ther are no options available atm.", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_options), QApplication::translate("DialogClass", "Options", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogClass", "PeSfx Creator (c) 2009 F.Muecke", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DialogClass);
    } // retranslateUi

};

namespace Ui {
    class DialogClass: public Ui_DialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
