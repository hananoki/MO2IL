#pragma once

#include <QApplication>

#define qtApp (App::instance)
#define g_fojpUtility (qtApp->fojpUtility())

class FojpUtility;

//////////////////////////////////////////////////////////////////////////////////
class App : public QApplication {
	Q_OBJECT

public:
	static App* instance;

	App( int& argc, char** argv, int = ApplicationFlags );
	virtual ~App();

	void play( const QString& fullPath, bool audio = true );
	void stop();

	FojpUtility& fojpUtility();

signals:

public:
	class Impl;
	std::unique_ptr<Impl> impl;
};

