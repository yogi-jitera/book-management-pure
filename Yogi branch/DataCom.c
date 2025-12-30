
// 情報送信「RX210 ⇔ CM3」
// 汎用ライブラリ
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
// RX210独自ライブラリ
#include "RX210.h"
#include "typedefine.h"
#include "iodefine.h"
// オリジナルライブラリ
#include "MyDefine.h"
#include "DataCom.h"
#include "RTC.h"
#include "common.h"
#include "InSig.h"//仮
#include "OutSig.h"
#include "LockUnit.h"	// ﾛｯｸﾕﾆｯﾄ
#include "InternalUnit.h"
#include "common.h"	// 共通ﾙｰﾁﾝ


//#define DEBUG_DATACOM_ERR

// 定義
/*enum {	DATA_NONE = 0,	DATA_NG,	// 共通ｺﾏﾝﾄﾞ
	DATA_OK_DT,	DATA_NG_DT,	// 日付その他ｺﾏﾝﾄﾞ
	DATA_OK_O1,	DATA_NG_O1,	// 汎用出力(単純操作)ｺﾏﾝﾄﾞ
	DATA_OK_O2,	DATA_NG_O2,	// 汎用出力(特定操作)ｺﾏﾝﾄﾞ
	DATA_NG_LU,	DATA_SP_LU,	DATA_PL_LU,	DATA_PS_LU,		// ﾛｯｸﾕﾆｯﾄｺﾏﾝﾄﾞ
	DATA_OK_LOG,DATA_NG_LOG							// サブプロ ログ出力コマンド
	};
*/
enum {					// データ種別
	KIND_SYSTEM,		// システム情報	
	KIND_INPUT,			// 入力ポート情報
	KIND_OUTPUT,		// 出力ポート情報
	KIND_UNIT,			// 内蔵ユニット情報
	KIND_LOCK,			// ロックユニット情報
	KIND_GATE,			// ゲート情報
	KIND_LOG = 0xE0,	// サブプロ ログ情報
	KIND_DMP,			// サブプロ 異常リセット情報
	KIND_ERR = 0xFF,	// データ異常(CRC異常)
};
enum {
	CMD_NONE = 0,		// 受信無し、無視
	CMD_ERR,			// 受信コマンド異常
	CMD_RETRY,			// コマンド再送
	CMD_SYS_DATE,		// システム 日時情報取得
	CMD_SYS_INFO,		// システム	システム情報取得
	CMD_IN_STATUS,		// 入力ポート 入力状態取得
	CMD_IN_SETTING,		// 入力ポート チャタリング吸収設定
	CMD_OUT_STATUS,		// 出力ポート 出力状態取得
	CMD_UNIT,
	CMD_LOCK_NG,		// ロックユニットコマンドNG
	CMD_LOCK_SP,		// ロックユニット制御 サブプロ状態
	CMD_LOCK_PL,		// ロックユニットポーリング
	CMD_LOCK_PS,		// ロックユニットパーキングステータス
	CMD_LOG_DATA,		// ログ情報取得
	CMD_DMP_DATA,		// 異常リセット情報取得
};

// 構造体
struct DataTx
{
	_UBYTE bData;	//テスト用
};

// 変数
struct DataTx DataTx;
volatile struct DataComIr DataComIr;
struct TxLogType TxLog;
struct LogBfType LogBfTx;
char chLogWork[LOG_LENGTH * LOG_CNT];
WORD wHubAutoScan = 0;

// プロトタイプ宣言
char ChkDataCom(void);

void DataComCmdMake(char chCmd);
//char *SetExSw(char *chp);
char *SetMatrixInInfo(char *chp);
char *SetMatrixInSettingInfo(char *chp);
char *SetGPInSettingInfo(char *chp);
//char *SetExIn(char *chp);
char *SetGPInInfo(char *chp);
char *SetVoltageInfo(char *chp,float flVoltage);
void StartDataComTx(void);
char *SetUserSens(char *chp);

// 関数
void InitDataCom(void)
{		// 初期化
	memset(&DataTx, 0, sizeof(DataTx));
	memset((struct DataComIr *)&DataComIr, 0, sizeof(DataComIr));
	memset(&TxLog,0,sizeof(TxLog));
	memset(&LogBfTx,0,sizeof(LogBfTx));
	DataComIr.chProc = RX_BUSY;
	RE_SCI5();	// 受信割込み許可
}

