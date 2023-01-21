#pragma once

#include <QDialog>

//////////////////////////////////////////////////////////////////////////////////
class UISettings : public QDialog {
	Q_OBJECT

public:
	explicit UISettings( QWidget* parent = nullptr );
	~UISettings();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

