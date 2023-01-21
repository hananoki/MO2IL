#pragma once

#include <QWidget>

class ItemFile;

//////////////////////////////////////////////////////////////////////////////////
class UIMO2ViewFile : public QWidget {
	Q_OBJECT

public:
	explicit UIMO2ViewFile( QWidget* parent = nullptr );
	~UIMO2ViewFile();

signals:
	void signal_previewItem( ItemFile* item );
	void signal_hideFlame();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

