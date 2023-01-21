#pragma once

#include <QWidget>

class ItemFile;

//////////////////////////////////////////////////////////////////////////////////
class UIVirtualFileView : public QWidget {
	Q_OBJECT

public:
	explicit UIVirtualFileView( QWidget* parent = nullptr );
	~UIVirtualFileView();

signals:
	void signal_itemSelectionChanged( ItemFile* );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