void MainDataCom(void)
{		// ﾃﾞｰﾀ送信ﾒｲﾝ処理
	char ch = CMD_NONE;	// 該当しない場合は"CMD_NONE"
	
	ClrWDT();	// ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｶｳﾝﾀｸﾘｱ
	
	switch (DataComIr.chProc)
	{
		case RX_OK:	// 受信済み
			ch = ChkDataCom();	// 受信ﾃﾞｰﾀの解析
			if(DataComIr.chCmd == CMD_LOG_DATA && ch != CMD_LOG_DATA){
				memset(&LogBfTx,0,sizeof(LogBfTx));
			}
			switch (ch)
			{
/*				case DATA_OK_DT:	// 日付その他OK
				case DATA_NG_DT:	// 日付その他NG
				case DATA_OK_O1:	// 汎用出力(単純動作)OK
				case DATA_NG_O1:	// 汎用出力(単純動作)NG
				case DATA_OK_O2:	// 汎用出力(特定動作)OK
				case DATA_NG_O2:	// 汎用出力(特定動作)NG
				case DATA_NG_LU:	// ﾛｯｸﾕﾆｯﾄNG
				case DATA_SP_LU:	// ﾛｯｸﾕﾆｯﾄｻﾌﾞﾌﾟﾛ
				case DATA_PL_LU:	// ﾛｯｸﾕﾆｯﾄﾎﾟｰﾘﾝｸ
				case DATA_PS_LU:	// ﾛｯｸﾕﾆｯﾄPS
				case DATA_OK_LOG:	// ログ問い合わせ
				//case DATA_NG_LOG:
*/
				case CMD_SYS_DATE:				// システム日時情報
				case CMD_SYS_INFO:				// システム情報
				case CMD_IN_STATUS:				// 入力ポート状態
				case CMD_IN_SETTING:			// 入力ポート設定(チャタリング吸収時間)
				case CMD_OUT_STATUS:			// 出力ポート状態
				case CMD_UNIT:					// 内蔵ユニット状態
				case CMD_LOCK_SP:				// ロックユニット制御サブプロ状態
				case CMD_LOCK_PL:				// ロックユニットポーリング
				case CMD_LOCK_PS:				// ロックユニットPS
				case CMD_LOG_DATA:				// サブプロ ログ情報
				case CMD_DMP_DATA:				// サブプロ コアダンプ情報(意図しないリセット情報)
					DataComIr.chProc = TX_STANDBY;
					DataComIr.chCmd = ch;
					break;
				case CMD_RETRY:					// 前回コマンド再送
					DataComIr.chProc = TX_STANDBY;
					break;
				case CMD_ERR:					// 受信コマンド異常
					DataComIr.chProc = TX_STANDBY;
					DataComIr.chCmd = CMD_ERR;
					break;
				case CMD_NONE:
				default: 
					DataComIr.chProc = RX_BUSY;
					break;
			} // end switch
			memset((char *)&DataComIr.chRx[0],0,sizeof(DataComIr.chRx));
			break;
		
		case TX_STANDBY:	// 送信待ち
			ch = DataComIr.chCmd;
			
			if (ch != CMD_NONE)
			{
				DataComCmdMake(ch);
				StartDataComTx();
			} // end if
			
			break;
		
		default:
			break;
	} // end switch
}

