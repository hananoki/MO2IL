#pragma once

#include <QWidget>

namespace Ui {
class UIItemFileRoot;
}

class UIItemFileRoot : public QWidget
{
	Q_OBJECT
	
public:
	explicit UIItemFileRoot(QWidget *parent = nullptr);
	~UIItemFileRoot();

	void setText(const QString& txt);
	
private:
	Ui::UIItemFileRoot *ui;
};

