#pragma once

class CSimpleCalculatorDlg : public CDialogEx
{
public:
    CSimpleCalculatorDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SIMPLECALCULATOR_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;

    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    CString m_currentInput;         // 当前输入的数字或表达式
    CString m_lastOperator;         // 上一次的操作符
    double m_lastValue;             // 上一次计算的值
    bool m_isOperatorPending;       // 是否在等待操作符

    CEdit m_editResult;             // 用于显示结果的编辑框

    void HandleOperator(const CString& op);

public:
    afx_msg void OnBnClickedButtonNumber(UINT nID); // 数字按钮点击事件
    afx_msg void OnBnClickedButtonClear();          // 清除按钮点击事件
    afx_msg void OnBnClickedButtonOperator(UINT nID); // 操作符按钮点击事件
    afx_msg void OnBnClickedButtonEqual();          // 等于按钮点击事件
    afx_msg void OnBnClickedButtonDot();            // 小数点按钮点击事件
};
