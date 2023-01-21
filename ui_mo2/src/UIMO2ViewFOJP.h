#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewFOJP : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2ViewFOJP( QWidget* parent = nullptr );
	~UIMO2ViewFOJP();

	bool eventFilter( QObject* obj, QEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

