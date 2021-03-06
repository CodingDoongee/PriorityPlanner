#include "includes.h"

HWND hCombo;
HWND hUrgent, hImportance, hEasy;
int32_t gs32_Urgent, gs32_Importance, gs32_Easy, gs32_NumOfSchedules, gs32_NumOfFinished;
Schedules garst_Schedules[1000] = { 0 };
Schedules garst_Finished[1000] = { 0 };
int32_t gs32_fSelectedSchedulesIdx = -1, gs32_fSelectedFinishedIdx = -1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TAEDONG'S PLANNER v1.9");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(ID_Menu);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		1200, 200, 400, 800,NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, NULL, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

	}
	return (int)Message.wParam;
}


BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int32_t s32_TempPos, s32_I_ComboBox, s32_I_Delete;
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR ars8_temp_str[512];
	char str[128];
	static HWND hDeadlineReg = NULL;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		hDeadlineReg = CreateWindow(TEXT("Button"), TEXT("마감 시간 등록"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			20, 180, 180, 25, hDlg, (HMENU)ID_ALARMCHECK, g_hInst, NULL);

		if (gs32_fSelectedSchedulesIdx != -1 && gs32_fSelectedFinishedIdx == -1)
		{
			SetDlgItemText(hDlg, IDC_EDIT1, garst_Schedules[gs32_fSelectedSchedulesIdx].ars8_ScheduleName);
			hUrgent = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 60, 200, 20, hDlg, (HMENU)ID_SCRRED, g_hInst, NULL);
			hImportance = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 90, 200, 20, hDlg, (HMENU)ID_SCRGREEN, g_hInst, NULL);
			hEasy = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 120, 200, 20, hDlg, (HMENU)ID_SCRBLUE, g_hInst, NULL);
			SetScrollRange(hUrgent, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hUrgent, SB_CTL, garst_Schedules[gs32_fSelectedSchedulesIdx].s32_UrgentPoint, TRUE);
			SetScrollRange(hImportance, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hImportance, SB_CTL, garst_Schedules[gs32_fSelectedSchedulesIdx].s32_ImptncPoint, TRUE);
			SetScrollRange(hEasy, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hEasy, SB_CTL, garst_Schedules[gs32_fSelectedSchedulesIdx].s32_EasyPoint, TRUE);

			//알람 관련
			if (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse == 1)
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_CHECKED, 0);
				SetDlgItemInt(hDlg, IDC_MONTH, (garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wMonth), FALSE);
				SetDlgItemInt(hDlg, IDC_DATE, (garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wDay), FALSE);
				SetDlgItemInt(hDlg, IDC_HOUR, (garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wHour), FALSE);
				SetDlgItemInt(hDlg, IDC_MINUTE, (garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wMinute), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE, (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisHour), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE2, (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisMinute), FALSE);
				
			}
			else
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
			}

		}
		else if (gs32_fSelectedSchedulesIdx == -1 && gs32_fSelectedFinishedIdx != -1)
		{
			SetDlgItemText(hDlg, IDC_EDIT1, garst_Finished[gs32_fSelectedFinishedIdx].ars8_ScheduleName);
			hUrgent = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 60, 200, 20, hDlg, (HMENU)ID_SCRRED, g_hInst, NULL);
			hImportance = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 90, 200, 20, hDlg, (HMENU)ID_SCRGREEN, g_hInst, NULL);
			hEasy = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 120, 200, 20, hDlg, (HMENU)ID_SCRBLUE, g_hInst, NULL);
			SetScrollRange(hUrgent, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hUrgent, SB_CTL, garst_Finished[gs32_fSelectedFinishedIdx].s32_UrgentPoint, TRUE);
			SetScrollRange(hImportance, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hImportance, SB_CTL, garst_Finished[gs32_fSelectedFinishedIdx].s32_ImptncPoint, TRUE);
			SetScrollRange(hEasy, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hEasy, SB_CTL, garst_Finished[gs32_fSelectedFinishedIdx].s32_EasyPoint, TRUE);
		
			//알람 관련
			if (garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse == 1)
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_CHECKED, 0);
				SetDlgItemInt(hDlg, IDC_MONTH, (garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMonth), FALSE);
				SetDlgItemInt(hDlg, IDC_DATE, (garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wDay), FALSE);
				SetDlgItemInt(hDlg, IDC_HOUR, (garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wHour), FALSE);
				SetDlgItemInt(hDlg, IDC_MINUTE, (garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMinute), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE, (garst_Finished[gs32_fSelectedFinishedIdx].s32_AlarmBeforeThisHour), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE2, (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisMinute), FALSE);
			}
			else
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
		else   //둘 다 -1
		{
			hUrgent = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 60, 200, 20, hDlg, (HMENU)ID_SCRRED, g_hInst, NULL);
			hImportance = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 90, 200, 20, hDlg, (HMENU)ID_SCRGREEN, g_hInst, NULL);
			hEasy = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
				150, 120, 200, 20, hDlg, (HMENU)ID_SCRBLUE, g_hInst, NULL);
			SetScrollRange(hUrgent, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hUrgent, SB_CTL, 0, TRUE);
			SetScrollRange(hImportance, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hImportance, SB_CTL, 0, TRUE);
			SetScrollRange(hEasy, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hEasy, SB_CTL, 0, TRUE);

			//알람 관련
			if (garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse == 1)
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_CHECKED, 0);
				SetDlgItemInt(hDlg, IDC_MONTH, (garst_Schedules[gs32_NumOfSchedules].stDeadline.wMonth), FALSE);
				SetDlgItemInt(hDlg, IDC_DATE, (garst_Schedules[gs32_NumOfSchedules].stDeadline.wDay), FALSE);
				SetDlgItemInt(hDlg, IDC_HOUR, (garst_Schedules[gs32_NumOfSchedules].stDeadline.wHour), FALSE);
				SetDlgItemInt(hDlg, IDC_MINUTE, (garst_Schedules[gs32_NumOfSchedules].stDeadline.wMinute), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE, (garst_Schedules[gs32_NumOfSchedules].s32_AlarmBeforeThisHour), FALSE);
				SetDlgItemInt(hDlg, IDC_ALARMBEFORE2, (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisMinute), FALSE);
			}
			else
			{
				SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
		InvalidateRect(hDlg, NULL, TRUE);
		return TRUE;

	case WM_HSCROLL:

		if ((HWND)lParam == hUrgent) s32_TempPos = gs32_Urgent;
		if ((HWND)lParam == hImportance) s32_TempPos = gs32_Importance;
		if ((HWND)lParam == hEasy) s32_TempPos = gs32_Easy;
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			s32_TempPos = max(0, s32_TempPos - 1);
			break;
		case SB_LINERIGHT:
			s32_TempPos = min(255, s32_TempPos + 1);
			break;
		case SB_PAGELEFT:
			s32_TempPos = max(0, s32_TempPos - 10);
			break;
		case SB_PAGERIGHT:
			s32_TempPos = min(255, s32_TempPos + 10);
			break;
		case SB_THUMBTRACK:
			s32_TempPos = HIWORD(wParam);
			break;
		}
		if ((HWND)lParam == hUrgent) gs32_Urgent = s32_TempPos;
		if ((HWND)lParam == hImportance) gs32_Importance = s32_TempPos;
		if ((HWND)lParam == hEasy) gs32_Easy = s32_TempPos;
		SetScrollPos((HWND)lParam, SB_CTL, s32_TempPos, TRUE);


		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		sprintf_s(ars8_temp_str, "하나도 안급해..", sizeof(ars8_temp_str));
		TextOut(hdc, 10, 60, ars8_temp_str, strlen(ars8_temp_str));
		sprintf_s(ars8_temp_str, "엄청 급해!!", sizeof(ars8_temp_str));
		TextOut(hdc, 410, 60, ars8_temp_str, strlen(ars8_temp_str));
		sprintf_s(ars8_temp_str, "그닥 안중요해..", sizeof(ars8_temp_str));
		TextOut(hdc, 10, 90, ars8_temp_str, strlen(ars8_temp_str));
		sprintf_s(ars8_temp_str, "엄청 중요해!!", sizeof(ars8_temp_str));
		TextOut(hdc, 410, 90, ars8_temp_str, strlen(ars8_temp_str));
		sprintf_s(ars8_temp_str, "엄청 오래걸려", sizeof(ars8_temp_str));
		TextOut(hdc, 10, 120, ars8_temp_str, strlen(ars8_temp_str));
		sprintf_s(ars8_temp_str, "금방 끝내", sizeof(ars8_temp_str));
		TextOut(hdc, 410, 120, ars8_temp_str, strlen(ars8_temp_str));
		EndPaint(hDlg, &ps);
		return 0;



	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			if (gs32_fSelectedSchedulesIdx != -1 && gs32_fSelectedFinishedIdx == -1)
			{
				GetDlgItemText(hDlg, IDC_EDIT1, garst_Schedules[gs32_fSelectedSchedulesIdx].ars8_ScheduleName, 200);
				garst_Schedules[gs32_fSelectedSchedulesIdx].s32_UrgentPoint = GetScrollPos(hUrgent, SB_CTL);
				garst_Schedules[gs32_fSelectedSchedulesIdx].s32_ImptncPoint = GetScrollPos(hImportance, SB_CTL);
				garst_Schedules[gs32_fSelectedSchedulesIdx].s32_EasyPoint = GetScrollPos(hEasy, SB_CTL);
				garst_Schedules[gs32_fSelectedSchedulesIdx].s32_TotalPoints = WGHT_URGNT * garst_Schedules[gs32_fSelectedSchedulesIdx].s32_UrgentPoint 
					+ WGHT_IMPTC * garst_Schedules[gs32_fSelectedSchedulesIdx].s32_ImptncPoint 
					+ WGHT_EASY * garst_Schedules[gs32_fSelectedSchedulesIdx].s32_EasyPoint;

				//알람 정보 저장
				if (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse == 1)
				{
					(garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wMonth) = (WORD)GetDlgItemInt(hDlg, IDC_MONTH, NULL, FALSE);
					(garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wDay) = (WORD)GetDlgItemInt(hDlg, IDC_DATE, NULL, FALSE);
					(garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wHour) = (WORD)GetDlgItemInt(hDlg, IDC_HOUR, NULL, FALSE);
					(garst_Schedules[gs32_fSelectedSchedulesIdx].stDeadline.wMinute) = (WORD)GetDlgItemInt(hDlg, IDC_MINUTE, NULL, FALSE);
					(garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisHour) = GetDlgItemInt(hDlg, IDC_ALARMBEFORE, NULL, FALSE);
					(garst_Schedules[gs32_fSelectedSchedulesIdx].s32_AlarmBeforeThisMinute) = GetDlgItemInt(hDlg, IDC_ALARMBEFORE2, NULL, FALSE);
					if (s32_ValidAlarmCheck(hDlg, garst_Schedules, gs32_fSelectedSchedulesIdx) == FALSE)
					{
						return FALSE;
					}
				}
				
				
				if (
					(garst_Schedules[gs32_fSelectedSchedulesIdx].s32_TotalPoints < 0)
					|| (strlen(garst_Schedules[gs32_fSelectedSchedulesIdx].ars8_ScheduleName) == 0)
					)
				{
					MessageBox(hDlg, TEXT("정상적으로 입력해주세요.."), TEXT("ERROR!!"), MB_OK);
					return FALSE;
				}
				else
				{
					EndDialog(hDlg, 0);
					SortSchedules(garst_Schedules, gs32_NumOfSchedules);
					SortSchedules(garst_Finished, gs32_NumOfFinished);
					return TRUE;
				}
			}
			else if (gs32_fSelectedSchedulesIdx == -1 && gs32_fSelectedFinishedIdx != -1)
			{
				GetDlgItemText(hDlg, IDC_EDIT1, garst_Finished[gs32_fSelectedFinishedIdx].ars8_ScheduleName, 200);
				garst_Finished[gs32_fSelectedFinishedIdx].s32_UrgentPoint = GetScrollPos(hUrgent, SB_CTL);
				garst_Finished[gs32_fSelectedFinishedIdx].s32_ImptncPoint = GetScrollPos(hImportance, SB_CTL);
				garst_Finished[gs32_fSelectedFinishedIdx].s32_EasyPoint = GetScrollPos(hEasy, SB_CTL);
				garst_Finished[gs32_fSelectedFinishedIdx].s32_TotalPoints = WGHT_URGNT * garst_Finished[gs32_fSelectedFinishedIdx].s32_UrgentPoint 
					+ WGHT_IMPTC * garst_Finished[gs32_fSelectedFinishedIdx].s32_ImptncPoint 
					+ WGHT_EASY * garst_Finished[gs32_fSelectedFinishedIdx].s32_EasyPoint;

				//알람 정보 저장
				if (garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse == 1)
				{
					//완료 된 목록은 알람시간을 해제함
					garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse == 0;
					SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);

					(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMonth) = (WORD)0;
					(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wDay) = (WORD)0;
					(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wHour) = (WORD)0;
					(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMinute) = (WORD)0;
					MessageBox(hDlg, TEXT("완료 된 스케쥴이므로 알람을 해제합니다."), TEXT("ERROR!!"), MB_OK);
				}

				if (
					(garst_Finished[gs32_fSelectedSchedulesIdx].s32_TotalPoints < 0)
					|| (strlen(garst_Finished[gs32_fSelectedFinishedIdx].ars8_ScheduleName) == 0)
					)
				{
					MessageBox(hDlg, TEXT("정상적으로 입력해주세요.."), TEXT("ERROR!!"), MB_OK);
					return FALSE;
				}
				else
				{
					EndDialog(hDlg, 0);
					SortSchedules(garst_Schedules, gs32_NumOfSchedules);
					SortSchedules(garst_Finished, gs32_NumOfFinished);
					return TRUE;
				}
			}
			else
			{
				GetDlgItemText(hDlg, IDC_EDIT1, garst_Schedules[gs32_NumOfSchedules].ars8_ScheduleName, 200);
				garst_Schedules[gs32_NumOfSchedules].s32_UrgentPoint = GetScrollPos(hUrgent, SB_CTL);
				garst_Schedules[gs32_NumOfSchedules].s32_ImptncPoint = GetScrollPos(hImportance, SB_CTL);
				garst_Schedules[gs32_NumOfSchedules].s32_EasyPoint = GetScrollPos(hEasy, SB_CTL);
				garst_Schedules[gs32_NumOfSchedules].s32_TotalPoints = WGHT_URGNT * garst_Schedules[gs32_NumOfSchedules].s32_UrgentPoint 
					+ WGHT_IMPTC * garst_Schedules[gs32_NumOfSchedules].s32_ImptncPoint 
					+ WGHT_EASY * garst_Schedules[gs32_NumOfSchedules].s32_EasyPoint;

				//알람 정보 저장
				if (garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse == 1)
				{
					(garst_Schedules[gs32_NumOfSchedules].stDeadline.wMonth) = (WORD)GetDlgItemInt(hDlg, IDC_MONTH, NULL, FALSE);
					(garst_Schedules[gs32_NumOfSchedules].stDeadline.wDay) = (WORD)GetDlgItemInt(hDlg, IDC_DATE, NULL, FALSE);
					(garst_Schedules[gs32_NumOfSchedules].stDeadline.wHour) = (WORD)GetDlgItemInt(hDlg, IDC_HOUR, NULL, FALSE);
					(garst_Schedules[gs32_NumOfSchedules].stDeadline.wMinute) = (WORD)GetDlgItemInt(hDlg, IDC_MINUTE, NULL, FALSE);
					(garst_Schedules[gs32_NumOfSchedules].s32_AlarmBeforeThisHour) = GetDlgItemInt(hDlg, IDC_ALARMBEFORE, NULL, FALSE);
					(garst_Schedules[gs32_NumOfSchedules].s32_AlarmBeforeThisMinute) = GetDlgItemInt(hDlg, IDC_ALARMBEFORE2, NULL, FALSE);
					if (s32_ValidAlarmCheck(hDlg, garst_Schedules, gs32_NumOfSchedules) == FALSE)
					{
						return FALSE;
					}
				}

				if (
					(garst_Schedules[gs32_NumOfSchedules].s32_TotalPoints < 0)
					|| (strlen(garst_Schedules[gs32_NumOfSchedules].ars8_ScheduleName) == 0)
					)
				{
					MessageBox(hDlg, TEXT("정상적으로 입력해주세요.."), TEXT("ERROR!!"), MB_OK);
					return FALSE;
				}
				else
				{
					gs32_NumOfSchedules = gs32_NumOfSchedules + 1;
					EndDialog(hDlg, 0);
					SortSchedules(garst_Schedules, gs32_NumOfSchedules);
					SortSchedules(garst_Finished, gs32_NumOfFinished);
					return TRUE;
				}
			}
			

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		
		case ID_DELETE:
			if (gs32_fSelectedSchedulesIdx != -1 && gs32_fSelectedFinishedIdx == -1)
			{
				memset(&(garst_Schedules[gs32_fSelectedSchedulesIdx]), 0, sizeof(Schedules));
				for (s32_I_Delete = gs32_fSelectedSchedulesIdx; s32_I_Delete < gs32_NumOfSchedules-1; s32_I_Delete++)
				{
					memcpy(&(garst_Schedules[s32_I_Delete]), &(garst_Schedules[s32_I_Delete+1]), sizeof(Schedules));
				}
				gs32_NumOfSchedules--;
				EndDialog(hDlg, 0);
				SortSchedules(garst_Schedules, gs32_NumOfSchedules);
				SortSchedules(garst_Finished, gs32_NumOfFinished);

				return TRUE;
			}
			else if (gs32_fSelectedSchedulesIdx == -1 && gs32_fSelectedFinishedIdx != -1)
			{
				memset(&(garst_Finished[gs32_fSelectedFinishedIdx]), 0, sizeof(Schedules));
				for (s32_I_Delete = gs32_fSelectedFinishedIdx; s32_I_Delete < gs32_NumOfFinished - 1; s32_I_Delete++)
				{
					memcpy(&(garst_Finished[s32_I_Delete]), &(garst_Finished[s32_I_Delete + 1]), sizeof(Schedules));
				}
				gs32_NumOfFinished--;
				EndDialog(hDlg, 0);
				SortSchedules(garst_Schedules, gs32_NumOfSchedules);
				SortSchedules(garst_Finished, gs32_NumOfFinished);

			}
			else
			{
				MessageBox(hDlg, TEXT("지울 스케쥴이 없습니다!!"), TEXT("ERROR!!"), MB_OK);
			}

			return TRUE;
		case ID_ALARMCHECK:
			if (gs32_fSelectedSchedulesIdx != -1 && gs32_fSelectedFinishedIdx == -1)
			{
				if (SendMessage(hDeadlineReg, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
				{
					if (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse == 0)
					{
						SendMessage(hDeadlineReg, BM_SETCHECK, BST_CHECKED, 0);
						garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse = 1;
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}

					
				}
				else
				{
					if (garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse == 1)
					{
						SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
						garst_Schedules[gs32_fSelectedSchedulesIdx].s32_fAlarmUse = 0;
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}
					
				}
				InvalidateRect(hDlg, NULL, TRUE);
				break;
			}
			else if (gs32_fSelectedSchedulesIdx == -1 && gs32_fSelectedFinishedIdx != -1)
			{
				if (SendMessage(hDeadlineReg, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
				{
					if (garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse == 0)
					{
						//SendMessage(hAlarmCheckBox, BM_SETCHECK, BST_CHECKED, 0);
						//garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse = 1;
						(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMonth) = (WORD)0;
						(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wDay) = (WORD)0;
						(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wHour) = (WORD)0;
						(garst_Finished[gs32_fSelectedFinishedIdx].stDeadline.wMinute) = (WORD)0;
						MessageBox(hDlg, TEXT("완료 된 스케쥴이므로 알람을 해제합니다."), TEXT("ERROR!!"), MB_OK);
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}

				}
				else
				{
					if (garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse == 1)
					{
						SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
						garst_Finished[gs32_fSelectedFinishedIdx].s32_fAlarmUse = 0;
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}

				}
				InvalidateRect(hDlg, NULL, TRUE);
				break;
			}
			else
			{
				if (SendMessage(hDeadlineReg, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
				{
					if (garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse == 0)
					{
						SendMessage(hDeadlineReg, BM_SETCHECK, BST_CHECKED, 0);
						garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse = 1;
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}

				}
				else
				{
					if (garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse == 1)
					{
						SendMessage(hDeadlineReg, BM_SETCHECK, BST_UNCHECKED, 0);
						garst_Schedules[gs32_NumOfSchedules].s32_fAlarmUse = 0;
					}
					else
					{
						//ERROR!!
						MessageBox(hDlg, TEXT("알람 플래그와 체크 상태가 이상함!!"), TEXT("ERROR!!"), MB_OK);
					}

				}
				InvalidateRect(hDlg, NULL, TRUE);
				break;
			}
			
		}
		break;
		
	}
	return FALSE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int32_t s32_TempPos;
	HDC hdc;
	PAINTSTRUCT ps;
	int32_t s32_I_Schedules;
	int32_t s32_I_idx, s32_I_idx2, s32_I_ScheduleFound = 0, s32_I_FinishedFound = 0;
	int32_t s32_xpos, s32_ypos, s32_Dday;
	SYSTEMTIME st_LocalTime;
	FILE* fp_Text;
	FILE* fp_ExitSave;
	TCHAR szTemp_str[256];
	TCHAR szAlarmInfo[100];
	TCHAR szDdayInfo[100];
	static TCHAR szTimeString[128];
	static TCHAR szTimeStringPrint[128];
	TCHAR szFilePath[MAX_PATH] = { 0 };
	TCHAR szCurrentPath[MAX_PATH] = { 0 };
	OPENFILENAME st_OFN;

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		fopen_s(&fp_ExitSave, TEXT("AutoSaveFile"), "rb");
		if (fp_ExitSave != NULL)
		{
			s32_AutoLoad(fp_ExitSave, garst_Schedules, &gs32_NumOfSchedules, garst_Finished, &gs32_NumOfFinished);
			fclose(fp_ExitSave);
		}
		
		return 0;

	case WM_TIMER:
		GetLocalTime(&st_LocalTime);
		wsprintf(szTimeString, TEXT("%d-%d-%d   %d시 %d분 %d초"), st_LocalTime.wYear, st_LocalTime.wMonth, st_LocalTime.wDay
			, st_LocalTime.wHour, st_LocalTime.wMinute, st_LocalTime.wSecond);

		//알람 시간계산
		s32_CalAlarmTime(hWnd, &st_LocalTime, garst_Schedules, &gs32_NumOfSchedules, garst_Finished, &gs32_NumOfFinished);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		sprintf_s(szTemp_str, "<TO DO LIST>", sizeof(szTemp_str));
		TextOut(hdc, SCHEDULES_START_X, SCHEDULES_START_Y, szTemp_str, strlen(szTemp_str));
		TextOut(hdc, TIME_START_X, TIME_START_Y, szTimeString, strlen(szTimeString));

		for (s32_I_Schedules = 0; s32_I_Schedules < gs32_NumOfSchedules; s32_I_Schedules++)
		{
			TextOut(hdc, SCHEDULES_START_X, SCHEDULES_START_Y + (s32_I_Schedules + 1) * 30, garst_Schedules[s32_I_Schedules].ars8_ScheduleName, strlen(garst_Schedules[s32_I_Schedules].ars8_ScheduleName));
			//알람 정보 표시
			if (garst_Schedules[s32_I_Schedules].s32_fAlarmUse == 1)
			{
				wsprintf(szAlarmInfo, TEXT("(마감: %d/%d %d시%d분)"), garst_Schedules[s32_I_Schedules].stDeadline.wMonth, 
					garst_Schedules[s32_I_Schedules].stDeadline.wDay, garst_Schedules[s32_I_Schedules].stDeadline.wHour,
					garst_Schedules[s32_I_Schedules].stDeadline.wMinute);
				TextOut(hdc, SCHEDULES_START_X+ 8*strlen(garst_Schedules[s32_I_Schedules].ars8_ScheduleName)+3,
					SCHEDULES_START_Y + (s32_I_Schedules + 1) * 30, szAlarmInfo, strlen(szAlarmInfo));
				s32_Dday = s32_GetDDAY(garst_Schedules[s32_I_Schedules].stDeadline);
				if (s32_Dday > 0)
				{
					SetTextColor(hdc, RGB(255, 0, 0));
					wsprintf(szDdayInfo, TEXT("D-%d"), s32_Dday);
					TextOut(hdc, SCHEDULES_START_X 
						+ 8 * strlen(garst_Schedules[s32_I_Schedules].ars8_ScheduleName)
						+ 7 * strlen(szAlarmInfo)+5,
						SCHEDULES_START_Y + (s32_I_Schedules + 1) * 30, szDdayInfo, strlen(szDdayInfo));
					SetTextColor(hdc, RGB(0, 0, 0));
				}
				else if (s32_Dday == 0)
				{
					SetTextColor(hdc, RGB(255, 0, 0));
					wsprintf(szDdayInfo, TEXT("D-%d !"), s32_Dday);
					TextOut(hdc, SCHEDULES_START_X
						+ 8 * strlen(garst_Schedules[s32_I_Schedules].ars8_ScheduleName)
						+ 7 * strlen(szAlarmInfo) + 5,
						SCHEDULES_START_Y + (s32_I_Schedules + 1) * 30, szDdayInfo, strlen(szDdayInfo));
					SetTextColor(hdc, RGB(0, 0, 0));
				}
				
			}
		}


		sprintf_s(szTemp_str, "<FINISHED LIST>", sizeof(szTemp_str));
		TextOut(hdc, FINISHED_START_X, FINISHED_START_Y, szTemp_str, strlen(szTemp_str));
		for (s32_I_Schedules = 0; s32_I_Schedules < gs32_NumOfFinished; s32_I_Schedules++)
		{
			TextOut(hdc, FINISHED_START_X, FINISHED_START_Y + (s32_I_Schedules + 1) * 30, garst_Finished[s32_I_Schedules].ars8_ScheduleName, strlen(garst_Finished[s32_I_Schedules].ars8_ScheduleName));
		}

		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDBLCLK:
		s32_xpos = LOWORD(lParam);
		s32_ypos = HIWORD(lParam);

		for (s32_I_idx = 0; s32_I_idx < gs32_NumOfSchedules; s32_I_idx++)
		{
			if (
				(s32_xpos > 10 && s32_xpos < 30 * strlen(garst_Schedules[s32_I_idx].ars8_ScheduleName))
				&& (s32_ypos > (10 + (s32_I_idx + 1) * 30 - 15) && s32_ypos < (10 + (s32_I_idx + 1) * 30 + 15))
				)
			{
				s32_I_ScheduleFound = 1;
				break;
			}
		}
		if (s32_I_ScheduleFound == 1)
		{
			memcpy(&(garst_Finished[gs32_NumOfFinished]), &(garst_Schedules[s32_I_idx]), sizeof(Schedules));
			garst_Finished[gs32_NumOfFinished].s32_fAlarmUse = 0;
			memset(&(garst_Finished[gs32_NumOfFinished].stDeadline), 0, sizeof(SYSTEMTIME));
			gs32_NumOfFinished = gs32_NumOfFinished + 1;

			for (s32_I_idx2 = s32_I_idx; s32_I_idx2 < gs32_NumOfSchedules - 1; s32_I_idx2++)
			{
				memcpy(&(garst_Schedules[s32_I_idx2]), &(garst_Schedules[s32_I_idx2 + 1]), sizeof(Schedules));
			}
			memset(&(garst_Schedules[gs32_NumOfSchedules - 1]), 0, sizeof(Schedules));
			gs32_NumOfSchedules = gs32_NumOfSchedules - 1;
		}


		for (s32_I_idx = 0; s32_I_idx < gs32_NumOfFinished; s32_I_idx++)
		{
			if (
				(s32_xpos > 10 && s32_xpos < 15 * strlen(garst_Finished[s32_I_idx].ars8_ScheduleName))
				&& (s32_ypos > (FINISHED_START_Y + (s32_I_idx + 1) * 30 - 15) && s32_ypos < (FINISHED_START_Y + (s32_I_idx + 1) * 30 + 15))
				)
			{
				s32_I_FinishedFound = 1;
				break;
			}
		}
		if (s32_I_FinishedFound == 1)
		{
			memcpy(&(garst_Schedules[gs32_NumOfSchedules]), &(garst_Finished[s32_I_idx]), sizeof(Schedules));
			gs32_NumOfSchedules = gs32_NumOfSchedules + 1;

			for (s32_I_idx2 = s32_I_idx; s32_I_idx2 < gs32_NumOfFinished - 1; s32_I_idx2++)
			{
				memcpy(&(garst_Finished[s32_I_idx2]), &(garst_Finished[s32_I_idx2 + 1]), sizeof(Schedules));
			}
			memset(&(garst_Finished[gs32_NumOfFinished - 1]), 0, sizeof(Schedules));
			gs32_NumOfFinished = gs32_NumOfFinished - 1;
		}
		SortSchedules(garst_Schedules, gs32_NumOfSchedules);
		SortSchedules(garst_Finished, gs32_NumOfFinished);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONDOWN:
		s32_xpos = LOWORD(lParam);
		s32_ypos = HIWORD(lParam);

		for (s32_I_idx = 0; s32_I_idx < gs32_NumOfSchedules; s32_I_idx++)
		{
			if (
				(s32_xpos > 10 && s32_xpos < 30 * strlen(garst_Schedules[s32_I_idx].ars8_ScheduleName))
				&& (s32_ypos > (10 + (s32_I_idx + 1) * 30 - 15) && s32_ypos < (10 + (s32_I_idx + 1) * 30 + 15))
				)
			{
				s32_I_ScheduleFound = 1;
				break;
			}
		}
		if (s32_I_ScheduleFound == 1)
		{
			gs32_fSelectedSchedulesIdx = s32_I_idx;
			gs32_fSelectedFinishedIdx = -1;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
		}


		for (s32_I_idx = 0; s32_I_idx < gs32_NumOfFinished; s32_I_idx++)
		{
			if (
				(s32_xpos > 10 && s32_xpos < 15 * strlen(garst_Finished[s32_I_idx].ars8_ScheduleName))
				&& (s32_ypos > (FINISHED_START_Y + (s32_I_idx + 1) * 30 - 15) && s32_ypos < (FINISHED_START_Y + (s32_I_idx + 1) * 30 + 15))
				)
			{
				s32_I_FinishedFound = 1;
				break;
			}
		}
		if (s32_I_FinishedFound == 1)
		{
			gs32_fSelectedSchedulesIdx = -1;
			gs32_fSelectedFinishedIdx = s32_I_idx;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
		}
		SortSchedules(garst_Schedules, gs32_NumOfSchedules);
		SortSchedules(garst_Finished, gs32_NumOfFinished);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEW:
			gs32_fSelectedSchedulesIdx = -1;
			gs32_fSelectedFinishedIdx = -1;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_EXPORT:
			GetLocalTime(&st_LocalTime);
			wsprintf(szTimeStringPrint, TEXT("%d년%d월%d일_%d시%d분%d초.txt"), st_LocalTime.wYear, st_LocalTime.wMonth, st_LocalTime.wDay
				, st_LocalTime.wHour, st_LocalTime.wMinute, st_LocalTime.wSecond);
			fopen_s(&fp_Text, szTimeStringPrint, "w");
			s32_WriteContext(fp_Text, garst_Schedules, gs32_NumOfSchedules, garst_Finished, gs32_NumOfFinished);
			fclose(fp_Text);

			return 0;
		case ID_LOAD:
			GetCurrentDirectory(MAX_PATH, szCurrentPath);
			memset(&st_OFN, 0, sizeof(OPENFILENAME));
			st_OFN.lStructSize = sizeof(OPENFILENAME);
			st_OFN.hwndOwner = hWnd;
			st_OFN.lpstrFilter = "텍스트 파일\0*.txt";
			st_OFN.lpstrFile = szFilePath;
			st_OFN.nMaxFile = MAX_PATH;
			st_OFN.lpstrInitialDir = szCurrentPath;

			if (GetOpenFileName(&st_OFN) != 0)
			{
				wsprintf(szFilePath, "%s", st_OFN.lpstrFile);
				s32_LoadContext(&st_OFN, garst_Schedules, &gs32_NumOfSchedules, garst_Finished, &gs32_NumOfFinished);
			}
			else
			{
				MessageBox(hWnd, "불러오기를 취소하였습니다.", "불러오기 취소", MB_OK);
			}

			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		case ID_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;

	case WM_DESTROY:
		//종료 전 저장
		fopen_s(&fp_ExitSave, TEXT("AutoSaveFile"), "wb");
		if (fp_ExitSave != NULL)
		{
			s32_SaveBeforeExit(fp_ExitSave, garst_Schedules, gs32_NumOfSchedules, garst_Finished, gs32_NumOfFinished);
		}
		else
		{
			MessageBox(hWnd, TEXT("종료 전 저장에 실패했습니다!!"), TEXT("ERROR!!"), MB_OK);
		}
		
		fclose(fp_ExitSave);

		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}



void SortSchedules(Schedules* parst_Schedules, int32_t s32_NumOfSchedules)
{
	Schedules st_temp_schedules = { 0 };
	int32_t s32_I_schedules, s32_I_Iteration, s32_I_OptimalCheck;

	for (s32_I_Iteration = 0; s32_I_Iteration < s32_NumOfSchedules; s32_I_Iteration++)
	{
		s32_I_OptimalCheck = 1;

		for (s32_I_schedules = 0; s32_I_schedules < s32_NumOfSchedules - 1; s32_I_schedules++)
		{
			if (parst_Schedules[s32_I_schedules].s32_TotalPoints < parst_Schedules[s32_I_schedules + 1].s32_TotalPoints)
			{
				s32_I_OptimalCheck = 0;
				memset(&st_temp_schedules, 0, sizeof(Schedules));
				memcpy(&st_temp_schedules, &(parst_Schedules[s32_I_schedules]), sizeof(Schedules));
				memset(&parst_Schedules[s32_I_schedules], 0, sizeof(Schedules));
				memcpy(&(parst_Schedules[s32_I_schedules]), &(parst_Schedules[s32_I_schedules + 1]), sizeof(Schedules));
				memset(&parst_Schedules[s32_I_schedules + 1], 0, sizeof(Schedules));
				memcpy(&(parst_Schedules[s32_I_schedules + 1]), &(st_temp_schedules), sizeof(Schedules));
			}
		}
		if (s32_I_OptimalCheck == 1)
		{
			break;
		}
	}



}


int32_t s32_WriteContext(FILE* fp_Text, Schedules* parst_Schedules, int32_t s32_NumOfSchedules, Schedules* parst_Finished, int32_t s32_NumOfFinished)
{
	TCHAR szTemp_str[512] = { 0 };
	int32_t s32_I_Schedules;

	sprintf_s(szTemp_str, "<TO DO LIST>", sizeof(szTemp_str));
	fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
	fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);

	for (s32_I_Schedules = 0; s32_I_Schedules < s32_NumOfSchedules; s32_I_Schedules++)
	{
		strcpy_s(szTemp_str, sizeof(szTemp_str), parst_Schedules[s32_I_Schedules].ars8_ScheduleName);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		wsprintf(szTemp_str, TEXT("(%d,%d,%d)"), parst_Schedules[s32_I_Schedules].s32_UrgentPoint,
			parst_Schedules[s32_I_Schedules].s32_ImptncPoint, parst_Schedules[s32_I_Schedules].s32_EasyPoint);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	}

	fwrite("\n\n\n", sizeof(TCHAR), strlen("\n\n\n"), fp_Text);

	sprintf_s(szTemp_str, "<FINISHED LIST>", sizeof(szTemp_str));
	fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
	fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	for (s32_I_Schedules = 0; s32_I_Schedules < s32_NumOfFinished; s32_I_Schedules++)
	{
		strcpy_s(szTemp_str, sizeof(szTemp_str), parst_Finished[s32_I_Schedules].ars8_ScheduleName);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		wsprintf(szTemp_str, TEXT("(%d,%d,%d)"), parst_Finished[s32_I_Schedules].s32_UrgentPoint,
			parst_Finished[s32_I_Schedules].s32_ImptncPoint, parst_Finished[s32_I_Schedules].s32_EasyPoint);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	}

	return 1;
}

int32_t s32_SaveBeforeExit(FILE* fp_Text, Schedules* parst_Schedules, int32_t s32_NumOfSchedules, Schedules* parst_Finished, int32_t s32_NumOfFinished)
{
	TCHAR szTemp_str[512] = { 0 };
	int32_t s32_I_Schedules;

	sprintf_s(szTemp_str, "<TO DO LIST>", sizeof(szTemp_str));
	fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
	fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);

	for (s32_I_Schedules = 0; s32_I_Schedules < s32_NumOfSchedules; s32_I_Schedules++)
	{
		strcpy_s(szTemp_str, sizeof(szTemp_str), parst_Schedules[s32_I_Schedules].ars8_ScheduleName);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		wsprintf(szTemp_str, TEXT("(%d,%d,%d)"), parst_Schedules[s32_I_Schedules].s32_UrgentPoint,
			parst_Schedules[s32_I_Schedules].s32_ImptncPoint, parst_Schedules[s32_I_Schedules].s32_EasyPoint);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	}

	fwrite("\n\n\n", sizeof(TCHAR), strlen("\n\n\n"), fp_Text);

	sprintf_s(szTemp_str, "<FINISHED LIST>", sizeof(szTemp_str));
	fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
	fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	for (s32_I_Schedules = 0; s32_I_Schedules < s32_NumOfFinished; s32_I_Schedules++)
	{
		strcpy_s(szTemp_str, sizeof(szTemp_str), parst_Finished[s32_I_Schedules].ars8_ScheduleName);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		wsprintf(szTemp_str, TEXT("(%d,%d,%d)"), parst_Finished[s32_I_Schedules].s32_UrgentPoint,
			parst_Finished[s32_I_Schedules].s32_ImptncPoint, parst_Finished[s32_I_Schedules].s32_EasyPoint);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	}
	sprintf_s(szTemp_str, "<ALARM INFO>", sizeof(szTemp_str));
	fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
	fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	for (s32_I_Schedules = 0; s32_I_Schedules < s32_NumOfSchedules; s32_I_Schedules++)
	{
		wsprintf(szTemp_str, TEXT("[%d,%d,%d,%d,%d,%d,%d]"),
			parst_Schedules[s32_I_Schedules].s32_fAlarmUse, parst_Schedules[s32_I_Schedules].stDeadline.wMonth,
			parst_Schedules[s32_I_Schedules].stDeadline.wDay, parst_Schedules[s32_I_Schedules].stDeadline.wHour,
			parst_Schedules[s32_I_Schedules].stDeadline.wMinute, parst_Schedules[s32_I_Schedules].s32_AlarmBeforeThisHour,
			parst_Schedules[s32_I_Schedules].s32_AlarmBeforeThisMinute);
		fwrite(szTemp_str, sizeof(TCHAR), strlen(szTemp_str), fp_Text);
		fwrite("\n", sizeof(TCHAR), strlen("\n"), fp_Text);
	}
	return 1;
}

int32_t s32_LoadContext(OPENFILENAME* pst_OFN, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinished)
{
	TCHAR szBuff[512] = { 0 };
	HANDLE fp;
	DWORD NumberOfBytesRead;
	char lpBuffer[1024];
	int32_t s32_I, s32_StringIdx, s32_StartIdx=0, s32_EndIdx = 0, s32_PutPoints = 0, s32_StartIdxSave=0;
	int32_t s32_TempPoint = 0;
	char * szCutContext;
	char * szRemainingContext = NULL;
	static int32_t s_s32_StartFound = 0;
	static int32_t s_s32_FinishFound = 0;

	//초기화
	memset(parst_Schedules, 0, sizeof(Schedules) * 1000);
	memset(parst_Finished, 0, sizeof(Schedules) * 1000);
	*ps32_NumOfSchedules = 0;
	*ps32_NumOfFinished = 0;

	if ((fp = CreateFile((LPCTSTR)pst_OFN->lpstrFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		// 파일 열기 에러 발생
	}
	// 필요한 만큼 ReadFile의 호출을 반복한다.
	ReadFile(fp, lpBuffer, 1024, &NumberOfBytesRead, NULL);

	szCutContext = strtok_s(lpBuffer, "\r\n", &szRemainingContext);
	while (szCutContext != NULL)
	{
		if (strcmp(szCutContext, "<TO DO LIST>") == 0)
		{
			s_s32_StartFound = 1;
			s_s32_FinishFound = 0;
		}
		szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);  
		if ((s_s32_StartFound == 1) && (strcmp(szCutContext, "<FINISHED LIST>") == 0))
		{
			s_s32_StartFound = 0;
			s_s32_FinishFound = 1;
			szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);  
		}

		if ((s_s32_StartFound == 1) && (s_s32_FinishFound == 0))
		{ 
			s32_StartIdx = 0;
			s32_EndIdx = 0;
			s32_TempPoint = 0;
			parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint = -1;
			parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint = -1;
			parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint = -1;

			for (s32_StringIdx = 0; s32_StringIdx < strlen(szCutContext); s32_StringIdx++)
			{
				if (szCutContext[s32_StringIdx] == '(' && s32_StartIdx == 0)
				{
					s32_StartIdx = s32_StringIdx+1;
					s32_StartIdxSave = s32_StartIdx;
				}
				if (s32_StartIdx != 0 && szCutContext[s32_StringIdx] == ',')
				{
					s32_EndIdx = s32_StringIdx - 1;
				}
				if (szCutContext[s32_StringIdx] == ')' && (parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint != -1))
				{
					s32_EndIdx = s32_StringIdx - 1;
				}

				if (s32_StartIdx != 0 && s32_EndIdx != 0)
				{
					s32_TempPoint = atoi(&szCutContext[s32_StartIdx]);

					if (parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint == -1)
					{
						parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint = s32_TempPoint;
					}
					else if (parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint == -1)
					{
						parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint = s32_TempPoint;
					}
					else if (parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint == -1)
					{
						parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint = s32_TempPoint;
						parst_Schedules[*ps32_NumOfSchedules].s32_TotalPoints = WGHT_URGNT*parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint
							+ WGHT_IMPTC*parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint
							+ WGHT_EASY*parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint;
					}

					s32_StartIdx = s32_EndIdx+2;
					s32_EndIdx = 0;
				}
			}
			szCutContext[s32_StartIdxSave-1] = '\0';
			strcpy_s(parst_Schedules[*ps32_NumOfSchedules].ars8_ScheduleName, szCutContext);
			*ps32_NumOfSchedules = (*ps32_NumOfSchedules) + 1;

		}
		if ((s_s32_StartFound == 0) && (s_s32_FinishFound == 1))
		{
			if (*szRemainingContext == '\0')
			{
				break;
			}
			else
			{
				s32_StartIdx = 0;
				s32_EndIdx = 0;
				s32_TempPoint = 0;
				parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint = -1;
				parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint = -1;
				parst_Finished[*ps32_NumOfFinished].s32_EasyPoint = -1;

				for (s32_StringIdx = 0; s32_StringIdx < strlen(szCutContext); s32_StringIdx++)
				{
					if (szCutContext[s32_StringIdx] == '(' && s32_StartIdx == 0)
					{
						s32_StartIdx = s32_StringIdx + 1;
						s32_StartIdxSave = s32_StartIdx;
					}
					if (s32_StartIdx != 0 && szCutContext[s32_StringIdx] == ',')
					{
						s32_EndIdx = s32_StringIdx - 1;
					}
					if (szCutContext[s32_StringIdx] == ')' && (parst_Finished[*ps32_NumOfSchedules].s32_ImptncPoint != -1))
					{
						s32_EndIdx = s32_StringIdx - 1;
					}

					if (s32_StartIdx != 0 && s32_EndIdx != 0)
					{
						s32_TempPoint = atoi(&szCutContext[s32_StartIdx]);

						if (parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint = s32_TempPoint;
						}
						else if (parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint = s32_TempPoint;
						}
						else if (parst_Finished[*ps32_NumOfFinished].s32_EasyPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_EasyPoint = s32_TempPoint;
							parst_Finished[*ps32_NumOfFinished].s32_TotalPoints = WGHT_URGNT*parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint
								+ WGHT_IMPTC*parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint
								+ WGHT_EASY*parst_Finished[*ps32_NumOfFinished].s32_EasyPoint;
						}

						s32_StartIdx = s32_EndIdx + 2;
						s32_EndIdx = 0;
					}
				}
				szCutContext[s32_StartIdxSave - 1] = '\0';
				strcpy_s(parst_Finished[*ps32_NumOfFinished].ars8_ScheduleName, szCutContext);
				*ps32_NumOfFinished = (*ps32_NumOfFinished) + 1;

			}
			


		}
	}


	// 작업이 다 끝났으면 파일을 닫는다.
	CloseHandle(fp);


	return 1;
}


int32_t s32_AutoLoad(FILE* fp_AutoLoad, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinished)
{

	TCHAR* szTempStr;
	int32_t s32_FileSize;
	DWORD NumberOfBytesRead;
	int32_t s32_I, s32_StringIdx, s32_StartIdx = 0, s32_EndIdx = 0, s32_PutPoints = 0, s32_StartIdxSave = 0, s32_I_AlarmSchedules = 0;
	int32_t s32_I_AlarmFilter = 0;
	int32_t s32_I_AlarmFound = 0;
	int32_t s32_TempPoint = 0;
	int32_t s32_TempAlam = 0;
	static int32_t s_s32_StartFound = 0;
	static int32_t s_s32_FinishFound = 0;
	static int32_t s_s32_AlarmFound = 0;
	char * szCutContext;
	char * szRemainingContext = NULL;

	//초기화
	memset(parst_Schedules, 0, sizeof(Schedules) * 1000);
	memset(parst_Finished, 0, sizeof(Schedules) * 1000);
	*ps32_NumOfSchedules = 0;
	*ps32_NumOfFinished = 0;
	

	
	if (fp_AutoLoad != NULL)
	{
		fseek(fp_AutoLoad, 0, SEEK_END);
		s32_FileSize = ftell(fp_AutoLoad);
		szTempStr = (TCHAR*)malloc(s32_FileSize + 1);  
		memset(szTempStr, 0, s32_FileSize + 1); 
		fseek(fp_AutoLoad, 0, SEEK_SET);
		fread(szTempStr, s32_FileSize, 1, fp_AutoLoad);


		szCutContext = strtok_s(szTempStr, "\r\n", &szRemainingContext);
		while (szCutContext != NULL)
		{
			if (strcmp(szCutContext, "<TO DO LIST>") == 0)
			{
				s_s32_StartFound = 1;
				s_s32_FinishFound = 0;
				s_s32_AlarmFound = 0;
			}
			szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);
			if ((s_s32_StartFound == 1) && (strcmp(szCutContext, "<FINISHED LIST>") == 0))
			{
				s_s32_StartFound = 0;
				s_s32_FinishFound = 1;
				s_s32_AlarmFound = 0;
				szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);
			}
			if (strcmp(szCutContext, "<ALARM INFO>") == 0)
			{
				s_s32_StartFound = 0;
				s_s32_FinishFound = 0;
				s_s32_AlarmFound = 1;
				szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);
			}

			if ((s_s32_StartFound == 1) && (s_s32_FinishFound == 0) && (s_s32_AlarmFound == 0))
			{
				s32_StartIdx = 0;
				s32_EndIdx = 0;
				s32_TempPoint = 0;
				parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint = -1;
				parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint = -1;
				parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint = -1;

				for (s32_StringIdx = 0; s32_StringIdx < strlen(szCutContext); s32_StringIdx++)
				{
					if (szCutContext[s32_StringIdx] == '(' && s32_StartIdx == 0)
					{
						s32_StartIdx = s32_StringIdx + 1;
						s32_StartIdxSave = s32_StartIdx;
					}
					if (s32_StartIdx != 0 && szCutContext[s32_StringIdx] == ',')
					{
						s32_EndIdx = s32_StringIdx - 1;
					}
					if (szCutContext[s32_StringIdx] == ')' && (parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint != -1))
					{
						s32_EndIdx = s32_StringIdx - 1;
					}

					if (s32_StartIdx != 0 && s32_EndIdx != 0)
					{
						s32_TempPoint = atoi(&szCutContext[s32_StartIdx]);

						if (parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint == -1)
						{
							parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint = s32_TempPoint;
						}
						else if (parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint == -1)
						{
							parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint = s32_TempPoint;
						}
						else if (parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint == -1)
						{
							parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint = s32_TempPoint;
							parst_Schedules[*ps32_NumOfSchedules].s32_TotalPoints = WGHT_URGNT * parst_Schedules[*ps32_NumOfSchedules].s32_UrgentPoint
								+ WGHT_IMPTC * parst_Schedules[*ps32_NumOfSchedules].s32_ImptncPoint
								+ WGHT_EASY * parst_Schedules[*ps32_NumOfSchedules].s32_EasyPoint;
						}

						s32_StartIdx = s32_EndIdx + 2;
						s32_EndIdx = 0;
					}
				}
				szCutContext[s32_StartIdxSave - 1] = '\0';
				strcpy_s(parst_Schedules[*ps32_NumOfSchedules].ars8_ScheduleName, szCutContext);
				*ps32_NumOfSchedules = (*ps32_NumOfSchedules) + 1;

			}
			if ((s_s32_StartFound == 0) && (s_s32_FinishFound == 1) && (s_s32_AlarmFound == 0))
			{
				if (szCutContext == NULL)
				{
					break;
				}

				s32_StartIdx = 0;
				s32_EndIdx = 0;
				s32_TempPoint = 0;
				parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint = -1;
				parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint = -1;
				parst_Finished[*ps32_NumOfFinished].s32_EasyPoint = -1;

				for (s32_StringIdx = 0; s32_StringIdx < strlen(szCutContext); s32_StringIdx++)
				{
					if (szCutContext[s32_StringIdx] == '(' && s32_StartIdx == 0)
					{
						s32_StartIdx = s32_StringIdx + 1;
						s32_StartIdxSave = s32_StartIdx;
					}
					if (s32_StartIdx != 0 && szCutContext[s32_StringIdx] == ',')
					{
						s32_EndIdx = s32_StringIdx - 1;
					}
					if (szCutContext[s32_StringIdx] == ')' && (parst_Finished[*ps32_NumOfSchedules].s32_ImptncPoint != -1))
					{
						s32_EndIdx = s32_StringIdx - 1;
					}

					if (s32_StartIdx != 0 && s32_EndIdx != 0)
					{
						s32_TempPoint = atoi(&szCutContext[s32_StartIdx]);

						if (parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint = s32_TempPoint;
						}
						else if (parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint = s32_TempPoint;
						}
						else if (parst_Finished[*ps32_NumOfFinished].s32_EasyPoint == -1)
						{
							parst_Finished[*ps32_NumOfFinished].s32_EasyPoint = s32_TempPoint;
							parst_Finished[*ps32_NumOfFinished].s32_TotalPoints = WGHT_URGNT * parst_Finished[*ps32_NumOfFinished].s32_UrgentPoint
								+ WGHT_IMPTC * parst_Finished[*ps32_NumOfFinished].s32_ImptncPoint
								+ WGHT_EASY * parst_Finished[*ps32_NumOfFinished].s32_EasyPoint;
						}

						s32_StartIdx = s32_EndIdx + 2;
						s32_EndIdx = 0;
					}
				}
				szCutContext[s32_StartIdxSave - 1] = '\0';
				strcpy_s(parst_Finished[*ps32_NumOfFinished].ars8_ScheduleName, szCutContext);
				*ps32_NumOfFinished = (*ps32_NumOfFinished) + 1;


				if (*szRemainingContext == '\0')
				{
					break;
				}


			}
			if ((s_s32_StartFound == 0) && (s_s32_FinishFound == 0) && (s_s32_AlarmFound == 1))
			{
				
				for (s32_I_AlarmSchedules = 0; s32_I_AlarmSchedules<*ps32_NumOfSchedules; s32_I_AlarmSchedules++)
				{
					if (szCutContext == NULL)
					{
						break;
					}
					s32_StartIdx = 0;
					parst_Schedules[s32_I_AlarmSchedules].s32_fAlarmUse = 0;
					parst_Schedules[s32_I_AlarmSchedules].stDeadline.wMonth = 0;
					parst_Schedules[s32_I_AlarmSchedules].stDeadline.wDay = 0;
					parst_Schedules[s32_I_AlarmSchedules].stDeadline.wHour = 0;
					parst_Schedules[s32_I_AlarmSchedules].stDeadline.wMinute = 0;
					parst_Schedules[s32_I_AlarmSchedules].s32_AlarmBeforeThisHour = 0;
					parst_Schedules[s32_I_AlarmSchedules].s32_AlarmBeforeThisMinute = 0;
					s32_I_AlarmFound = 0;

					for (s32_StringIdx = 0; s32_StringIdx < strlen(szCutContext); s32_StringIdx++)
					{
						if (szCutContext[s32_StringIdx] == '[' && s32_StartIdx == 0 && s32_I_AlarmFound == 0)
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 1;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 1))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 2;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 2))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 3;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 3))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 4;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 4))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 5;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 5))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 6;
						}
						else if (szCutContext[s32_StringIdx] == ',' && (s32_I_AlarmFound == 6))
						{
							s32_StartIdx = s32_StringIdx + 1;
							s32_I_AlarmFilter = 7;
						}
						else
						{
							s32_I_AlarmFilter = 0;
						}

						switch (s32_I_AlarmFilter)
						{
						case 1:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].s32_fAlarmUse = s32_TempAlam;
							s32_I_AlarmFound = 1;
							break;
						case 2:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].stDeadline.wMonth = s32_TempAlam;
							s32_I_AlarmFound = 2;
							break;
						case 3:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].stDeadline.wDay = s32_TempAlam;
							s32_I_AlarmFound = 3;
							break;
						case 4:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].stDeadline.wHour = s32_TempAlam;
							s32_I_AlarmFound = 4;
							break;
						case 5:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].stDeadline.wMinute = s32_TempAlam;
							s32_I_AlarmFound = 5;
							break;
						case 6:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].s32_AlarmBeforeThisHour = s32_TempAlam;
							s32_I_AlarmFound = 6;
							break;
						case 7:
							s32_TempAlam = atoi(&szCutContext[s32_StartIdx]);
							parst_Schedules[s32_I_AlarmSchedules].s32_AlarmBeforeThisMinute = s32_TempAlam;
							s32_I_AlarmFound = 7;
							break;
						default:
							break;

						}
					}
					szCutContext = strtok_s(NULL, "\r\n", &szRemainingContext);
				}
				if ((*szRemainingContext == '\0') || (szCutContext == NULL))
				{
					break;
				}

			}
		}


		free(szTempStr);
	}
	else
	{
		return -1;
	}

	
	return 1;
}


