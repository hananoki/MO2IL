#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewMerge : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2ViewMerge( QWidget* parent = nullptr );
	virtual ~UIMO2ViewMerge() override;

	void initView();

signals:
	void signal_addItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