char ChkDataCom(void)
{		// 受信ﾃﾞｰﾀの解析
	char chRes = CMD_NONE;	// 該当しない場合は"DATA_NONE"
	WORD wKind = 0;	// 種別
	WORD wLen  = 0;	// ﾃﾞｰﾀ長
	WORD wParam = 0xFFFF;
	WORD wCmd  = 0;	// ｺﾏﾝﾄﾞ
	WORD wCrc,wRxCrc;
	char ch = 0;	// 汎用変数(char)
	long l  = 0;	// 汎用変数(long)
	char *chp = (char *)&DataComIr.chRx[5];
	
	ch = DataComIr.chRx[0];	// 先頭文字抽出
	
	if (ch != ETX && ch != STX)
	{
//		chRes = DATA_NG;	// 未受信で無く開始文字でもない場合
		chRes = CMD_NONE;	// 先頭データ欠損またはゴミデータは無視
	}
	else
	{
		l = CheckCrc((char *)&DataComIr.chRx, 1, (long)sizeof(DataComIr.chRx));	// CRCﾁｪｯｸ
		
		if (l >= 0)	// CRC一致
		{
			if (DataComIr.chRx[1] == 'M' && DataComIr.chRx[2] == '1' && DataComIr.chRx[3] == '2' && DataComIr.chRx[4] == '2')
			{	// 受信ﾍｯﾀﾞ部OK
				chp = HexAsciiToWord(chp, 2, &wKind);	// 種別
				chp = DecAsciiToWord(chp, 3, &wLen);	// ﾃﾞｰﾀ長
				l = GetDataLength((char *)&DataComIr.chRx , 10, (long)sizeof(DataComIr.chRx)); // ﾃﾞｰﾀ長の次から算出
/*				
				if (wLen == 1)	// ﾃﾞｰﾀ長が1ﾊﾞｲﾄの場合
				{
					switch (*chp)
					{
						case '0':	// 受信完了
							switch (DataComIr.chCmd)
							{
								case DATA_PL_LU:	// 命令がﾎﾟｰﾘﾝｸﾞ要求の場合
									PostLockUnitRequestPL(DataComIr.wParam);
									break;
								
								default:		// その他は何もしない
									break;
							} // ens switch
							
							DataComIr.chCmd  = DATA_NONE;	// ｺﾏﾝﾄﾞ初期化
							DataComIr.wParam = 0;		// ﾊﾟﾗﾒｰﾀ初期化
							break;
						
						case '1':	// 再送要求
							chRes = DataComIr.chCmd;	// 同じｺﾏﾝﾄﾞを再ｾｯﾄ
							break;
							
					} // end switch
					
				}
				else */if (l == (long)wLen) // データ長チェック
				{
					switch (wKind)
					{
						case KIND_SYSTEM:		// システム情報
							chp = DecAsciiToWord(chp, 4, (WORD *)&wParam);	// 種別パラメータ受信
							switch(wParam){
								case 0:		chRes = CMD_SYS_DATE;	break;
								case 1:		chRes = CMD_SYS_INFO;	break;
								default:	chRes = CMD_ERR;		break; 
							}
							break;
						case KIND_INPUT: 		// 入力ポート情報
							chp = DecAsciiToWord(chp, 4, (WORD *)&wParam);	// 種別パラメータ受信
							switch(wParam){
								case 0:		chRes = CMD_IN_STATUS;	break;
								case 1:		chRes = CMD_IN_SETTING;	break;
								default:	chRes = CMD_ERR;		break;
							}
							break;
						case KIND_OUTPUT: 		// 出力ポート情報
							chp = DecAsciiToWord(chp, 4, (WORD *)&wParam);	// 種別パラメータ受信
							switch(wParam){
								case 0:		chRes = CMD_OUT_STATUS;	break;
								default:	chRes = CMD_ERR;		break;
							}
							break;
						case KIND_UNIT:			// 内蔵ユニット情報
							chp = DataComRxInternalUnit(chp);
							chRes = CMD_UNIT;
							break;
						case KIND_LOCK: // ﾛｯｸﾕﾆｯﾄ
							if(wLen == 0)
							{
								switch (DataComIr.chCmd)
								{
									case CMD_LOCK_PL:	// 命令がﾎﾟｰﾘﾝｸﾞ要求の場合
										PostLockUnitRequestPL(DataComIr.wParam);
										break;
									
									default:		// その他は何もしない
										break;
								} // ens switch
								
								DataComIr.chCmd  = CMD_NONE;	// ｺﾏﾝﾄﾞ初期化
								DataComIr.wParam = 0;		// ﾊﾟﾗﾒｰﾀ初期化
							}
							else
							{
								chp = DecAsciiToWord(chp, 2, &wCmd);	// ｺﾏﾝﾄﾞ
								switch (wCmd)
								{
									case 0:	// ｺﾏﾝﾄﾞ0
										//chRes = DATA_SP_LU;	// ｻﾌﾞﾌﾟﾛ状態要求受信
										chRes = CMD_LOCK_SP;
										break;
									
									case 1:	// ｺﾏﾝﾄﾞ1
										//chRes = DATA_PL_LU;	// ﾎﾟｰﾘﾝｸﾞ要求受信(ﾕﾆｯﾄﾌﾞﾛｯｸ番号)
										chRes = CMD_LOCK_PL;
										chp = DecAsciiToWord(chp, 2, (WORD *)&wParam);
										break;
									
									case 2:	// ｺﾏﾝﾄﾞ2
										//chRes = DATA_PS_LU;	// PS要求受信(ﾕﾆｯﾄｱﾄﾞﾚｽ番号)
										chRes = CMD_LOCK_PS;
										chp = DecAsciiToWord(chp, 3, (WORD *)&wParam);
										break;
									
									default:
										//chRes = DATA_NG_LU;	// 不明なﾛｯｸﾕﾆｯﾄｺﾏﾝﾄﾞ
										chRes = CMD_LOCK_NG;
										break;
								} // end switch
							
							}
								
							break;
						case KIND_LOG:	// ログ取得
							chRes = CMD_LOG_DATA;
							break;
						case KIND_DMP:
							chRes = CMD_DMP_DATA;
						case KIND_ERR:
							chRes = CMD_RETRY;
							break;
						default:
							//chRes = DATA_NG;	// 種別が合わない
							chRes = CMD_ERR;
#ifdef DEBUG_DATACOM_ERR
	Debug("データ種別異常検出\r\n");
#endif
							break;
					} // end switch

					if(chRes != CMD_ERR && chRes != CMD_RETRY){
						DataComIr.wKind = wKind;
						DataComIr.wParam = wParam;
					}

				}
				else	// ﾃﾞｰﾀ長が合わない
				{
					/*switch (wKind)
					{
						case 0: // 日付その他(仮
							chRes = DATA_NG_DT;
							break;
						
						case 1: // 汎用出力(単純動作)
							chRes = DATA_NG_O1;
							break;
						
						case 2: // 汎用出力(特定動作)
							chRes = DATA_NG_O2;
							break;
						
						case 3: // ﾛｯｸﾕﾆｯﾄ
							chRes = DATA_NG_LU;
							break;
						
						default:
							chRes = DATA_NG;	// 種別が合わない
							break;
					} // end switch	
					*/
					chRes = CMD_ERR;
#ifdef DEBUG_DATACOM_ERR
	Debug("データ長異常検出\r\n");
#endif				
				} // end if
			}
			else
			{
				//chRes = DATA_NG;	// ﾍｯﾀﾞｰが合わない
				chRes = CMD_ERR;
#ifdef DEBUG_DATACOM_ERR
	Debug("ヘッダー異常検出\r\n");
#endif
			} // end if
		}
		else
		{
			//chRes = DATA_NG;	// CRCが合わない
			chRes = CMD_ERR;
#ifdef DEBUG_DATACOM_ERR
	Debug("CRC異常検出\r\n");
#endif
		} // end if
	} // end if
	
	return chRes;
}

