#pragma once

#include <QString>
#include <functional>

class TESForm;
struct TESField;

enum TESStringsType : int {
	Strings,
	DLStrings,
	ILStrings,
};

//////////////////////////////////////////////////////////////////////////////////
class TESStringTable {
public:
	TESStringTable( const QString& filePath );
	virtual ~TESStringTable();

	QString operator[]( quint32 hash );

	QList<quint32> keys();

	static TESStringsType stringsType( TESForm* form, TESField * field );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

