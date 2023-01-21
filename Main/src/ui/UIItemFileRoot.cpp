#include "UIItemFileRoot.h"
#include "ui_UIItemFileRoot.h"

UIItemFileRoot::UIItemFileRoot(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UIItemFileRoot)
{
	ui->setupUi(this);
}

UIItemFileRoot::~UIItemFileRoot()
{
	delete ui;
}

void UIItemFileRoot::setText( const QString& txt ) {
	ui->label->setText( txt );
}
