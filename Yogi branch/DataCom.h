
// 定数宣言
#define LOG_CNT 20
#define LOG_LENGTH 50
// 構造体宣言
struct DataComIr
{
	char chTx[1024];
	char chRx[64];
	short shTxPos;
	short shRxPos;
	char chProc;
	char chCmd;
	WORD wRxTmOut;
	WORD wKind,wParam;
};

struct LogTimeType
{
	short shYear;
	char chMonth, chDay, chWeek, chHour, chMin, chSec;
	short sh1ms;
};

struct LogBfType{
	struct LogTimeType LogTime;
	short LogTime1ms;
	BYTE bSerial;
	char chLogBf[LOG_LENGTH];
};

struct TxLogType{
    short shInPos,shOutPos,shCnt;
	BYTE bSerial;
    struct LogBfType LogBf[LOG_CNT];
};

// 外部変数宣言
//extern volatile struct DataTxIr DataTxIr;
extern volatile struct DataComIr DataComIr;

// 関数プロトタイプ宣言
void InitDataCom(void);
void MainDataCom(void);

short GetTxLogCount(void);
void SetTxLog(const char *chpFormat,...);
struct LogBfType GetTxLog(void);
WORD GetTxLogLength(struct LogBfType LogBf);
char *SetTxLogString(char *chpBf,struct LogBfType LogBf);
void SaveTxLog(struct LogBfType BfTmp);
void DeleteTxLog(void);

void Ir1msDataCom(void);

//void Debug(const char *chpFormat,...);