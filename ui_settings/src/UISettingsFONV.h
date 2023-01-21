#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFONV : public QWidget {
	Q_OBJECT

public:
	explicit UISettingsFONV( QWidget* parent = nullptr );
	~UISettingsFONV();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

