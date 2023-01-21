#pragma once

#include "HComboBox.h"

//////////////////////////////////////////////////////////////////////////////////
class ComboBoxFileHandler : public HComboBox {
	Q_OBJECT
public:
	explicit ComboBoxFileHandler( QWidget* parent = 0 );

	void makeItem( bool init =false);

	virtual void bind( QString* t, QObject* config = nullptr ) override;

	QString* bindString;
};