int32_t s32_CalAlarmTime(HWND hWnd, SYSTEMTIME* pstCurrentTime, Schedules* parst_Schedules, int32_t* ps32_NumOfSchedules, Schedules* parst_Finished, int32_t* ps32_NumOfFinished)
{
	int32_t s32_I_Time = 0;
	int32_t s32_MinutesConverted_Current = 0;
	int32_t s32_MinutesConverted_Schedules[MAX_SCHEDULES] = { 0 };
	TCHAR szAlarmContext[300] = { 0 };
	TCHAR szAlarmBeforeSomeHours[50] = { 0 };
	static int32_t s_s32_MinutesConverted_Prev = 0;
	

	s32_MinutesConverted_Current = 24 * 60 * ((int32_t)(pstCurrentTime->wDay)) + 60 * ((int32_t)(pstCurrentTime->wHour)) + ((int32_t)(pstCurrentTime->wMinute));

	if (s_s32_MinutesConverted_Prev == s32_MinutesConverted_Current)
	{
		return 0;
	}
	else
	{
		s_s32_MinutesConverted_Prev = s32_MinutesConverted_Current;
	}


	for (s32_I_Time = 0; s32_I_Time<*ps32_NumOfSchedules; s32_I_Time++)
	{
		s32_MinutesConverted_Schedules[s32_I_Time] = 24 * 60 * ((int32_t)(parst_Schedules[s32_I_Time].stDeadline.wDay)) 
			+ 60 * ((int32_t)(parst_Schedules[s32_I_Time].stDeadline.wHour))
			+ ((int32_t)(parst_Schedules[s32_I_Time].stDeadline.wMinute));

		if ( ((int32_t)(parst_Schedules[s32_I_Time].stDeadline.wMonth)) == ((int32_t)(pstCurrentTime->wMonth)) )
		{
			if (s32_MinutesConverted_Schedules[s32_I_Time] - s32_MinutesConverted_Current 
				== (parst_Schedules[s32_I_Time].s32_AlarmBeforeThisHour * 60 + parst_Schedules[s32_I_Time].s32_AlarmBeforeThisMinute))
			{
				strcpy_s(szAlarmContext, parst_Schedules[s32_I_Time].ars8_ScheduleName);
				wsprintf(szAlarmBeforeSomeHours, TEXT(" %d시간 %d분 전입니다!!"), 
					parst_Schedules[s32_I_Time].s32_AlarmBeforeThisHour, parst_Schedules[s32_I_Time].s32_AlarmBeforeThisMinute);
				strcat_s(szAlarmContext, sizeof(szAlarmContext), szAlarmBeforeSomeHours);
				MessageBox(hWnd, szAlarmContext, TEXT("ALARM!!"), MB_OK);
			}
			
		}
		else
		{
			// 달 수 가 다르므로 알람 안울림!!
		}
	}

	
	return 1;
}


