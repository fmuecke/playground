#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
	class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

protected:
	void changeEvent(QEvent *e);

private:
	void play_file(unsigned int i, QPushButton* pButton);
	void check_buttons_and_files();

	Ui::Dialog *ui;
	QStringList m_files;
	QPushButton* m_pActiveButton;

private slots:
	void on_pushButton_1_toggled(bool checked);
	void on_pushButton_2_toggled(bool checked);
	void on_pushButton_3_toggled(bool checked);
	void on_pushButton_4_toggled(bool checked);
	void on_pushButton_5_toggled(bool checked);
	void on_pushButton_6_toggled(bool checked);
	void on_pushButton_7_toggled(bool checked);
	void on_pushButton_8_toggled(bool checked);
	void on_pushButton_9_toggled(bool checked);
	void on_pushButton_10_toggled(bool checked);
	void on_pushButton_11_toggled(bool checked);
	void on_pushButton_12_toggled(bool checked);
	void on_pushButton_13_toggled(bool checked);
};

#endif // DIALOG_H
