#pragma once

class ItemModFormID;
class ContextMenu;

namespace utils {
	QStringList targetGameTitleList();
	QString targetGameTitleName();
	QString installGamePath();

	ItemModFormID* currentSelectFile();

	QStringList bsaExtractedFileList( const QString& filePath );

	/////////////////////////////////////////
	void createFileContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget );

	/////////////////////////////////////////
	void createFolderContextMenu( std::unique_ptr<ContextMenu>* contextMenu, HTreeWidget* treeWidget );

	/////////////////////////////////////////
	QString nexusURL( int modID );

	QStringList& expandTree();
};