int32_t s32_ValidAlarmCheck(HWND hDlg, Schedules* parst_Schedules, int32_t s32_IdxSchedules)
{
	int32_t ars32_NumOfDaysInMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	if (
		(parst_Schedules[s32_IdxSchedules].stDeadline.wMonth <= 0
			|| parst_Schedules[s32_IdxSchedules].stDeadline.wMonth >= 13)
		|| (parst_Schedules[s32_IdxSchedules].stDeadline.wDay <= 0
			|| parst_Schedules[s32_IdxSchedules].stDeadline.wDay > ars32_NumOfDaysInMonth[parst_Schedules[s32_IdxSchedules].stDeadline.wMonth-1])
		|| (parst_Schedules[s32_IdxSchedules].stDeadline.wHour >= 24
			|| parst_Schedules[s32_IdxSchedules].stDeadline.wHour < 0)
		|| (parst_Schedules[s32_IdxSchedules].stDeadline.wMinute >= 60
			|| parst_Schedules[s32_IdxSchedules].stDeadline.wMinute < 0)
		)
	{
		MessageBox(hDlg, TEXT("유효한 알람 마감시간을 입력해주세요"), TEXT("ERROR!!"), MB_OK);
		return FALSE;
	}
	else if (
		(parst_Schedules[s32_IdxSchedules].s32_AlarmBeforeThisHour < 0)
		|| (parst_Schedules[s32_IdxSchedules].s32_AlarmBeforeThisMinute < 0)
		|| (parst_Schedules[s32_IdxSchedules].s32_AlarmBeforeThisHour == 0
			&& parst_Schedules[s32_IdxSchedules].s32_AlarmBeforeThisMinute == 0)
		)
	{
		MessageBox(hDlg, TEXT("유효한 알릴 시간을 입력해주세요"), TEXT("ERROR!!"), MB_OK);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int32_t s32_GetDDAY(SYSTEMTIME stDeatline)
{
	SYSTEMTIME stCurrentTime = { 0 };
	int32_t s32_Retval = 0;
	int32_t ars32_NumOfDaysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int32_t s32_BetweenMonthsDays = 0, s32_CurrentMonthDays = 0, s32_LastMonthDays = 0;
	int32_t s32_I_BetweenMonths;
	GetSystemTime(&stCurrentTime);
	if (stDeatline.wMonth - stCurrentTime.wMonth >= 2)
	{
		for (s32_I_BetweenMonths= stCurrentTime.wMonth+1; s32_I_BetweenMonths<stDeatline.wMonth; s32_I_BetweenMonths++)
		{
			s32_BetweenMonthsDays = s32_BetweenMonthsDays + ars32_NumOfDaysInMonth[s32_I_BetweenMonths];
		}
		s32_CurrentMonthDays = ars32_NumOfDaysInMonth[stCurrentTime.wMonth] - stCurrentTime.wDay;
		s32_LastMonthDays = stDeatline.wDay;

		s32_Retval = s32_CurrentMonthDays + s32_BetweenMonthsDays + s32_LastMonthDays;
	}
	else if (stDeatline.wMonth - stCurrentTime.wMonth == 1)
	{
		s32_BetweenMonthsDays = 0;
		s32_CurrentMonthDays = ars32_NumOfDaysInMonth[stCurrentTime.wMonth] - stCurrentTime.wDay;
		s32_LastMonthDays = stDeatline.wDay;

		s32_Retval = s32_CurrentMonthDays + s32_BetweenMonthsDays + s32_LastMonthDays+1;
	}
	else if (stDeatline.wMonth - stCurrentTime.wMonth == 0)
	{
		s32_Retval = stDeatline.wDay - stCurrentTime.wDay;
	}
	else
	{
		s32_Retval = 0;
	}


	return s32_Retval;

}
