
#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <Windows.h>
#include <stdio.h>
#include "resource.h"
#include "math.h"

typedef int int32_t;
typedef unsigned int uint32_t;

#define ID_ALARMCHECK 1033
#define ID_SCRRED 100
#define ID_SCRGREEN 101
#define ID_SCRBLUE 102
#define MAX_SCHEDULES 1000
#define WGHT_URGNT 1.0
#define WGHT_IMPTC 1.0
#define WGHT_EASY 0.5
#define TIME_START_X 200
#define TIME_START_Y 10
#define SCHEDULES_START_X 10
#define SCHEDULES_START_Y 10
#define FINISHED_START_X 10
#define FINISHED_START_Y 400

typedef struct _Schedules {
	int32_t s32_TotalPoints;
	int32_t s32_UrgentPoint;
	int32_t s32_ImptncPoint;
	int32_t s32_EasyPoint;
	TCHAR ars8_ScheduleName[200];
	int32_t s32_fAlarmUse;
	SYSTEMTIME stDeadline;
	int32_t s32_AlarmBeforeThisHour;
	int32_t s32_AlarmBeforeThisMinute;
} Schedules;

void SortSchedules(Schedules* parst_Schedules, int32_t s32_NumOfSchedules);

int32_t s32_WriteContext(FILE* fp_Text, Schedules* parst_Schedules, int32_t s32_NumOfSchedules, Schedules* parst_Finished, int32_t s32_NumOfFinished);
int32_t s32_LoadContext(OPENFILENAME* pst_OFN, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinished);
int32_t s32_SaveBeforeExit(FILE* fp_Text, Schedules* parst_Schedules, int32_t s32_NumOfSchedules, Schedules* parst_Finished, int32_t s32_NumOfFinished);
int32_t s32_AutoLoad(FILE* fp_AutoLoad, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinisheds);
int32_t s32_CalAlarmTime(HWND hWnd, SYSTEMTIME* pstCurrentTime, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinished);
int32_t s32_ValidAlarmCheck(HWND hDlg, Schedules* parst_Schedules, int32_t s32_IdxSchedules);
int32_t s32_GetDDAY(SYSTEMTIME stDeatline);

#endif
