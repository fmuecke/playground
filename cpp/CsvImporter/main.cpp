// Copyright 2010-2012 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
// $Id$

#include <QApplication>
#include "csvimportdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CsvImportDlg w;
    w.show();
    
    return a.exec();
}
