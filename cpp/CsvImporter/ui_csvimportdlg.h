/********************************************************************************
** Form generated from reading UI file 'csvimportdlg.ui'
**
** Created: Tue 22. May 01:24:54 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CSVIMPORTDLG_H
#define UI_CSVIMPORTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CsvImportDlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTreeWidget *treeWidget;

    void setupUi(QDialog *CsvImportDlg)
    {
        if (CsvImportDlg->objectName().isEmpty())
            CsvImportDlg->setObjectName(QString::fromUtf8("CsvImportDlg"));
        CsvImportDlg->resize(459, 331);
        verticalLayout = new QVBoxLayout(CsvImportDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CsvImportDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        treeWidget = new QTreeWidget(CsvImportDlg);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setRootIsDecorated(false);
        treeWidget->setUniformRowHeights(true);
        treeWidget->setItemsExpandable(false);
        treeWidget->setSortingEnabled(true);
        treeWidget->setExpandsOnDoubleClick(false);
        treeWidget->header()->setDefaultSectionSize(110);

        verticalLayout->addWidget(treeWidget);


        retranslateUi(CsvImportDlg);

        QMetaObject::connectSlotsByName(CsvImportDlg);
    } // setupUi

    void retranslateUi(QDialog *CsvImportDlg)
    {
        CsvImportDlg->setWindowTitle(QApplication::translate("CsvImportDlg", "CsvImportDlg", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CsvImportDlg", "Imported fighters", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("CsvImportDlg", "last", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("CsvImportDlg", "first", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("CsvImportDlg", "club", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("CsvImportDlg", "weight", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CsvImportDlg: public Ui_CsvImportDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSVIMPORTDLG_H
