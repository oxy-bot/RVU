#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <windows.h>

using namespace std;

// کنترل‌های رابط کاربری
HWND hInputBox, hSearchButton, hResultLabel, hRadioByCode, hRadioByDescription;
HWND hFeatureTextBox, hProfessionalValueTextBox, hTechnicalValueTextBox;
HWND hDescriptionLabel, hFeatureTitleLabel, hProfessionalValueTitleLabel, hTechnicalValueTitleLabel;
HWND hCodeDescriptionLabel, hCodeDescriptionTitleLabel; // لیبل توضیحات کد

// تابع حذف فاصله‌های اضافی
string Trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return ""; // رشته فقط شامل فاصله است
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// تابع جستجوی دقیق
vector<tuple<string, string, string, string, string, string, string>> Search(const string& query, bool searchByCode) {
    ifstream file("D:\\X.csv"); // مسیر فایل CSV
    if (!file.is_open()) {
        return vector<tuple<string, string, string, string, string, string, string>>(); // بازگشت یک وکتور خالی در صورت عدم موفقیت در باز کردن فایل
    }

    string line;
    vector<tuple<string, string, string, string, string, string, string>> results;
    while (getline(file, line)) {
        size_t pos1 = line.find(','); // اولین کاما
        size_t pos2 = line.find(',', pos1 + 1); // دومین کاما
        size_t pos3 = line.find(',', pos2 + 1); // سومین کاما
        size_t pos4 = line.find(',', pos3 + 1); // چهارمین کاما
        size_t pos5 = line.find(',', pos4 + 1); // پنجمین کاما
        size_t pos6 = line.find(',', pos5 + 1); // ششمین کاما
        size_t pos7 = line.find(',', pos6 + 1); // هفتمین کاما

        if (pos7 != string::npos) {
            string columnA = line.substr(0, pos1);                   // ستون A (کد ارزش نسبی)
            string columnB = line.substr(pos1 + 1, pos2 - pos1 - 1); // ستون B (ویژگی کد)
            string columnC = line.substr(pos2 + 1, pos3 - pos2 - 1); // ستون C (شرح کد)
            string columnD = line.substr(pos3 + 1, pos4 - pos3 - 1); // ستون D (توضیحات کد)
            string columnE = line.substr(pos4 + 1, pos5 - pos4 - 1); // ستون E (ارزش حرفه‌ای)
            string columnF = line.substr(pos5 + 1, pos6 - pos5 - 1); // ستون F (ارزش فنی)
            string columnG = line.substr(pos6 + 1, pos7 - pos6 - 1); // ستون G (ویژگی کد)

            columnA = Trim(columnA);
            columnB = Trim(columnB);
            columnC = Trim(columnC);
            columnD = Trim(columnD);
            columnE = Trim(columnE);
            columnF = Trim(columnF);
            columnG = Trim(columnG);

            if ((searchByCode && columnA == query) || (!searchByCode && columnC.find(query) != string::npos)) {
                results.push_back(make_tuple(columnA, columnB, columnC, columnD, columnE, columnF, columnG)); // افزودن به وکتور نتایج
            }
        }
    }
    return results; // بازگشت نتایج
}

