#pragma once

#include <QMap>
#include "Enums.h"
#include "IConfigChanged.h"

using TMap = QMap<QString, QString>;

#define CONFIG_VALUE(type,editorName) type editorName;
#define CONFIG_VALUE_INIT(type,editorName,ini) type editorName = ini;

#define CONFIG_VALUE_LIST \
	CONFIG_VALUE( QPoint, pos ) \
	CONFIG_VALUE( QSize, size ) \
	CONFIG_VALUE( QPoint, transPos ) \
	CONFIG_VALUE( QSize, transSize ) \
	CONFIG_VALUE( int, tabIndex ) \
	CONFIG_VALUE( EGameTitle, targetGameTitle ) \
	CONFIG_VALUE( int, selectSettingsNo ) \
	CONFIG_VALUE( QString, seleectMO2ProfileName ) \
	CONFIG_VALUE( QString, fo4MO2Path ) \
	CONFIG_VALUE( QString, fo4CKPath ) \
	CONFIG_VALUE( QString, fonvMO2Path ) \
	CONFIG_VALUE( QString, fonvFOJPTextPath ) \
	CONFIG_VALUE( QString, fonvXMLPath ) \
	CONFIG_VALUE( QString, sseCKPath ) \
	CONFIG_VALUE( QString, sseMO2Path ) \
	CONFIG_VALUE( QStringList, ssePapyrusImportPaths ) \
	CONFIG_VALUE( QStringList, sseExpandTree ) \
	CONFIG_VALUE( QStringList, fo4ExpandTree ) \
	CONFIG_VALUE( QStringList, fonvExpandTree ) \
	CONFIG_VALUE( QStringList, toolPaths ) \
	CONFIG_VALUE( QString, bsaToolPath ) \
	CONFIG_VALUE( QString, espToolPath ) \
	CONFIG_VALUE( QString, nifToolPath ) \
	CONFIG_VALUE( QString, textToolPath ) \
	CONFIG_VALUE( QString, transSplitter1 ) \
	CONFIG_VALUE( QString, transSplitter2 ) \
	CONFIG_VALUE( QString, mo2ViewSplitter ) \
	CONFIG_VALUE( QString, settingsSplitter ) \
	CONFIG_VALUE( QString, headerTreeViewBuild ) \
	CONFIG_VALUE( QString, mo2ViewHeaderL ) \
	CONFIG_VALUE( QString, UIMO2File_Header ) \
	CONFIG_VALUE( QString, UIMO2Merge_Header ) \
	CONFIG_VALUE( QString, modRightViewHeader ) \
	CONFIG_VALUE( QString, modLeftViewHeader )

//CONFIG_VALUE( QString, addDataPath ) \

class Config : public QObject, public IConfigChanged {
	Q_OBJECT
public:
	CONFIG_VALUE_LIST

	static void save();
	static void load();

	virtual void changedValue() override;

	QString getMO2Path();

};

extern Config config;
