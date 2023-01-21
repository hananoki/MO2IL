#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewMarge : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2ViewMarge( QWidget* parent = nullptr );
	virtual ~UIMO2ViewMarge() override;

	void initView();

signals:
	void signal_addItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

