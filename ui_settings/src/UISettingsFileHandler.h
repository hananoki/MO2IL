#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UISettingsFileHandler : public QWidget {
	Q_OBJECT

public:
	explicit UISettingsFileHandler( QWidget* parent = nullptr );
	~UISettingsFileHandler();

	virtual void showEvent( QShowEvent* event ) override;

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

