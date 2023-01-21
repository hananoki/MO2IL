#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFO4 : public QWidget
{
	Q_OBJECT
	
public:
	explicit UISettingsFO4(QWidget *parent = nullptr);
	~UISettingsFO4();
	
private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

