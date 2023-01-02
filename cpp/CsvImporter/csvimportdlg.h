// Copyright 2010-2012 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
// $Id$

#ifndef CSVIMPORTDLG_H
#define CSVIMPORTDLG_H

#include <QDialog>

namespace Ui {
class CsvImportDlg;
}

class CsvImportDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CsvImportDlg(QWidget *parent = 0);
    ~CsvImportDlg();
    
private:
    void ProcessLine(const QString& line);

    Ui::CsvImportDlg *ui;
};

#endif // CSVIMPORTDLG_H
