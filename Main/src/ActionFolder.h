#pragma once


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenFolder : public QAction {
public:
	ActionOpenFolder( HTreeWidget* tw );
	static ActionOpenFolder* create( HTreeWidget* p ) { return new ActionOpenFolder( p ); }
};


//////////////////////////////////////////////////////////////////////////////////
class ActionFolderPathCopy : public QAction {
public:
	ActionFolderPathCopy( HTreeWidget* tw );
	static ActionFolderPathCopy* create( HTreeWidget* p ) { return new ActionFolderPathCopy( p ); }
};


//////////////////////////////////////////////////////////////////////////////////
class ActionFolderOpenNexus : public QAction {
public:
	ActionFolderOpenNexus( HTreeWidget* tw, int modID );
	static ActionFolderOpenNexus* create( HTreeWidget* p );

private:
	int modID = -1;
};
