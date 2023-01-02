#ifndef DIALOG_H
#define DIALOG_H


#include <QtGui/QDialog>
#include "QMenu"
#include "QAction"
#include "QListWidgetItem"

#include "pesfxcreator.h"

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
	explicit Dialog( PeSfxCreator* creator, QWidget* parent = NULL );
    ~Dialog();

private:
	PeSfxCreator* m_Creator;
    Ui::DialogClass* m_ui;
    QMenu* m_menu;
    QAction* m_actionSetExe;
    //QAction* m_actionAddFolders;
	QAction* m_actionRemove;
	void GetDirContent_( const QString& dir, QStringList& files ) const;
	void UpdateExe_( const QString& fileName );
	void UpdateBaseDirAndList_();

private slots:
	void on_pushButton_create_released();
	void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
	void on_pushButton_dir_released();
    void on_listWidget_customContextMenuRequested(QPoint pos);
    void on_action_setExe();
    //void on_action_addFolders();
	void on_action_remove();
};

#endif // DIALOG_H
