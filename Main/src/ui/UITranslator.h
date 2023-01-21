#pragma once

#include <QDialog>

//////////////////////////////////////////////////////////////////////////////////
class UITranslator : public QDialog {
	Q_OBJECT

public:
	explicit UITranslator( QWidget* parent , ItemModFormID* item );
	~UITranslator();

	/////////////////////////////////////////
	virtual void closeEvent( QCloseEvent* event ) override;

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