void DataComCmdMake(char chCmd)
{		// ﾃﾞｰﾀの送信
	char ch = 0;	// 汎用変数(char)
	WORD w  = 0;	// 汎用変数(WORD)
	char *chp = (char *)DataComIr.chTx;
	struct Time TimeTmp;
	
	memset((void *)DataComIr.chTx,0x00,sizeof(DataComIr.chTx));//sanodebug
	chp += sprintf(chp, "%cS212", STX);	// ﾍｯﾀﾞｰ
	
	switch (chCmd)
	{
		case CMD_SYS_DATE:			// 日時
			chp += sprintf(chp,"%02X",KIND_SYSTEM);		// データ種別(システム)
			chp += sprintf(chp,"%03d",18);				// データ長
			chp += sprintf(chp,"%04d",DataComIr.wParam);// パラメータ
			TimeTmp = RtcRead(0);  // RTC読み込み
			chp += sprintf(chp, "%04d%02d%02d%02d%02d%02d", TimeTmp.shYear,TimeTmp.chMonth,TimeTmp.chDay,TimeTmp.chHour,TimeTmp.chMin,TimeTmp.chSec);
			break;
		case CMD_SYS_INFO:
			chp += sprintf(chp,"%02X",KIND_SYSTEM);		// データ種別(システム)
			chp += sprintf(chp,"%03d",68);				// データ長
			chp += sprintf(chp,"%04d",DataComIr.wParam);// パラメータ
			chp += sprintf(chp,"%d",GetShutDownReq());	// シャットダウン要求
			chp += sprintf(chp, "%03d%03d%03d%08X", VER, VER_DEC1, VER_DEC2,dwMyCheckSum);		// ソフト情報
			for (ch = 0 ; ch < 46 ; ch++){				// 予備46バイト '0'埋め
				*chp = '0';
				chp++;
			}
			break;
		case CMD_IN_STATUS:
			chp += sprintf(chp,"%02X",KIND_INPUT);		// データ種別
			chp += sprintf(chp,"%03d",56);				// データ長
			chp += sprintf(chp,"%04d",DataComIr.wParam);// パラメータ
			chp += sprintf(chp,"%X",GetRotSw());		// ロータリスイッチ情報
			chp += sprintf(chp,"%d",GetShutDownReq());	// シャットダウン要求
			for (ch = 0 ; ch < 8 ; ch++){				// オンボード機能予備8バイト
				*chp = '0';
				chp++;
			}
			chp = SetVoltageInfo(chp,Get24V_Avg());		// 24V電源電圧
			chp = SetVoltageInfo(chp,Get12VBat_Avg());	// 12Vバッテリー電圧
			chp = SetMatrixInInfo(chp);					// マトリクス入力
			chp = SetGPInInfo(chp);						// 汎用入力	

			break;
		case CMD_IN_SETTING:
			chp += sprintf(chp,"%02X",KIND_INPUT);		// データ種別
			chp += sprintf(chp,"%03d",204);				// データ長 16×4 + 
			chp += sprintf(chp,"%04d",DataComIr.wParam);// パラメータ
			chp = SetMatrixInSettingInfo(chp);				// マトリクス入力チャタリング吸収時間
			chp = SetGPInSettingInfo(chp);					// 汎用入力チャタリング吸収時間
			break;
		case CMD_OUT_STATUS:
			chp += sprintf(chp,"%02X",KIND_OUTPUT);		// データ種別
			
			if(DataComIr.wParam == 0){					// 全出力ポート					
				chp += sprintf(chp,"%03d",48);			// データ長 44
				chp += sprintf(chp,"%04d",DataComIr.wParam);
				for(ch = 1 ; ch < 45 ; ch++){
					chp += sprintf(chp,"%X",GetOutputStatus(ch));
				} 
			}else{										// 特定出力ポート
				chp += sprintf(chp,"%03d",1);			// データ長 1
				chp += sprintf(chp,"%04d",DataComIr.wParam);
				chp += sprintf(chp,"%X",GetOutputStatus(DataComIr.wParam));
			}
			break;
		case CMD_UNIT:
			chp += sprintf(chp,"%02X",KIND_UNIT);		// データ種別
			chp += sprintf(chp,"%03d",GetDataComUnitTxDataLen());				// データ長 0(内蔵ユニットなし) 
			chp = GetDataComUnitTxData(chp);
			break;
		/*
		case DATA_OK_DT:	// 日付その他OK応答
			chp += sprintf(chp, "00");	// 種別
			
			switch (DataComIr.wParam)
			{
				case 0:	// 日時
					chp += sprintf(chp, "%03d", 19); // データ長
					chp += sprintf(chp, "%d", 0); // 正常
					chp += sprintf(chp, "%04d", DataComIr.wParam);
					TimeTmp = RtcRead(0);  // RTC読み込み
					chp += sprintf(chp, "%04d%02d%02d%02d%02d%02d", TimeTmp.shYear,TimeTmp.chMonth,TimeTmp.chDay,TimeTmp.chHour,TimeTmp.chMin,TimeTmp.chSec);
					break;
				
				case 1: // その他
					chp += sprintf(chp, "0690");	// ﾃﾞｰﾀ長(14 + 54)、正常(1) = 69
					chp += sprintf(chp, "%04d", DataComIr.wParam);
					chp += sprintf(chp, "%d", GetShutDownReq());
					chp += sprintf(chp, "%03d%03d%03d", VER, VER_DEC1, VER_DEC2);
					
					for (ch = 0; ch < 54; ch++)
					{
						chp += sprintf(chp, "0");
					} // end for
					
					break;
				
				default:
					break;
			} // end switch
			
			break;
		*/
//		case DATA_SP_LU:	// ﾛｯｸﾕﾆｯﾄｻﾌﾞﾌﾟﾛ状態応答
		case CMD_LOCK_SP:
			chp += sprintf(chp, "04006000");	// 種別、ﾃﾞｰﾀ長、正常
			if( LockUnitIr.chMode == 32 && wHubAutoScan > 0)
			{
				// Hub仕様の場合は自動接続完了前にfAutoScanPSがOFFとなる為、全車室チェック完了するまではONと判定する
				*chp = '1';
			}
			else
			{
				*chp = (char)GetAutoScanPS() + '0';	// 自動接続状態
			}
			chp++;
			*chp = GetCommSts();			// ﾛｯｸﾕﾆｯﾄ通信状態
			chp++;
			*chp = (char)GetInitSts() + '0';	// 初期化状態
			chp++;
			break;
			
//		case DATA_PL_LU:	// ﾛｯｸﾕﾆｯﾄﾎﾟｰﾘﾝｸﾞ応答
		case CMD_LOCK_PL:
			w  = GetPollingNotifyCount(DataComIr.wParam) * 10;	// 1ﾕﾆｯﾄﾃﾞｰﾀのﾊﾞｲﾄ数を掛ける
			w += 5;	// 固定ﾊﾞｲﾄを足す
			chp += sprintf(chp, "04%03d001", w);	// 種別、ﾃﾞｰﾀ長、正常、ｺﾏﾝﾄﾞ
			chp += sprintf(chp, "%02d", DataComIr.wParam);	// ﾕﾆｯﾄﾌﾞﾛｯｸ番号
			chp  = SetLockUnitRequestPL(chp, DataComIr.wParam);	// ﾎﾟｰﾘﾝｸﾞ応答
			break;
		
//		case DATA_PS_LU:	// ﾛｯｸﾕﾆｯﾄPS応答
		case CMD_LOCK_PS:
			chp += sprintf(chp, "04034002", w);	// 種別、ﾃﾞｰﾀ長、正常、ｺﾏﾝﾄﾞ
			chp += sprintf(chp, "%03d", DataComIr.wParam);	// ﾕﾆｯﾄ番号
			chp  = SetLockUnitRequestPS(chp, DataComIr.wParam);	//PS応答
			break;
//		case DATA_OK_LOG:
		case CMD_LOG_DATA:
			ch = 1;
			if(LogBfTx.LogTime.shYear == 0 && LogBfTx.LogTime.chMonth == 0 && LogBfTx.LogTime.chDay == 0)
			{//送信用のログデータが空なら
				if(TxLog.shCnt > 0)
				{
					LogBfTx = GetTxLog();	// ログバッファから出力用データを取得
					if(LogBfTx.LogTime.shYear != 0 || LogBfTx.LogTime.chMonth != 0 || LogBfTx.LogTime.chDay != 0)
					{// 取得データありなら
						ch = 1;
						DeleteTxLog();	// ログバッファから出力用データを削除する
					}else{
						ch = 0;
					}
				}
				else
				{
					ch = 0;
				}
			}

			if(ch == 1)
			{ // 送信するログデータあり
				chp += sprintf(chp,"E0%03d",GetTxLogLength(LogBfTx));
				chp = SetTxLogString(chp,LogBfTx);
			}
			else
			{
				chp += sprintf(chp, "E0000");	// 種別(E0h)、データ長(0)
			}

			

			break;
		//case DATA_NG_LOG:
		//	break;
		case CMD_DMP_DATA:
			chp += sprintf(chp,"%02X",KIND_DMP);
//		case DATA_NG:		// 不明な受信
		case CMD_ERR:
		default:
#ifdef DEBUG_DATACOM_ERR
	Debug("異常応答コマンド生成\r\n");
#endif
			//chp += sprintf(chp, "FF0011");	// 種別、ﾃﾞｰﾀ長、異常
			chp += sprintf(chp,"%02X",KIND_ERR);
			chp += sprintf(chp,"%03d",0);
			break;
	} // end switch
	
	*chp = ETX;
	chp++;
	chp += sprintf(chp, "%04X", GetCrc((char *)DataComIr.chTx, 1, (long)sizeof(DataComIr.chTx)));	// 固定開始位置でCRC作成
	*chp = EOD;
	chp++;
	*chp = '\0';
}

