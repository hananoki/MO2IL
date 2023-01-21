#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIToolBar : public QWidget {
	Q_OBJECT

public:
	explicit UIToolBar( QWidget* parent = nullptr );
	~UIToolBar();

signals:
	void signal_refresh();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

