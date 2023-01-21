#pragma once

#include <QTreeWidget>

enum class ELogType : int {
	Info,
	Warning,
	Error,
};

//////////////////////////////////////////////////////////////////////////////////
class HLogView : public QTreeWidget {
	Q_OBJECT

public:
	HLogView( QWidget* parent = nullptr );

	static void info( QString text );
	static void warning( QString text );
	static void error( QString text );
	static void clear();

signals:
	void signal_addLog( QString text, ELogType logType );
	void signal_clearLog();

private:
	class Impl;
	std::unique_ptr<Impl> impl;

	virtual void resizeEvent( QResizeEvent* ) override;
};
