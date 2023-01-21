#pragma once

#include "HTreeWidget.h"

//////////////////////////////////////////////////////////////////////////////////
class MO2FileTreeView : public HTreeWidget {
	Q_OBJECT
public:
	explicit MO2FileTreeView( QWidget* parent = nullptr );

	void open( const QString& folderPath );

signals:
	void signal_addItem( QTreeWidgetItem* item );
	void signal_addChild( QTreeWidgetItem* item, QTreeWidgetItem* item2 );
	void signal_clearItems();
	void signal_checkedTopFolder();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