//char *SetExSw(char *chp)
char *SetMatrixInInfo(char *chp)
{		// マトリクス入力電文作成
	char chScan,chSens;
	BYTE bInfo;
	for (chScan = 0; chScan < 16; chScan++)
	{
		ClrWDT();
		bInfo = 0;
		for (chSens = 3; chSens >= 0; chSens--)
		{
			bInfo = (bInfo << 1) + GetMatrixIn(chScan, chSens);
		}

		chp += sprintf(chp,"%X",(bInfo & 0x0F));
	}
	return chp;
}

//char *SetExIn(char *chp)
char *SetGPInInfo(char *chp)
{		// 汎用入力電文作成
	char ch;
	
	for (ch = 0; ch < 18; ch++)
	{
//		*chp = GetExIn(ch) + 0x30;
		*chp = GetGPIn(ch) + 0x30;
		chp++;
	}
	return chp;
}

// 電圧情報電文作成
char *SetVoltageInfo(char *chp,float flVoltage)
{
	chp += sprintf(chp,"%02d",(short)flVoltage);		// 整数部
	chp += sprintf(chp,"%02d",(short)((flVoltage - (short)flVoltage) * 100));	// 小数部(上位2桁)
	return chp;
}

// マトリクス入力設定情報 電文作成
char *SetMatrixInSettingInfo(char *chp)
{
	char chScan,chSens;
	for(chScan = 0 ; chScan < 16 ; chScan++){
		ClrWDT();
		for(chSens = 0 ; chSens < 4; chSens++){
			chp += sprintf(chp,"%02X",(BYTE)GetMatrixInChatTime(chScan,chSens));
		}
	}
	return chp;
}

