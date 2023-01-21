#pragma once

namespace steam {
	/**
	 * @brief  Steamがインストールされたパスを返す
	 */
	QString installPath();


	/**
	 * @brief  インストールされるフォルダ名を指定して、インストール先のSteamフルパスを取得する
	 * @return ゲームがインストールされたのSteamフルパス
	 *         インストール先のフォルダが存在しない場合は無効なパスを返す
	 */
	QString installGamePath( const QString& title );


	/**
	 * @brief  ゲームの実行ファイルへのパスを返す
	 */
	QString gameExePath( const QString& title );
}
