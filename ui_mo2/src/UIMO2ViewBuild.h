#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewBuild : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2ViewBuild( QWidget* parent = nullptr );
	~UIMO2ViewBuild();

	void initView( const QString& filePath );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