// 汎用入力設定情報 電文作成
char *SetGPInSettingInfo(char *chp)
{
	char chNum;
	for(chNum = 0 ; chNum < 18 ; chNum++){
		ClrWDT();
		chp += sprintf(chp,"%04X",GetGPInChatTime(chNum));
		
	}
	return chp;
}

void StartDataComTx(void)
{
	DataComIr.chProc = TX_BUSY;
	DataComIr.shTxPos = 0;
	TE_SCI5(); // 送信割込み許可
}
// ############################################# 外部呼び出し関数 ##################################################
short GetTxLogCount(void)
{
	return TxLog.shCnt;
}
void SetTxLog(const char *chpFormat,...)
{
	va_list args;
//	char chBf[256];
	struct LogBfType BfTmp;
	struct Time TimeTmp;
	long l,lCnt;
	BYTE bScrHold;
	
	memset(&chLogWork[0],0,sizeof(chLogWork));

	va_start(args, chpFormat);
	lCnt = (long)vsprintf(&chLogWork[0], chpFormat, args);
	va_end(args);

	memset(&BfTmp,0,sizeof(BfTmp));
	TimeTmp = RtcRead(FALSE);
	BfTmp.LogTime.shYear = TimeTmp.shYear;
	BfTmp.LogTime.chMonth = TimeTmp.chMonth;
	BfTmp.LogTime.chDay = TimeTmp.chDay;
	BfTmp.LogTime.chHour = TimeTmp.chHour;
	BfTmp.LogTime.chMin = TimeTmp.chMin;
	BfTmp.LogTime.chSec = TimeTmp.chSec;
	BfTmp.LogTime.sh1ms = Rtc1msRead();
	l = 0;
	if(lCnt > LOG_LENGTH){					// ログ1件分のデータ長を超える場合、複数件に分割
		
		while(lCnt > LOG_LENGTH){
			memcpy(&BfTmp.chLogBf[0],&chLogWork[l],(size_t)LOG_LENGTH);
			SaveTxLog(BfTmp);
			lCnt -= LOG_LENGTH;
			l += LOG_LENGTH;
		}
		
	}
	memcpy(&BfTmp.chLogBf[0],&chLogWork[l],(size_t)lCnt);
	SaveTxLog(BfTmp);
	
}
struct LogBfType GetTxLog(void)
{
	struct LogBfType LogBf;
	memset(&LogBf,0,sizeof(LogBf));
	
