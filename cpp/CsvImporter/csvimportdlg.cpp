// Copyright 2010-2012 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
// $Id$

#include "csvimportdlg.h"
#include "ui_csvimportdlg.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

CsvImportDlg::CsvImportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CsvImportDlg)
{
    ui->setupUi(this);

    QFileDialog fileDlg(
                this,
                "Select CSV file to import data from",
                "",
                "CSV files (*.csv)");
    if (QDialog::Accepted == fileDlg.exec()
            && !fileDlg.selectedFiles().empty())
    {
        QFile file(fileDlg.selectedFiles()[0]);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString line = in.readLine();

            // skip first line (headings)
            line = in.readLine();

            while (!line.isNull())
            {
                ProcessLine(line);
                line = in.readLine();
            }

            file.close();
        }
    }
}

CsvImportDlg::~CsvImportDlg()
{
    delete ui;
}

void CsvImportDlg::ProcessLine(const QString& line)
{
    QStringList splitted = line.split(';');
    if (splitted.size() >= 4)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(splitted);
        ui->treeWidget->addTopLevelItem(item);
    }
}
