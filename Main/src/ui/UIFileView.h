#pragma once

#include <QWidget>

namespace Ui {
class UIFileView;
}

class UIFileView : public QWidget
{
	Q_OBJECT
	
public:
	explicit UIFileView(QWidget *parent = nullptr);
	~UIFileView();
	
private:
	Ui::UIFileView *ui;
};

