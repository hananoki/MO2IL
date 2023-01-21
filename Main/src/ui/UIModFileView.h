#pragma once

#include <QWidget>

//struct TESRecord;

//////////////////////////////////////////////////////////////////////////////////
class UIModFileView : public QWidget {
	Q_OBJECT

public:
	explicit UIModFileView( QWidget* parent = nullptr );
	~UIModFileView();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

