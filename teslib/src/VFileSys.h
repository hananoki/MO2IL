#pragma once
#include <QObject>

//////////////////////////////////////////////////////////////////////////////////
class VFileSys : public QObject {
	Q_OBJECT
public:
	VFileSys();
	virtual ~VFileSys();

	/**
	 * @brief  MO2ファイルシステムで初期化する
	 */
	void initMO2( const QString& installGamePath, const QString& mo2Path, const QString& profileName );

	///////////////////////////////////////
	void enumerateFiles( std::function<void( const QString& folder, const QString& file, const QString& fullName )> cb );

	///////////////////////////////////////
	QString absolutePath( QString virtualPath );

	///////////////////////////////////////
	QString mo2Path();

	///////////////////////////////////////
	QString mo2ProfilePath();

	///////////////////////////////////////
	QHash<QString, QString> getModFiles();

	///////////////////////////////////////
	void dumpText();

signals:
	// 仮想ファイルの初期化開始時
	void signal_initStarted( QString );

	// 仮想ファイルの初期化が完了した時
	void signal_initCompleted();

	// Modファイル列挙が完了した時
	void signal_completeStage1();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

extern VFileSys fileDB;
