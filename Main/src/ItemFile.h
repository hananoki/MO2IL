#pragma once

//////////////////////////////////////////////////////////////////////////////////
// ファイルパスを持つアイテム
class ItemFile : public HTreeWidgetItem {
public:
	QString fullPath;
	int previewNo;

	/////////////////////////////////////////
	ItemFile( QString _fullPath, int column = 0, QString _replace = "" );

	/////////////////////////////////////////
	bool showInExplorer();
	bool openFile();

	/////////////////////////////////////////
	QString fileName();

	/////////////////////////////////////////
	QString folderPath();

	bool hasSuffix( const QString& suffix );
	bool hasSuffix( const QStringList& suffixList );
};


//////////////////////////////////////////////////////////////////////////////////
class ItemGroupViewFile : public ItemFile {
public:
	ItemGroupViewFile( const QString& fullPath );
};