	if(TxLog.shCnt > 0){
		LogBf = TxLog.LogBf[TxLog.shOutPos];
	}

	return LogBf;

}
WORD GetTxLogLength(struct LogBfType LogBf)
{
	WORD w;
	w = 0;
	ClrWDT();
	while(LogBf.chLogBf[w] != 0 && w <LOG_LENGTH){
		w++;
	}

	if(w > 0)	w += 29;	// SetTxLogStringで生成される日時文字列分を加算 

	return w;
}
char *SetTxLogString(char *chpBf,struct LogBfType LogBf)
{
	short sh;

	if(LogBf.LogTime.shYear == 0 && LogBf.LogTime.chMonth == 0 && LogBf.LogTime.chDay == 0)
		return chpBf;

	// ログ発生日時を文字列化
	chpBf += sprintf(chpBf,"%04d/%02d/%02d %02d:%02d:%02d.%03d,[%02d],",LogBf.LogTime.shYear,		// 年
																		LogBf.LogTime.chMonth,		// 月
																		LogBf.LogTime.chDay,		// 日
																		LogBf.LogTime.chHour,		// 時
																		LogBf.LogTime.chMin,		// 分
																		LogBf.LogTime.chSec,		// 秒
																		LogBf.LogTime.sh1ms,		// 疑似ミリ秒
																		LogBf.bSerial);				// 通し番号
	
	// ログデータ
	sh = 0;
	ClrWDT();
	while(LogBf.chLogBf[sh] != 0 && sh <LOG_LENGTH){
		*chpBf = LogBf.chLogBf[sh];
		chpBf++;
		sh++;
	}
	return chpBf;
}

