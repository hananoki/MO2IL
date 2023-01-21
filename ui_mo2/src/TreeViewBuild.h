#pragma once

#include "HTreeWidget.h"

//////////////////////////////////////////////////////////////////////////////////
class TreeViewBuild : public HTreeWidget {
public:
	explicit TreeViewBuild( QWidget* parent = nullptr );

	void setup();
	void build();
	void open( const QString& filePath );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

