#pragma once

#include <QWidget>

#include "TESFile.h"

//////////////////////////////////////////////////////////////////////////////////
class UIModListView : public QWidget {
	Q_OBJECT

public:
	explicit UIModListView( QWidget* parent = nullptr );
	~UIModListView();

	ItemModFormID* currentItem();

	void selectModRoot( const QString& modFilePath );

signals:
	void signal_itemSelectionChanged( TESForm* m_formData );

	// アイテム追加
	void signal_addItem( QTreeWidgetItem* item );
	void signal_addItem2( QTreeWidgetItem* item, QList<QTreeWidgetItem*>* child );

	// Modリスト構築が完了した
	void signal_completeModList();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

