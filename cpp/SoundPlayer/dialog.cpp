#include "dialog.h"
#include "ui_dialog.h"

#include <QFile>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Dialog)
	, m_files()
	, m_pActiveButton(nullptr)
{
	ui->setupUi(this);

	// init sound files
	m_files.push_back("sounds\\Wecker.wav");
	m_files.push_back("sounds\\Sonnenschein.wav");
	m_files.push_back("sounds\\Zaehneputzen.wav");
	m_files.push_back("sounds\\Rasierer.wav");
	m_files.push_back("sounds\\Auto Start.wav");
	m_files.push_back("sounds\\Tastatur1.wav");
	m_files.push_back("sounds\\Banana Boat.wav");
	m_files.push_back("sounds\\Fliegen.wav");
	m_files.push_back("sounds\\Tastatur2.wav");
	m_files.push_back("sounds\\Telefon (alt).wav");
	m_files.push_back("sounds\\Im Wagen vor.wav");
	m_files.push_back("sounds\\Haushalt.wav");
	m_files.push_back("sounds\\Schlaflied.wav");

	check_buttons_and_files();
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Dialog::check_buttons_and_files()
{
	QList<QPushButton*> buttons;
	buttons.push_back(ui->pushButton_1);
	buttons.push_back(ui->pushButton_2);
	buttons.push_back(ui->pushButton_3);
	buttons.push_back(ui->pushButton_4);
	buttons.push_back(ui->pushButton_5);
	buttons.push_back(ui->pushButton_6);
	buttons.push_back(ui->pushButton_7);
	buttons.push_back(ui->pushButton_8);
	buttons.push_back(ui->pushButton_9);
	buttons.push_back(ui->pushButton_10);
	buttons.push_back(ui->pushButton_11);
	buttons.push_back(ui->pushButton_12);
	buttons.push_back(ui->pushButton_13);

	for( int i(0); i<buttons.size(); ++i )
	{
		QString name = m_files[i];
		name = name.mid(7, name.length()-7-4);
		buttons[i]->setText(name);

		if( m_files.size() > i && QFile::exists(m_files[i]) )
		{
			buttons[i]->setEnabled(true);
		}
		else
		{
			buttons[i]->setEnabled(false);
		}
	}
}

void Dialog::play_file(unsigned int i, QPushButton* pButton)
{
	if( pButton->isChecked() )
	{
		if( m_pActiveButton )
			m_pActiveButton->setChecked(false);

		::PlaySound((LPCTSTR)m_files[i].utf16(), NULL, SND_FILENAME|SND_ASYNC);
		m_pActiveButton = pButton;
	}
	else
	{
		::PlaySound(NULL, NULL, SND_FILENAME);
		m_pActiveButton = nullptr;
	}
}

void Dialog::on_pushButton_1_toggled(bool checked)
{
	play_file(0, ui->pushButton_1);
}

void Dialog::on_pushButton_2_toggled(bool checked)
{
	play_file(1, ui->pushButton_2);
}

void Dialog::on_pushButton_3_toggled(bool checked)
{
	play_file(2, ui->pushButton_3);
}

void Dialog::on_pushButton_4_toggled(bool checked)
{
	play_file(3, ui->pushButton_4);
}

void Dialog::on_pushButton_5_toggled(bool checked)
{
	play_file(4, ui->pushButton_5);
}

void Dialog::on_pushButton_6_toggled(bool checked)
{
	play_file(5, ui->pushButton_6);
}

void Dialog::on_pushButton_7_toggled(bool checked)
{
	play_file(6, ui->pushButton_7);
}

void Dialog::on_pushButton_8_toggled(bool checked)
{
	play_file(7, ui->pushButton_8);
}

void Dialog::on_pushButton_9_toggled(bool checked)
{
	play_file(8, ui->pushButton_9);
}

void Dialog::on_pushButton_10_toggled(bool checked)
{
	play_file(9, ui->pushButton_10);
}

void Dialog::on_pushButton_11_toggled(bool checked)
{
	play_file(10, ui->pushButton_11);
}

void Dialog::on_pushButton_12_toggled(bool checked)
{
	play_file(11, ui->pushButton_12);
}

void Dialog::on_pushButton_13_toggled(bool checked)
{
	play_file(12, ui->pushButton_13);
}

