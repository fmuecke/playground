#include "dialog.h"
#include "ui_dialog.h"
#include "QFileDialog"
#include "QString"
#include "QMessagebox"

Dialog::Dialog( PeSfxCreator* creator, QWidget* parent )
	: m_Creator(creator)
	, QDialog(parent)
    , m_ui(new Ui::DialogClass)
    , m_menu(NULL)
    , m_actionSetExe(NULL)
{
    m_ui->setupUi(this);

    // menu actions
	m_actionSetExe = new QAction( tr("Set as start program"), this );
	m_actionRemove = new QAction( tr("Remove"), this );

	connect( m_actionSetExe, SIGNAL(triggered()), this, SLOT(on_action_setExe()) );
	connect( m_actionRemove, SIGNAL(triggered()), this, SLOT(on_action_remove()) );
    m_menu = new QMenu( m_ui->listWidget );
	m_menu->addAction( m_actionSetExe );
	m_menu->addSeparator();
	m_menu->addAction( m_actionRemove );
	m_menu->addSeparator();
    m_ui->progressBar->setHidden( true );
	UpdateBaseDirAndList_();
}

Dialog::~Dialog()
{
    delete m_menu;
    delete m_ui;
}

void Dialog::on_listWidget_customContextMenuRequested(QPoint pos)
{
	QPoint parent_pos = m_ui->listWidget->pos();
	QWidget::mapFrom( this, pos );
	if( !m_ui->listWidget->selectedItems().isEmpty() )
		m_menu->popup( QWidget::mapToGlobal( pos ) + parent_pos );
}

//----------------------------
// on_pushButton_dir_released
//----------------------------
void Dialog::on_pushButton_dir_released()
{
	QString pathStr = QFileDialog::getExistingDirectory( NULL );
    if( !pathStr.isEmpty() )
    {
		m_Creator->SetBaseDir( pathStr );
		UpdateBaseDirAndList_();
   }
}

//------------------
// on_action_remove
//------------------
void Dialog::on_action_remove()
{
	// get all items
	QList<QListWidgetItem*> items = m_ui->listWidget->selectedItems();
	QList<QListWidgetItem*>::const_iterator iter;
	for( iter = items.begin(); iter!=items.end(); ++iter )
	{
		int row = m_ui->listWidget->row( *iter );
		m_Creator->RemoveFile( m_ui->listWidget->item( row )->text() );
		// remove exe name if file is going to be removed
		if( m_ui->listWidget->item( row )->text() == m_ui->lineEdit_exeFile->text() )
			UpdateExe_( "" );
	}
	m_ui->listWidget->clear();
	m_ui->listWidget->addItems( m_Creator->GetFiles() );
	// update file info label
	m_ui->label_fileInfo->setText( QString( "%1 files selected" ).arg( m_ui->listWidget->count() ) );
}



// CREATE
void Dialog::on_pushButton_create_released()
{
	QString outFileName = QFileDialog::getSaveFileName( NULL, QString(), QString(), "Executables (*.exe)");
	if( !outFileName.isEmpty() )
	{
		m_Creator->SetOutputFile( outFileName );
		m_ui->progressBar->setHidden(false);
		try
		{
			m_Creator->Create( this );
		}
		catch(...)
		{
			QMessageBox::critical( this, "PeSfxCreator", "error occured creating output file" );
			m_ui->progressBar->setHidden(true);
		}
	}
}

// SET EXE
void Dialog::on_action_setExe()
{
	UpdateExe_( m_ui->listWidget->currentItem()->text() );
}
void Dialog::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
	UpdateExe_( item->text() );
}
void Dialog::UpdateExe_( const QString& fileName )
{
	m_ui->groupBox_appToRun->setEnabled( !fileName.isEmpty() );
	m_ui->lineEdit_exeFile->setText( fileName );
	m_ui->groupBox_output->setEnabled( !fileName.isEmpty() );
}

void Dialog::UpdateBaseDirAndList_()
{
	m_ui->listWidget->clear();
	m_ui->lineEdit_dir->setText( m_Creator->GetBaseDir() );
	m_ui->listWidget->addItems( m_Creator->GetFiles() );
	m_ui->label_fileInfo->setText( QString( "%1 files selected" ).arg( m_ui->listWidget->count() ) );
}
