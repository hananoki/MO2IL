#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UISettingsTool : public QWidget {
	Q_OBJECT

public:
	explicit UISettingsTool( QWidget* parent = nullptr );
	~UISettingsTool();

	bool eventFilter( QObject* obj, QEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

