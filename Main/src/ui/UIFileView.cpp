#include "UIFileView.h"
#include "ui_UIFileView.h"

UIFileView::UIFileView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIFileView)
{
	ui->setupUi(this);
}

UIFileView::~UIFileView()
{
	delete ui;
}
