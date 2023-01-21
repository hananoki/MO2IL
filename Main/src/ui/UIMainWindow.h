#pragma once

#include <QMainWindow>

#define qtWindow (UIMainWindow::instance)

class UIModListView;
class UIVirtualFileView;
class UIToolBar;
class UIMO2ViewFile;
class UIMO2View;

//////////////////////////////////////////////////////////////////////////////////
class UIMainWindow : public QMainWindow {
	Q_OBJECT

public:
	static UIMainWindow* instance;

	UIMainWindow( QWidget* parent = nullptr );
	~UIMainWindow();

	/////////////////////////////////////////
	void start();

	/////////////////////////////////////////
	virtual void closeEvent( QCloseEvent* event ) override;

	UIModListView* modListView();

	UIMO2View* mo2View();
	UIMO2ViewFile* mo2View_mo2File();
	//UIVirtualFileView* fileView();
	QTabWidget* tabWidget();
	UIToolBar* toolBar();

	HTreeWidget* mo2ViewTreeView();

signals:
	void signal_start();
	void signal_closeWindow();
	//void signal_changePanel( EPanelMode panelMode );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};
