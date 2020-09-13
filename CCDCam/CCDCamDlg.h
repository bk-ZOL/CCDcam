
// CCDCamDlg.h: 头文件
//

#pragma once


#include "types.h"





// CCCDCamDlg 对话框
class CCCDCamDlg : public CDialogEx
{
// 构造
public:
	CCCDCamDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCDCAM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	bool CameraConnect();


	int CameraType = 0;
	int ImageColor = 0;
	int ConvertMode = 0;
	HBITMAP hBitmap = NULL;
	CStatic m_picCtrl;
	BOOL isConnect = FALSE;

public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonExpose();
};