void SaveTxLog(struct LogBfType BfTmp)
{
	struct LogBfTye *pLogBf;
	
	ClrWDT();	// ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｶｳﾝﾀｸﾘｱ
	
	if(TxLog.shCnt >= LOG_CNT){
		DeleteTxLog();
	}
	BfTmp.bSerial = TxLog.bSerial;
	TxLog.LogBf[TxLog.shInPos] = BfTmp;
	
	TxLog.shCnt++;
	TxLog.shInPos++;
	if(TxLog.shInPos >= LOG_CNT){
		TxLog.shInPos = 0;
	}
	TxLog.bSerial++;
	if(TxLog.bSerial >= 100){
		TxLog.bSerial = 0;
	}
}

void DeleteTxLog(void)
{
	memset(&TxLog.LogBf[TxLog.shOutPos],0,sizeof(TxLog.LogBf[TxLog.shOutPos]));
	TxLog.shOutPos++;
	if(TxLog.shOutPos >= LOG_CNT){
		TxLog.shOutPos = 0;
	}
	if(TxLog.shCnt > 0){
		TxLog.shCnt--;
	}
}

char *SetUserSens(char *chp)
{    // 人検知センサ入力用電文作成
    char ch;

    //*chp = GetUserSens() + 0x30;
    *chp = GetGPIn(IN_BF_USER_SENS) + 0x30;
    chp++;

    //Debug("USER_SENS:%c\n",GetUserSens() + 0x30);

    return chp;
}

/*void Debug(const char *chpFormat,...)
{		// ﾃﾞﾊﾞｯｸﾞ用出力
	va_list args;
	char chBf[256];
	long lCnt;
	
	va_start(args, chpFormat);
	lCnt = (long)vsprintf(&chBf[0], chpFormat, args);
	va_end(args);
	
	memcpy((char *)DataComIr.chTx, (char *)chBf, (size_t)lCnt);
	DataComIr.chTx[lCnt] = 0;
	
	DataComIr.chProc = TX_BUSY;
	DataComIr.shTxPos = 0;
	TE_SCI1();
}*/

// ############################################# 割込み処理 ##################################################
void Ir1msDataCom(void)
{
	if (DataComIr.wRxTmOut > 0)
	{
		DataComIr.wRxTmOut--;
		if (DataComIr.wRxTmOut == 0 && DataComIr.chProc == RX_BUSY)
		{
			DataComIr.chProc = RX_OK;
			DataComIr.chRx[DataComIr.shRxPos] = EOD;
			DataComIr.shRxPos = 0;
		}
	}
}

