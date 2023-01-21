#pragma once

#include "HTreeWidget.h"

class TreeViewOverride : public HTreeWidget {
public:
	explicit TreeViewOverride( QWidget* parent = nullptr );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