// رویه اصلی پنجره (Window Procedure)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Tahoma");

        // Radio Buttons
        hRadioByCode = CreateWindow("BUTTON", "جستجو با کد ارزش نسبي", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            50, 20, 250, 20, hwnd, (HMENU)2, NULL, NULL);
        SendMessage(hRadioByCode, WM_SETFONT, (WPARAM)hFont, TRUE);

        hRadioByDescription = CreateWindow("BUTTON", "جستجو با شرح خدمت", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            50, 40, 250, 20, hwnd, (HMENU)3, NULL, NULL);
        SendMessage(hRadioByDescription, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hRadioByCode, BM_SETCHECK, BST_CHECKED, 0);

        // Input Box
        hInputBox = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            50, 80, 300, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hInputBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Search Button
        hSearchButton = CreateWindow("BUTTON", "جستجو", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            370, 80, 100, 30, hwnd, (HMENU)1, NULL, NULL);
        SendMessage(hSearchButton, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Result Label (ستون C)
        hResultLabel = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL | WS_HSCROLL,
            50, 130, 400, 200, hwnd, NULL, NULL, NULL);
        SendMessage(hResultLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Feature Title (ویژگی کد)
        hFeatureTitleLabel = CreateWindow("STATIC", "ويژگي کد:", WS_CHILD | WS_VISIBLE,
            480, 130, 150, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hFeatureTitleLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        hFeatureTextBox = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            480, 150, 150, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hFeatureTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Professional Value Title (ارزش حرفه‌ای)
        hProfessionalValueTitleLabel = CreateWindow("STATIC", "ارزش حرفه‌اي:", WS_CHILD | WS_VISIBLE,
            480, 190, 150, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hProfessionalValueTitleLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        hProfessionalValueTextBox = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            480, 210, 150, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hProfessionalValueTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Technical Value Title (ارزش فنی)
        hTechnicalValueTitleLabel = CreateWindow("STATIC", "ارزش فني:", WS_CHILD | WS_VISIBLE,
            480, 250, 150, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hTechnicalValueTitleLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        hTechnicalValueTextBox = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
            480, 270, 150, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hTechnicalValueTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Title for Description Label (توضیحات کد)
        hCodeDescriptionTitleLabel = CreateWindow("STATIC", "توضيحات کد:", WS_CHILD | WS_VISIBLE,
            50, 320, 150, 20, hwnd, NULL, NULL, NULL);
        SendMessage(hCodeDescriptionTitleLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Description Label (توضیحات کد)
        hCodeDescriptionLabel = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER |
            ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL | WS_HSCROLL,
            50, 340, 580, 100, hwnd, NULL, NULL, NULL);
        SendMessage(hCodeDescriptionLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

        // تغییر رنگ پس زمینه فرم
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 253, 208)));

        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) { // وقتی دکمه جستجو کلیک می‌شود
            char queryBuffer[256];
            GetWindowText(hInputBox, queryBuffer, 256); // دریافت مقدار وارد شده
            string query = Trim(queryBuffer);

            bool searchByCode = SendMessage(hRadioByCode, BM_GETCHECK, 0, 0) == BST_CHECKED;

            // جستجوی مقادیر
            vector<tuple<string, string, string, string, string, string, string>> results = Search(query, searchByCode);

            // نمایش نتایج
            string resultText;
            if (!results.empty()) {
                for (const auto& result : results) {
                    resultText += get<0>(result) + " - " + get<2>(result) + "\r\n"; // نمایش کد ارزش نسبی و شرح خدمت
                }
                SetWindowText(hResultLabel, resultText.c_str());

                // نمایش ویژگی کد، ارزش حرفه‌ای و ارزش فنی
                SetWindowText(hFeatureTextBox, get<1>(results[0]).c_str());
                SetWindowText(hProfessionalValueTextBox, get<4>(results[0]).c_str());
                SetWindowText(hTechnicalValueTextBox, get<5>(results[0]).c_str());

                // نمایش توضیحات کد (ستون D)
                string description = get<3>(results[0]); // ستون D
                SetWindowText(hCodeDescriptionLabel, description.empty() ? "توضيحي موجود نيست." : description.c_str());
            }
            else {
                SetWindowText(hResultLabel, "نتیجه‌ای یافت نشد.");
                SetWindowText(hCodeDescriptionLabel, "توضيحي موجود نيست.");
                SetWindowText(hFeatureTextBox, "");
                SetWindowText(hProfessionalValueTextBox, "");
                SetWindowText(hTechnicalValueTextBox, "");
            }
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// تابع WinMain
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindow";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "جستجوي ارزش نسبي", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 500, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
