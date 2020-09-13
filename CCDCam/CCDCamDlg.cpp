
// CCDCamDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CCDCam.h"
#include "CCDCamDlg.h"
#include "afxdialogex.h"
#include "BitranCCDlib.h"
#include "Imaging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCCDCamDlg 对话框



CCCDCamDlg::CCCDCamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCDCAM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCCDCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Picture, m_picCtrl);
}

BEGIN_MESSAGE_MAP(CCCDCamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Connect, &CCCDCamDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_EXPOSE, &CCCDCamDlg::OnBnClickedButtonExpose)
END_MESSAGE_MAP()


// CCCDCamDlg 消息处理程序

BOOL CCCDCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitBitranCCDlib("");


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCCDCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCCDCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCCDCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CCCDCamDlg::CameraConnect()
{
	const int index = 0;

	int width, height;
	int result = BitranCCDlibCreate[index](BitranCCDlibCamera[index], &width, &height);

	//printf("c%-d%-d%", BitranCCDlibCamera[i], &width, &height);
	//printf("c%\n", BitranCCDlibCamera[i]);
	//printf("d%\n", &width);
	//printf("d%\n", &height);


	if (result != 1)
	return false;

	char info[100];
	int lng = BitranCCDlibCameraInfo[index](1, info, sizeof(info));
	if (lng == 0)
	{
		MessageBox(_T("BitranCCDlibCameraInfo failed!"));
		return false;
	}


	USES_CONVERSION;
	CString strInfo = A2T(info) + CString(_T("\r\n"));


	char* camera = strstr(info, "BU-6");
	if (camera == NULL)
	{
		MessageBox(_T("camera type error!"));
		return false;
	}

	int type = atoi(&camera[3]);
	CameraType = (type == 60) ? 0 : (type == 66) ? 2 : 1;
	ImageColor = camera[5] == 'C';
	pImageData[index] = new WORD[width * height];


	BitranCCDlibCameraInfo[index](0, info, sizeof(info));
	strInfo += A2T(info);
	

	MessageBox(strInfo);


	return true;
}


void CCCDCamDlg::OnBnClickedButtonConnect()
{
	isConnect = CameraConnect();
}


static BOOL CreateStretchImage(const CImage &src, CImage &dest, int stretchWidth, int stretchHeight)
{
	if (src.IsDIBSection())
	{
		dest.Create(stretchWidth, stretchHeight, 32);

		HDC destDC = dest.GetDC();
		::SetStretchBltMode(destDC, HALFTONE);
		src.StretchBlt(destDC, 0, 0, stretchWidth, stretchHeight);
		dest.ReleaseDC();
		return TRUE;
	}
	return FALSE;
}

void CCCDCamDlg::OnBnClickedButtonExpose()
{
	if (!isConnect)
	{
		MessageBox(_T("Please connect first!"));
		return;
	}


	ExposeInfo ExposeParam = {0};
	ExposeParam.time = 30;
	ExposeParam.binning = 1;
	ExposeParam.center = 0;
	ExposeParam.mode = 0;
	ExposeParam.trigger = 0;
	ExposeParam.emgain = 0;
	ExposeParam.gainup = -1;
	ExposeParam.fandelay = -1;
	ExposeParam.abort = false;


	
	int ImageWidth = 0;
	int ImageHeight = 0;
	if (SnapExposure(&ExposeParam, pImageData[0], NULL, NULL, ImageWidth, ImageHeight))
	{
		if (ImageColor > 0)
			ImageColor = (ExposeParam.binning > 1) + 1;

		/*if (hBitmap)
		{
			::DeleteObject(hBitmap);
		}*/

		hBitmap = ImageConversion(ConvertMode,
			ImageColor == 1, pImageData[0], ImageWidth * ImageHeight);


		CImage orgImage;
		orgImage.Attach(hBitmap);
		CImage showImage;
		CreateStretchImage(orgImage, showImage, ImageWidth / 2, ImageHeight / 2);

		HBITMAP hShowBmp = showImage.Detach();
		HBITMAP oldBmp = m_picCtrl.GetBitmap();
		m_picCtrl.SetBitmap(hShowBmp);
		if (oldBmp != NULL) {
			::DeleteObject(oldBmp);
		}
	}

}
