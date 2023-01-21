#include "steam.h"

namespace steam {
	/**
	 * @brief  Steamがインストールされたパスを返す
	 */
	QString installPath() {
		QSettings m( R"(HKEY_CURRENT_USER\Software\Valve\Steam)", QSettings::NativeFormat );

		QString value = m.value( "SteamPath", "" ).toString();

		if( value.isEmpty() )return "";

		return value;
	}


	/**
	 * @brief  インストールされるフォルダ名を指定して、インストール先のSteamフルパスを取得する
	 * @return ゲームがインストールされたのSteamフルパス
	 *         インストール先のフォルダが存在しない場合は無効なパスを返す
	 */
	QString installGamePath( const QString& title ) {
		auto path = $$( "%1/steamapps/common/%2" ).arg( installPath() ).arg( title );
		if( !fs::isExistDirectory( path ) )return "";
		return path;
	}


	/**
	 * @brief  ゲームの実行ファイルへのパスを返す
	 */
	QString gameExePath( const QString& title ) {
		QHash<QString, QString> to;
		to.insert( "Fallout 4", "Fallout4.exe" );
		to.insert( "Fallout New Vegas", "FalloutNV.exe" );
		to.insert( "Skyrim Special Edition", "SkyrimSE.exe" );

		auto it = to.find( title );
		if( it == to.constEnd() )return "";
		auto path = $$( "%1/steamapps/common/%2/%3" ).arg( installPath() ).arg( title ).arg( *it );
		return path;
	}
}