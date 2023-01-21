#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIOverrideCheck : public QWidget {
	Q_OBJECT

public:
	explicit UIOverrideCheck( QWidget* parent = nullptr );
	virtual ~UIOverrideCheck() override;

signals:
	// アイテム追加
	void signal_addItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

