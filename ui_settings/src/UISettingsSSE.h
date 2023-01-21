#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UISettingsSSE : public QWidget {
	Q_OBJECT

public:
	explicit UISettingsSSE( QWidget* parent = nullptr );
	~UISettingsSSE();

	bool eventFilter( QObject* obj, QEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

