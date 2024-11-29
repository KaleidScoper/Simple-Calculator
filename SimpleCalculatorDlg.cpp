#include "pch.h"
#include "framework.h"
#include "SimpleCalculator.h"
#include "SimpleCalculatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSimpleCalculatorDlg::CSimpleCalculatorDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SIMPLECALCULATOR_DIALOG, pParent)
    , m_lastValue(0.0)
    , m_isOperatorPending(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_RESULT, m_editResult);
}

BEGIN_MESSAGE_MAP(CSimpleCalculatorDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSimpleCalculatorDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_EQUAL, &CSimpleCalculatorDlg::OnBnClickedButtonEqual)
    ON_COMMAND_RANGE(IDC_BUTTON_0, IDC_BUTTON_9, &CSimpleCalculatorDlg::OnBnClickedButtonNumber)
    ON_COMMAND_RANGE(IDC_BUTTON_ADD, IDC_BUTTON_MOD, &CSimpleCalculatorDlg::OnBnClickedButtonOperator)
    ON_BN_CLICKED(IDC_BUTTON_DOT, &CSimpleCalculatorDlg::OnBnClickedButtonDot)
END_MESSAGE_MAP()

BOOL CSimpleCalculatorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_editResult.SetWindowText(_T("0"));
    return TRUE;
}

void CSimpleCalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CDialogEx aboutDlg;
        aboutDlg.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CSimpleCalculatorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CSimpleCalculatorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleCalculatorDlg::OnBnClickedButtonNumber(UINT nID)
{
    int number = nID - IDC_BUTTON_0;

    if (m_isOperatorPending)
    {
        m_currentInput.Empty();
        m_isOperatorPending = false;
    }

    CString temp;
    temp.Format(_T("%d"), number);
    m_currentInput += temp;
    m_editResult.SetWindowText(m_currentInput);
}

void CSimpleCalculatorDlg::OnBnClickedButtonDot()
{
    if (m_currentInput.Find('.') == -1)
    {
        if (m_currentInput.IsEmpty())
        {
            m_currentInput = _T("0");
        }

        m_currentInput.Append(_T("."));
        m_editResult.SetWindowText(m_currentInput);
    }
    else
    {
        MessageBeep(MB_ICONWARNING);
    }
}

void CSimpleCalculatorDlg::OnBnClickedButtonClear()
{
    m_currentInput.Empty();
    m_lastValue = 0.0;
    m_lastOperator.Empty();
    m_isOperatorPending = false;

    m_editResult.SetWindowText(_T("0"));
}

void CSimpleCalculatorDlg::OnBnClickedButtonOperator(UINT nID)
{
    CString op;
    switch (nID)
    {
    case IDC_BUTTON_ADD: op = _T("+"); break;
    case IDC_BUTTON_SUB: op = _T("-"); break;
    case IDC_BUTTON_MUL: op = _T("*"); break;
    case IDC_BUTTON_DIV: op = _T("/"); break;
    case IDC_BUTTON_MOD: op = _T("%"); break;
    default: return;
    }

    HandleOperator(op);
}

void CSimpleCalculatorDlg::HandleOperator(const CString& op)
{
    double currentValue = _tstof(m_currentInput);

    if (!m_lastOperator.IsEmpty())
    {
        if (m_lastOperator == _T("+"))
            m_lastValue += currentValue;
        else if (m_lastOperator == _T("-"))
            m_lastValue -= currentValue;
        else if (m_lastOperator == _T("*"))
            m_lastValue *= currentValue;
        else if (m_lastOperator == _T("/"))
        {
            if (currentValue == 0.0)
            {
                AfxMessageBox(_T("Error: Division by zero"));
                return;
            }
            m_lastValue /= currentValue;
        }
        else if (m_lastOperator == _T("%"))
        {
            if (currentValue == 0.0)
            {
                AfxMessageBox(_T("Error: Division by zero"));
                return;
            }
            m_lastValue = fmod(m_lastValue, currentValue);
        }
    }
    else
    {
        m_lastValue = currentValue;
    }

    m_lastOperator = op;
    m_isOperatorPending = true;

    CString result;
    result.Format(_T("%g"), m_lastValue);
    m_editResult.SetWindowText(result);
}

void CSimpleCalculatorDlg::OnBnClickedButtonEqual()
{
    HandleOperator(_T(""));
    m_lastOperator.Empty();
    m_isOperatorPending = false;
}