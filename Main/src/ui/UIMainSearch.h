#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIMainSearch : public QWidget {
	Q_OBJECT

public:
	explicit UIMainSearch( QWidget* parent = nullptr );
	~UIMainSearch();

signals:
	// アイテム追加
	void signal_addItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

