#pragma once

class HTreeWidget;


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenFile : public QAction {
public:
	ActionOpenFile( HTreeWidget* tw );
	static ActionOpenFile* create( HTreeWidget* p ) { return new ActionOpenFile( p ); }
};


//////////////////////////////////////////////////////////////////////////////////
class ActionShowInExplorer : public QAction {
public:
	ActionShowInExplorer( HTreeWidget* tw );
	static ActionShowInExplorer* create( HTreeWidget* p ) { return new ActionShowInExplorer( p ); }

	virtual ~ActionShowInExplorer();
	HTreeWidget* treeWidget;
};


//////////////////////////////////////////////////////////////////////////////////
class ActionCopyFileName : public QAction {
public:
	ActionCopyFileName( HTreeWidget* tw );
	static ActionCopyFileName* create( HTreeWidget* p ) { return new ActionCopyFileName( p ); }

	HTreeWidget* treeWidget;
};


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenBSA : public QAction {
public:
	ActionOpenBSA( HTreeWidget* tw );
	static ActionOpenBSA* create( HTreeWidget* p );

};


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenNIF : public QAction {
public:
	ActionOpenNIF( HTreeWidget* tw );
	static ActionOpenNIF* create( HTreeWidget* p );

};


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenESP : public QAction {
public:
	ActionOpenESP( HTreeWidget* tw );
	static ActionOpenESP* create( HTreeWidget* p );

};

//////////////////////////////////////////////////////////////////////////////////
class ActionOpenText : public QAction {
public:
	ActionOpenText( HTreeWidget* tw );
	static ActionOpenText* create( HTreeWidget* p );

};
