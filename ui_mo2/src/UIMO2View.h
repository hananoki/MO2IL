#pragma once

#include <QWidget>

class UIMO2ViewFile;
class ItemFolderMO2View;

//////////////////////////////////////////////////////////////////////////////////
class UIMO2View : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2View( QWidget* parent = nullptr );
	~UIMO2View();

	//bool eventFilter( QObject* obj, QEvent* event );

	HTreeWidget* treeView();

	UIMO2ViewFile* mo2File();

	QList<ItemFolderMO2View*> modFolderItemByChecked();

signals:
	// アイテム追加
	void signal_addItem( QTreeWidgetItem* item );
	void signal_makeTreeComplete();
	void signal_itemSelectionChanged( ItemFolderMO2View* );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

