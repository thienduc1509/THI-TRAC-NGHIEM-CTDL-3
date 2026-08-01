#include "RaylibApp.h"
#include "SubjectManager.h"
#include "StudentManager.h"
#include "FileManager.h"
#include "StudentController.h"
#include "TeacherController.h"
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

#ifdef Rectangle
#undef Rectangle
#endif
#ifdef CloseWindow
#undef CloseWindow
#endif
#ifdef DrawText
#undef DrawText
#endif
#ifdef ShowCursor
#undef ShowCursor
#endif

using namespace std;

// Real-Time Substring / Prefix Live Match Helper
static bool MatchesSearch(const string& text, const string& query) {
    if (query.empty()) return true;
    string textUpper = ToUpper(text);
    string queryUpper = ToUpper(query);
    return (textUpper.find(queryUpper) != string::npos);
}

// Draw Sidebar Navigation Bar (Teacher Mode) with Extra Large Fonts
static void DrawSidebar(AppState& currentState, const string& title, float sw, float sh) {
    float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);

    DrawRectangle(0, 0, (int)sbW, (int)sh, COLOR_CARD);
    DrawLine((int)sbW, 0, (int)sbW, (int)sh, COLOR_BORDER);

    DrawTextCustom("PTITHCM CTDL", 20, 20, 28.0f, COLOR_PRIMARY);
    DrawTextCustom("Thi Trắc Nghiệm C++", 20, 58, 18.0f, COLOR_MUTED);
    DrawLine(20, 88, (int)sbW - 20, 88, COLOR_BORDER);

    auto DrawSideItem = [&](AppState targetState, const char* label, int yPos) {
        bool isActive = (currentState == targetState);
        Color bg = isActive ? COLOR_PRIMARY : COLOR_CARD;
        Color textCol = isActive ? COLOR_BG : COLOR_TEXT;

        if (DrawButton(Rectangle{ 15, (float)yPos, sbW - 30.0f, 52 }, label, bg, textCol)) {
            currentState = targetState;
        }
    };

    DrawSideItem(APP_SUBJECT_MANAGER, "1. Quản lý Môn học", 105);
    DrawSideItem(APP_QUESTION_MANAGER, "2. Quản lý Câu hỏi", 170);
    DrawSideItem(APP_CLASS_MANAGER, "3. Quản lý Lớp & SV", 235);
    DrawSideItem(APP_REPORT_DETAIL_EXAM, "4. Chi tiết Bài thi (h)", 300);
    DrawSideItem(APP_REPORT_CLASS_SCORES, "5. Bảng điểm Lớp (i)", 365);

    if (DrawButton(Rectangle{ 15, sh - 70.0f, sbW - 30.0f, 52 }, "ĐĂNG XUẤT", COLOR_DANGER, COLOR_BG)) {
        currentState = APP_LOGIN;
    }

    // Top Header Banner
    DrawRectangle((int)sbW, 0, (int)(sw - sbW), 88, COLOR_CARD);
    DrawLine((int)sbW, 88, (int)sw, 88, COLOR_BORDER);
    DrawTextCustom(title.c_str(), sbW + 30.0f, 26.0f, 28.0f, COLOR_PRIMARY);
}

void RunRaylibApp(SubjectTree& rootSubjects, ClassList& listClasses) {
    // Enable Window Resizing & Anti-aliasing
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "HỆ THỐNG THI TRẮC NGHIỆM - PTITHCM CTDL");
    SetWindowMinSize(1024, 640);
    SetTargetFPS(60);

    // Initialize Vietnamese Unicode Font
    InitVietnameseFont();

    AppState currentState = APP_LOGIN;

    // Login Form State
    char usernameBuf[64] = "";
    char passwordBuf[64] = "";
    bool userActive = false;
    bool passActive = false;
    string loginErrorMsg = "";

    // Student Logged In Context
    StudentNode* loggedInStudent = nullptr;

    // Subject Form State & Real-Time Search
    char subMamhBuf[32] = "";
    char subTenmhBuf[64] = "";
    bool subMamhActive = false, subTenmhActive = false;
    SubjectNode* selectedSubject = nullptr;
    char searchSubjectBuf[64] = "";
    bool searchSubjectActive = false;

    // Question Form State & Real-Time Search
    char qContentBuf[256] = "";
    char qABuf[128] = "";
    char qBBuf[128] = "";
    char qCBuf[128] = "";
    char qDBuf[128] = "";
    char qAnsBuf[8] = "A";
    bool qContentActive = false, qAActive = false, qBActive = false, qCActive = false, qDActive = false, qAnsActive = false;
    char searchQuestionBuf[64] = "";
    bool searchQuestionActive = false;

    // Class & Student Form State & Real-Time Search
    char classMalopBuf[32] = "";
    char classTenlopBuf[64] = "";
    bool classMalopActive = false, classTenlopActive = false;
    Class* selectedClass = nullptr;
    char searchClassBuf[64] = "";
    bool searchClassActive = false;

    char stMasvBuf[32] = "";
    char stHoBuf[32] = "";
    char stTenBuf[32] = "";
    char stPhaiBuf[16] = "Nam";
    char stPassBuf[32] = "";
    bool stMasvActive = false, stHoActive = false, stTenActive = false, stPhaiActive = false, stPassActive = false;
    StudentNode* selectedStudent = nullptr;
    char searchStudentBuf[64] = "";
    bool searchStudentActive = false;

    // Report Selection State & Real-Time Search
    char rptMasvBuf[32] = "";
    char rptMamhBuf[32] = "";
    bool rptMasvActive = false, rptMamhActive = false;
    char searchReportHBuf[64] = "";
    bool searchReportHActive = false;
    char searchReportIBuf[64] = "";
    bool searchReportIActive = false;

    // Exam Setup State
    char examMamhBuf[32] = "";
    char examNumQBuf[16] = "10";
    char examTimeBuf[16] = "15";
    bool mamhActive = false, numQActive = false, timeActive = false;
    string examErrorMsg = "";

    // Exam Session State
    Question* currentExamQuestions = nullptr;
    char* currentStudentAnswers = nullptr;
    int examTotalQ = 0;
    int examDurationMin = 0;
    int currentQIndex = 0;
    time_t examStartTime = 0;
    time_t examEndTime = 0;
    SubjectNode* activeExamSubject = nullptr;

    // Toast Notification State
    string toastMsg = "";
    float toastTimer = 0.0f;
    Color toastColor = COLOR_SUCCESS;

    auto ShowToast = [&](const string& msg, Color color = COLOR_SUCCESS) {
        toastMsg = msg;
        toastColor = color;
        toastTimer = 3.0f;
    };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (toastTimer > 0.0f) toastTimer -= deltaTime;

        // Dynamic Screen Dimensions
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        BeginDrawing();
        ClearBackground(COLOR_BG);

        // ==========================================
        // 1. LOGIN SCREEN (Large Responsive Card)
        // ==========================================
        if (currentState == APP_LOGIN) {
            float cardW = 540.0f;
            float cardH = 500.0f;
            float cardX = (sw - cardW) / 2.0f;
            float cardY = (sh - cardH) / 2.0f;

            // Keyboard Navigation (TAB to switch focus, ENTER to submit)
            if (IsKeyPressed(KEY_TAB)) {
                if (userActive) {
                    userActive = false;
                    passActive = true;
                } else {
                    userActive = true;
                    passActive = false;
                }
            }

            bool pressEnter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);

            DrawCard(Rectangle{ cardX, cardY, cardW, cardH });
            DrawTextCustom("ĐĂNG NHẬP HỆ THỐNG", cardX + (cardW - MeasureTextCustom("ĐĂNG NHẬP HỆ THỐNG", 32.0f)) / 2.0f, cardY + 35.0f, 32.0f, COLOR_PRIMARY);
            DrawTextCustom("Thi Trắc Nghiệm PTITHCM CTDL", cardX + (cardW - MeasureTextCustom("Thi Trắc Nghiệm PTITHCM CTDL", 20.0f)) / 2.0f, cardY + 78.0f, 20.0f, COLOR_MUTED);

            float inputX = cardX + 50.0f;
            float inputW = cardW - 100.0f;

            DrawTextCustom("Tài khoản / Mã SV:", inputX, cardY + 125.0f, 22.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ inputX, cardY + 155.0f, inputW, 52.0f }, usernameBuf, 32, userActive, "Nhập GV hoặc Mã SV");

            DrawTextCustom("Mật khẩu:", inputX, cardY + 225.0f, 22.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ inputX, cardY + 255.0f, inputW, 52.0f }, passwordBuf, 32, passActive, "Nhập mật khẩu", true);

            if (!loginErrorMsg.empty()) {
                DrawTextCustom(loginErrorMsg.c_str(), inputX, cardY + 325.0f, 20.0f, COLOR_DANGER);
            }

            if (DrawButton(Rectangle{ inputX, cardY + 375.0f, inputW, 58.0f }, "ĐĂNG NHẬP", COLOR_PRIMARY, COLOR_BG) || pressEnter) {
                string userStr = ToUpper(string(usernameBuf));
                string passStr = string(passwordBuf);

                if (userStr == "GV" && passStr == "GV") {
                    loggedInStudent = nullptr;
                    currentState = APP_SUBJECT_MANAGER;
                    loginErrorMsg = "";
                    ShowToast("Đăng nhập Giảng viên thành công!", COLOR_SUCCESS);
                } else {
                    Class* belongClass = nullptr;
                    StudentNode* st = FindStudentGlobal(listClasses, userStr, belongClass);
                    if (st != nullptr && st->data.password == passStr) {
                        loggedInStudent = st;
                        currentState = APP_STUDENT_MENU;
                        loginErrorMsg = "";
                        ShowToast("Đăng nhập Sinh viên thành công!", COLOR_SUCCESS);
                    } else {
                        loginErrorMsg = "Sai Tài khoản hoặc Mật khẩu!";
                    }
                }
            }
        }

        // ==========================================
        // 2. SUBJECT MANAGER SCREEN (BST CRUD & Live Search)
        // ==========================================
        else if (currentState == APP_SUBJECT_MANAGER) {
            DrawSidebar(currentState, "QUẢN LÝ MÔN HỌC (CÂY NHỊ PHÂN TÌM KIẾM BST)", sw, sh);

            float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);
            float cX = sbW + 20.0f;
            float cY = 100.0f;
            float cW = sw - sbW - 40.0f;
            float cH = sh - 120.0f;

            float formW = cW * 0.44f;
            float tableW = cW * 0.53f;

            // Form Left
            DrawCard(Rectangle{ cX, cY, formW, cH });
            DrawTextCustom("THÊM / HIỆU CHỈNH MÔN HỌC", cX + 20.0f, cY + 20.0f, 24.0f, COLOR_PRIMARY);

            float fInputW = formW - 40.0f;
            DrawTextCustom("Mã Môn Học (MAMH):", cX + 20.0f, cY + 65.0f, 20.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 95.0f, fInputW, 48.0f }, subMamhBuf, 15, subMamhActive, "VD: CTDL");

            DrawTextCustom("Tên Môn Học (TENMH):", cX + 20.0f, cY + 160.0f, 20.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 190.0f, fInputW, 48.0f }, subTenmhBuf, 50, subTenmhActive, "VD: Cấu trúc dữ liệu");

            if (DrawButton(Rectangle{ cX + 20.0f, cY + 265.0f, fInputW, 52.0f }, "THÊM MÔN HỌC MỚI", COLOR_SUCCESS, COLOR_BG)) {
                string mamh = ToUpper(string(subMamhBuf));
                string tenmh = NormalizeString(string(subTenmhBuf));

                if (mamh.empty() || tenmh.empty()) {
                    ShowToast("Lỗi: MAMH và TENMH không được bỏ trống!", COLOR_DANGER);
                } else {
                    Subject s; s.MAMH = mamh; s.TENMH = tenmh; s.questions = nullptr;
                    if (InsertSubject(rootSubjects, s)) {
                        SaveSubjects(rootSubjects);
                        ShowToast("Thêm Môn học " + mamh + " thành công!", COLOR_SUCCESS);
                        memset(subMamhBuf, 0, sizeof(subMamhBuf));
                        memset(subTenmhBuf, 0, sizeof(subTenmhBuf));
                    } else {
                        ShowToast("Lỗi: Mã Môn Học hoặc Tên Môn Học đã tồn tại!", COLOR_DANGER);
                    }
                }
            }

            if (selectedSubject != nullptr) {
                if (DrawButton(Rectangle{ cX + 20.0f, cY + 335.0f, fInputW, 52.0f }, "CẬP NHẬT TÊN MÔN HỌC", COLOR_PRIMARY, COLOR_BG)) {
                    string newTen = NormalizeString(string(subTenmhBuf));
                    if (!newTen.empty()) {
                        selectedSubject->data.TENMH = newTen;
                        SaveSubjects(rootSubjects);
                        ShowToast("Cập nhật Tên môn học thành công!", COLOR_SUCCESS);
                    }
                }

                if (DrawButton(Rectangle{ cX + 20.0f, cY + 405.0f, fInputW, 52.0f }, "XÓA MÔN HỌC NÀY", COLOR_DANGER, COLOR_BG)) {
                    if (DeleteSubject(rootSubjects, selectedSubject->data.MAMH)) {
                        SaveSubjects(rootSubjects);
                        ShowToast("Đã xóa Môn học thành công!", COLOR_SUCCESS);
                        selectedSubject = nullptr;
                        memset(subMamhBuf, 0, sizeof(subMamhBuf));
                        memset(subTenmhBuf, 0, sizeof(subTenmhBuf));
                    }
                }
            }

            // Table Right with Live Search Input Bar
            float tX = cX + formW + cW * 0.03f;
            DrawCard(Rectangle{ tX, cY, tableW, cH });
            DrawTextCustom("DANH SÁCH MÔN HỌC (BST)", tX + 20.0f, cY + 16.0f, 22.0f, COLOR_PRIMARY);

            // Real-Time Search Bar
            DrawTextInput(Rectangle{ tX + 20.0f, cY + 52.0f, tableW - 40.0f, 42.0f }, searchSubjectBuf, 64, searchSubjectActive, "🔍 Gõ để lọc theo mã/tên môn học...");

            SubjectNode* arr[500];
            int count = 0;
            CollectSubjectsInOrder(rootSubjects, arr, count);

            float y = cY + 110.0f;
            DrawTextCustom("STT", tX + 20.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("MÃ MÔN", tX + 85.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("TÊN MÔN HỌC", tX + 230.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("CÂU HỎI", tX + tableW - 100.0f, y, 19.0f, COLOR_MUTED);
            DrawLine((int)(tX + 20.0f), (int)(y + 26.0f), (int)(tX + tableW - 20.0f), (int)(y + 26.0f), COLOR_BORDER);
            y += 36.0f;

            string searchKey = string(searchSubjectBuf);
            int displayIdx = 1;
            for (int i = 0; i < count && y < (cY + cH - 35.0f); i++) {
                if (!searchKey.empty() && !MatchesSearch(arr[i]->data.MAMH, searchKey) && !MatchesSearch(arr[i]->data.TENMH, searchKey)) {
                    continue; // Skip items that don't match live search query
                }

                bool isSel = (arr[i] == selectedSubject);
                Rectangle rowRec = { tX + 15.0f, y - 4.0f, tableW - 30.0f, 36.0f };
                if (CheckCollisionPointRec(GetMousePosition(), rowRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    selectedSubject = arr[i];
                    strncpy(subMamhBuf, arr[i]->data.MAMH.c_str(), sizeof(subMamhBuf));
                    strncpy(subTenmhBuf, arr[i]->data.TENMH.c_str(), sizeof(subTenmhBuf));
                }

                if (isSel) DrawRectangleRec(rowRec, COLOR_CARD_HOVER);

                DrawTextCustom(to_string(displayIdx++).c_str(), tX + 20.0f, y, 19.0f, COLOR_TEXT);
                DrawTextCustom(arr[i]->data.MAMH.c_str(), tX + 85.0f, y, 19.0f, COLOR_PRIMARY);
                DrawTextCustom(arr[i]->data.TENMH.c_str(), tX + 230.0f, y, 19.0f, COLOR_TEXT);
                DrawTextCustom(to_string(CountQuestions(arr[i]->data.questions)).c_str(), tX + tableW - 85.0f, y, 19.0f, COLOR_SUCCESS);
                y += 36.0f;
            }
        }

        // ==========================================
        // 3. QUESTION MANAGER SCREEN (Singly Linked List & Live Search)
        // ==========================================
        else if (currentState == APP_QUESTION_MANAGER) {
            DrawSidebar(currentState, "QUẢN LÝ CÂU HỎI THI (DANH SÁCH LIÊN KẾT ĐƠN)", sw, sh);

            float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);
            float cX = sbW + 20.0f;
            float cY = 100.0f;
            float cW = sw - sbW - 40.0f;
            float cH = sh - 120.0f;

            if (selectedSubject == nullptr) {
                DrawCard(Rectangle{ cX, cY, cW, cH });
                DrawTextCustom("Vui lòng chọn 1 Môn học trước khi quản lý câu hỏi!", cX + 50.0f, cY + 50.0f, 24.0f, COLOR_WARNING);
                if (DrawButton(Rectangle{ cX + 50.0f, cY + 100.0f, 300.0f, 52.0f }, "VỀ QUẢN LÝ MÔN HỌC", COLOR_PRIMARY, COLOR_BG)) {
                    currentState = APP_SUBJECT_MANAGER;
                }
            } else {
                float formW = cW * 0.46f;
                float tableW = cW * 0.51f;

                DrawCard(Rectangle{ cX, cY, formW, cH });
                DrawTextCustom(("THÊM CÂU HỎI: " + selectedSubject->data.MAMH).c_str(), cX + 20.0f, cY + 18.0f, 22.0f, COLOR_PRIMARY);

                float fInputW = formW - 40.0f;
                DrawTextCustom("Nội dung câu hỏi:", cX + 20.0f, cY + 52.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 76.0f, fInputW, 42.0f }, qContentBuf, 250, qContentActive, "Nội dung câu hỏi");

                DrawTextCustom("Đáp án A:", cX + 20.0f, cY + 126.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 148.0f, fInputW, 40.0f }, qABuf, 120, qAActive, "Đáp án A");

                DrawTextCustom("Đáp án B:", cX + 20.0f, cY + 196.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 218.0f, fInputW, 40.0f }, qBBuf, 120, qBActive, "Đáp án B");

                DrawTextCustom("Đáp án C:", cX + 20.0f, cY + 266.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 288.0f, fInputW, 40.0f }, qCBuf, 120, qCActive, "Đáp án C");

                DrawTextCustom("Đáp án D:", cX + 20.0f, cY + 336.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 358.0f, fInputW, 40.0f }, qDBuf, 120, qDActive, "Đáp án D");

                DrawTextCustom("Đáp án đúng (A, B, C, D):", cX + 20.0f, cY + 406.0f, 18.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 428.0f, 120.0f, 40.0f }, qAnsBuf, 2, qAnsActive, "A");

                if (DrawButton(Rectangle{ cX + 20.0f, cY + 482.0f, fInputW, 48.0f }, "THÊM CÂU HỎI MỚI", COLOR_SUCCESS, COLOR_BG)) {
                    string content = NormalizeString(string(qContentBuf));
                    string a = NormalizeString(string(qABuf));
                    string b = NormalizeString(string(qBBuf));
                    string c = NormalizeString(string(qCBuf));
                    string d = NormalizeString(string(qDBuf));
                    char ans = toupper(qAnsBuf[0]);

                    if (content.empty() || a.empty() || b.empty() || c.empty() || d.empty() || (ans < 'A' || ans > 'D')) {
                        ShowToast("Lỗi: Nội dung, 4 đáp án và key A/B/C/D phải hợp lệ!", COLOR_DANGER);
                    } else {
                        Question q;
                        q.id = GenerateNewQuestionId(rootSubjects);
                        q.content = content; q.A = a; q.B = b; q.C = c; q.D = d; q.answer = ans;

                        AddQuestion(selectedSubject->data.questions, q);
                        SaveQuestions(rootSubjects);
                        ShowToast("Thêm Câu hỏi ID " + to_string(q.id) + " thành công!", COLOR_SUCCESS);

                        memset(qContentBuf, 0, sizeof(qContentBuf));
                        memset(qABuf, 0, sizeof(qABuf)); memset(qBBuf, 0, sizeof(qBBuf));
                        memset(qCBuf, 0, sizeof(qCBuf)); memset(qDBuf, 0, sizeof(qDBuf));
                    }
                }

                // Question Table Right with Real-Time Search Bar
                float tX = cX + formW + cW * 0.03f;
                DrawCard(Rectangle{ tX, cY, tableW, cH });
                DrawTextCustom("DANH SÁCH CÂU HỎI CỦA MÔN", tX + 20.0f, cY + 16.0f, 22.0f, COLOR_PRIMARY);

                DrawTextInput(Rectangle{ tX + 20.0f, cY + 52.0f, tableW - 40.0f, 40.0f }, searchQuestionBuf, 64, searchQuestionActive, "🔍 Gõ để lọc nội dung hoặc ID...");

                float y = cY + 105.0f;
                QuestionNode* qNode = selectedSubject->data.questions;
                if (qNode == nullptr) {
                    DrawTextCustom("(Môn học này chưa có câu hỏi nào)", tX + 20.0f, y, 20.0f, COLOR_MUTED);
                }

                string searchQKey = string(searchQuestionBuf);
                while (qNode != nullptr && y < (cY + cH - 40.0f)) {
                    if (!searchQKey.empty() && !MatchesSearch(qNode->data.content, searchQKey) && !MatchesSearch(to_string(qNode->data.id), searchQKey)) {
                        qNode = qNode->next;
                        continue; // Skip non-matching questions in real-time
                    }

                    DrawTextCustom(("ID " + to_string(qNode->data.id) + ": " + qNode->data.content).c_str(), tX + 20.0f, y, 19.0f, COLOR_TEXT);
                    DrawTextCustom(("Key: " + string(1, qNode->data.answer)).c_str(), tX + tableW - 120.0f, y, 19.0f, COLOR_SUCCESS);

                    Rectangle delRec = { tX + tableW - 50.0f, y - 2.0f, 36.0f, 30.0f };
                    if (DrawButton(delRec, "X", COLOR_DANGER, COLOR_BG)) {
                        DeleteQuestion(selectedSubject->data.questions, qNode->data.id);
                        SaveQuestions(rootSubjects);
                        ShowToast("Đã xóa câu hỏi!", COLOR_SUCCESS);
                        break;
                    }

                    y += 38.0f;
                    qNode = qNode->next;
                }
            }
        }

        // ==========================================
        // 4. CLASS & STUDENT MANAGER SCREEN (Live Search)
        // ==========================================
        else if (currentState == APP_CLASS_MANAGER) {
            DrawSidebar(currentState, "QUẢN LÝ LỚP HỌC & SINH VIÊN", sw, sh);

            float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);
            float cX = sbW + 20.0f;
            float cY = 100.0f;
            float cW = sw - sbW - 40.0f;
            float cH = sh - 120.0f;

            float leftW = cW * 0.44f;
            float rightW = cW * 0.53f;

            // Left Class Table & Form with Live Search Bar
            DrawCard(Rectangle{ cX, cY, leftW, cH });
            DrawTextCustom("DANH SÁCH LỚP HỌC", cX + 20.0f, cY + 16.0f, 22.0f, COLOR_PRIMARY);

            float fW = (leftW - 50.0f) / 2.0f;
            DrawTextCustom("Mã Lớp:", cX + 20.0f, cY + 52.0f, 17.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 74.0f, fW, 40.0f }, classMalopBuf, 15, classMalopActive, "D18CQCN01");

            DrawTextCustom("Tên Lớp:", cX + 30.0f + fW, cY + 52.0f, 17.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 30.0f + fW, cY + 74.0f, fW, 40.0f }, classTenlopBuf, 30, classTenlopActive, "CNTT 1");

            if (DrawButton(Rectangle{ cX + 20.0f, cY + 125.0f, leftW - 40.0f, 44.0f }, "THÊM LỚP HỌC MỚI", COLOR_SUCCESS, COLOR_BG)) {
                string malop = ToUpper(string(classMalopBuf));
                string tenlop = NormalizeString(string(classTenlopBuf));

                if (!malop.empty() && !tenlop.empty()) {
                    Class c; c.MALOP = malop; c.TENLOP = tenlop; c.students = nullptr;
                    if (AddClass(listClasses, c)) {
                        SaveClasses(listClasses);
                        ShowToast("Thêm Lớp " + malop + " thành công!", COLOR_SUCCESS);
                    } else {
                        ShowToast("Lỗi: Mã Lớp hoặc Tên Lớp đã tồn tại!", COLOR_DANGER);
                    }
                }
            }

            // Real-Time Search Bar for Classes
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 180.0f, leftW - 40.0f, 38.0f }, searchClassBuf, 32, searchClassActive, "🔍 Lọc mã hoặc tên lớp...");

            float y = cY + 230.0f;
            DrawTextCustom("STT", cX + 20.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("MÃ LỚP", cX + 80.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("TÊN LỚP", cX + 230.0f, y, 19.0f, COLOR_MUTED);
            DrawLine((int)(cX + 20.0f), (int)(y + 24.0f), (int)(cX + leftW - 20.0f), (int)(y + 24.0f), COLOR_BORDER);
            y += 34.0f;

            string qClass = string(searchClassBuf);
            int classIdx = 1;
            for (int i = 0; i < listClasses.size && y < (cY + cH - 30.0f); i++) {
                if (listClasses.nodes[i] != nullptr) {
                    if (!qClass.empty() && !MatchesSearch(listClasses.nodes[i]->MALOP, qClass) && !MatchesSearch(listClasses.nodes[i]->TENLOP, qClass)) {
                        continue; // Live filter classes
                    }

                    bool isSel = (listClasses.nodes[i] == selectedClass);
                    Rectangle rowRec = { cX + 15.0f, y - 4.0f, leftW - 30.0f, 34.0f };
                    if (CheckCollisionPointRec(GetMousePosition(), rowRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        selectedClass = listClasses.nodes[i];
                    }

                    if (isSel) DrawRectangleRec(rowRec, COLOR_CARD_HOVER);

                    DrawTextCustom(to_string(classIdx++).c_str(), cX + 20.0f, y, 19.0f, COLOR_TEXT);
                    DrawTextCustom(listClasses.nodes[i]->MALOP.c_str(), cX + 80.0f, y, 19.0f, COLOR_PRIMARY);
                    DrawTextCustom(listClasses.nodes[i]->TENLOP.c_str(), cX + 230.0f, y, 19.0f, COLOR_TEXT);
                    y += 34.0f;
                }
            }

            // Right Student Table & Form with Live Search Bar
            float rX = cX + leftW + cW * 0.03f;
            DrawCard(Rectangle{ rX, cY, rightW, cH });
            if (selectedClass == nullptr) {
                DrawTextCustom("Click chọn 1 Lớp bên trái để quản lý Sinh viên!", rX + 30.0f, cY + 50.0f, 22.0f, COLOR_MUTED);
            } else {
                DrawTextCustom(("SINH VIÊN LỚP: " + selectedClass->MALOP).c_str(), rX + 20.0f, cY + 16.0f, 22.0f, COLOR_PRIMARY);

                float sInputW = (rightW - 70.0f) / 4.0f;
                DrawTextCustom("Mã SV:", rX + 20.0f, cY + 48.0f, 16.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ rX + 20.0f, cY + 68.0f, sInputW, 38.0f }, stMasvBuf, 15, stMasvActive, "N18DCCN001");

                DrawTextCustom("Họ:", rX + 25.0f + sInputW, cY + 48.0f, 16.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ rX + 25.0f + sInputW, cY + 68.0f, sInputW, 38.0f }, stHoBuf, 30, stHoActive, "Nguyễn Văn");

                DrawTextCustom("Tên:", rX + 30.0f + sInputW * 2.0f, cY + 48.0f, 16.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ rX + 30.0f + sInputW * 2.0f, cY + 68.0f, sInputW, 38.0f }, stTenBuf, 15, stTenActive, "An");

                DrawTextCustom("Phái:", rX + 35.0f + sInputW * 3.0f, cY + 48.0f, 16.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ rX + 35.0f + sInputW * 3.0f, cY + 68.0f, sInputW - 10.0f, 38.0f }, stPhaiBuf, 5, stPhaiActive, "Nam");

                DrawTextCustom("Mật khẩu:", rX + 20.0f, cY + 115.0f, 16.0f, COLOR_TEXT);
                DrawTextInput(Rectangle{ rX + 20.0f, cY + 135.0f, rightW * 0.48f, 38.0f }, stPassBuf, 20, stPassActive, "Mật khẩu", true);

                if (DrawButton(Rectangle{ rX + rightW * 0.52f, cY + 135.0f, rightW * 0.43f, 38.0f }, "THÊM SINH VIÊN", COLOR_SUCCESS, COLOR_BG)) {
                    string masv = ToUpper(string(stMasvBuf));
                    string ho = NormalizeString(string(stHoBuf));
                    string ten = NormalizeString(string(stTenBuf));
                    string phai = string(stPhaiBuf);
                    string pass = string(stPassBuf);

                    if (!masv.empty() && !ho.empty() && !ten.empty() && !pass.empty()) {
                        Student s; s.MASV = masv; s.HO = ho; s.TEN = ten; s.PHAI = phai; s.password = pass; s.scores = nullptr;
                        AddStudent(selectedClass->students, s);
                        SaveStudents(listClasses);
                        ShowToast("Thêm Sinh viên " + masv + " thành công!", COLOR_SUCCESS);
                    }
                }

                // Real-Time Search Bar for Students
                DrawTextInput(Rectangle{ rX + 20.0f, cY + 185.0f, rightW - 40.0f, 38.0f }, searchStudentBuf, 32, searchStudentActive, "🔍 Lọc mã SV hoặc họ tên sinh viên...");

                float sy = cY + 235.0f;
                DrawTextCustom("MASV", rX + 20.0f, sy, 18.0f, COLOR_MUTED);
                DrawTextCustom("HỌ TÊN", rX + 160.0f, sy, 18.0f, COLOR_MUTED);
                DrawTextCustom("PHÁI", rX + rightW - 80.0f, sy, 18.0f, COLOR_MUTED);
                DrawLine((int)(rX + 20.0f), (int)(sy + 22.0f), (int)(rX + rightW - 20.0f), (int)(sy + 22.0f), COLOR_BORDER);
                sy += 32.0f;

                string qSt = string(searchStudentBuf);
                StudentNode* stNode = selectedClass->students;
                while (stNode != nullptr && sy < (cY + cH - 25.0f)) {
                    string fullName = stNode->data.HO + " " + stNode->data.TEN;
                    if (!qSt.empty() && !MatchesSearch(stNode->data.MASV, qSt) && !MatchesSearch(fullName, qSt)) {
                        stNode = stNode->next; // Filter student list in real-time
                        continue;
                    }

                    DrawTextCustom(stNode->data.MASV.c_str(), rX + 20.0f, sy, 18.0f, COLOR_PRIMARY);
                    DrawTextCustom(fullName.c_str(), rX + 160.0f, sy, 18.0f, COLOR_TEXT);
                    DrawTextCustom(stNode->data.PHAI.c_str(), rX + rightW - 80.0f, sy, 18.0f, COLOR_MUTED);
                    sy += 32.0f;
                    stNode = stNode->next;
                }
            }
        }

        // ==========================================
        // 5. REPORT DETAIL EXAM SCREEN (Requirement h & Live Search)
        // ==========================================
        else if (currentState == APP_REPORT_DETAIL_EXAM) {
            DrawSidebar(currentState, "IN CHI TIẾT CÂU HỎI ĐÃ THI CỦA 1 SINH VIÊN (MỤC H)", sw, sh);

            float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);
            float cX = sbW + 20.0f;
            float cY = 100.0f;
            float cW = sw - sbW - 40.0f;
            float cH = sh - 120.0f;

            DrawCard(Rectangle{ cX, cY, cW, cH });
            DrawTextCustom("TRA CỨU CHI TIẾT BÀI THI", cX + 20.0f, cY + 18.0f, 24.0f, COLOR_PRIMARY);

            DrawTextCustom("Nhập Mã SV:", cX + 20.0f, cY + 55.0f, 18.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 78.0f, 240.0f, 44.0f }, rptMasvBuf, 15, rptMasvActive, "N18DCCN001");

            DrawTextCustom("Nhập Mã Môn:", cX + 280.0f, cY + 55.0f, 18.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 280.0f, cY + 78.0f, 240.0f, 44.0f }, rptMamhBuf, 15, rptMamhActive, "CTDL");

            if (DrawButton(Rectangle{ cX + 540.0f, cY + 78.0f, 200.0f, 44.0f }, "XEM CHI TIẾT", COLOR_PRIMARY, COLOR_BG)) {
                string masv = ToUpper(string(rptMasvBuf));
                string mamh = ToUpper(string(rptMamhBuf));

                Class* belongClass = nullptr;
                StudentNode* st = FindStudentGlobal(listClasses, masv, belongClass);
                if (st != nullptr) {
                    ScoreNode* sc = FindScore(st->data.scores, mamh);
                    if (sc != nullptr) {
                        selectedStudent = st;
                        ShowToast("Tìm thấy bài thi!", COLOR_SUCCESS);
                    } else {
                        ShowToast("Sinh viên này CHƯA THI môn này!", COLOR_DANGER);
                    }
                } else {
                    ShowToast("Không tìm thấy Sinh viên!", COLOR_DANGER);
                }
            }

            if (selectedStudent != nullptr) {
                string mamh = ToUpper(string(rptMamhBuf));
                ScoreNode* sc = FindScore(selectedStudent->data.scores, mamh);
                if (sc != nullptr) {
                    float dy = cY + 138.0f;
                    DrawTextCustom(("Sinh viên: " + selectedStudent->data.HO + " " + selectedStudent->data.TEN + " - Điểm: " + to_string(sc->data.Diem)).c_str(), cX + 20.0f, dy, 22.0f, COLOR_SUCCESS);
                    dy += 35.0f;

                    // Real-Time Search Bar for Exam Questions
                    DrawTextInput(Rectangle{ cX + 20.0f, dy, cW - 40.0f, 40.0f }, searchReportHBuf, 64, searchReportHActive, "🔍 Gõ để lọc câu hỏi trong bài thi...");
                    dy += 50.0f;

                    string qReportH = string(searchReportHBuf);
                    AnswerDetailNode* ad = sc->data.details;
                    int qIdx = 1;
                    while (ad != nullptr && dy < (cY + cH - 30.0f)) {
                        if (!qReportH.empty() && !MatchesSearch(ad->data.content, qReportH)) {
                            ad = ad->next; // Filter exam questions in real-time
                            qIdx++;
                            continue;
                        }

                        Color ansColor = (ad->data.studentSelection == ad->data.answer) ? COLOR_SUCCESS : COLOR_DANGER;
                        string lineText = "Câu " + to_string(qIdx++) + ": " + ad->data.content + " | SV chọn: " + string(1, ad->data.studentSelection) + " (Đáp án đúng: " + string(1, ad->data.answer) + ")";
                        DrawTextCustom(lineText.c_str(), cX + 20.0f, dy, 18.0f, ansColor);
                        dy += 32.0f;
                        ad = ad->next;
                    }
                }
            }
        }

        // ==========================================
        // 6. REPORT CLASS SCORES SCREEN (Requirement i & Live Search)
        // ==========================================
        else if (currentState == APP_REPORT_CLASS_SCORES) {
            DrawSidebar(currentState, "IN BẢNG ĐIỂM THI TRẮC NGHIỆM CỦA 1 LỚP (MỤC I)", sw, sh);

            float sbW = (sw * 0.24f < 260.0f) ? 260.0f : (sw * 0.24f > 300.0f ? 300.0f : sw * 0.24f);
            float cX = sbW + 20.0f;
            float cY = 100.0f;
            float cW = sw - sbW - 40.0f;
            float cH = sh - 120.0f;

            DrawCard(Rectangle{ cX, cY, cW, cH });
            DrawTextCustom("TRA CỨU BẢNG ĐIỂM THEO LỚP & MÔN HỌC", cX + 20.0f, cY + 18.0f, 24.0f, COLOR_PRIMARY);

            DrawTextCustom("Nhập Mã Lớp:", cX + 20.0f, cY + 55.0f, 18.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 20.0f, cY + 78.0f, 240.0f, 44.0f }, classMalopBuf, 15, classMalopActive, "D18CQCN01");

            DrawTextCustom("Nhập Mã Môn:", cX + 280.0f, cY + 55.0f, 18.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ cX + 280.0f, cY + 78.0f, 240.0f, 44.0f }, rptMamhBuf, 15, rptMamhActive, "CTDL");

            if (DrawButton(Rectangle{ cX + 540.0f, cY + 78.0f, 200.0f, 44.0f }, "IN BẢNG ĐIỂM", COLOR_PRIMARY, COLOR_BG)) {
                string malop = ToUpper(string(classMalopBuf));
                Class* c = FindClass(listClasses, malop);
                if (c != nullptr) {
                    selectedClass = c;
                    ShowToast("Đã tải Bảng điểm lớp " + malop, COLOR_SUCCESS);
                } else {
                    ShowToast("Lỗi: Không tìm thấy Lớp!", COLOR_DANGER);
                }
            }

            if (selectedClass != nullptr) {
                string mamh = ToUpper(string(rptMamhBuf));

                // Real-Time Search Bar for Class Score Table
                DrawTextInput(Rectangle{ cX + 20.0f, cY + 135.0f, cW - 40.0f, 40.0f }, searchReportIBuf, 64, searchReportIActive, "🔍 Gõ để lọc theo mã SV hoặc họ tên...");

                float y = cY + 190.0f;
                DrawTextCustom("STT", cX + 20.0f, y, 19.0f, COLOR_MUTED);
                DrawTextCustom("MASV", cX + 90.0f, y, 19.0f, COLOR_MUTED);
                DrawTextCustom("HỌ TÊN", cX + 260.0f, y, 19.0f, COLOR_MUTED);
                DrawTextCustom("ĐIỂM THI", cX + 600.0f, y, 19.0f, COLOR_MUTED);
                DrawLine((int)(cX + 20.0f), (int)(y + 26.0f), (int)(cX + cW - 20.0f), (int)(y + 26.0f), COLOR_BORDER);
                y += 36.0f;

                string qReportI = string(searchReportIBuf);
                int idx = 1;
                StudentNode* st = selectedClass->students;
                while (st != nullptr && y < (cY + cH - 30.0f)) {
                    string fullName = st->data.HO + " " + st->data.TEN;
                    if (!qReportI.empty() && !MatchesSearch(st->data.MASV, qReportI) && !MatchesSearch(fullName, qReportI)) {
                        st = st->next; // Filter score table in real-time
                        continue;
                    }

                    ScoreNode* sc = FindScore(st->data.scores, mamh);

                    DrawTextCustom(to_string(idx++).c_str(), cX + 20.0f, y, 19.0f, COLOR_TEXT);
                    DrawTextCustom(st->data.MASV.c_str(), cX + 90.0f, y, 19.0f, COLOR_PRIMARY);
                    DrawTextCustom(fullName.c_str(), cX + 260.0f, y, 19.0f, COLOR_TEXT);

                    if (sc != nullptr) {
                        DrawTextCustom((to_string(sc->data.Diem) + " / 10.0").c_str(), cX + 600.0f, y, 19.0f, COLOR_SUCCESS);
                    } else {
                        DrawTextCustom("Chưa thi", cX + 600.0f, y, 19.0f, COLOR_DANGER);
                    }

                    y += 34.0f;
                    st = st->next;
                }
            }
        }

        // ==========================================
        // 7. STUDENT DASHBOARD MENU
        // ==========================================
        else if (currentState == APP_STUDENT_MENU) {
            DrawHeader("MENU SINH VIÊN", ("Xin chào, " + loggedInStudent->data.HO + " " + loggedInStudent->data.TEN).c_str(), sw);

            if (DrawButton(Rectangle{ sw - 180.0f, 20.0f, 140.0f, 46.0f }, "ĐĂNG XUẤT", COLOR_DANGER, COLOR_BG)) {
                currentState = APP_LOGIN;
            }

            float cardW = (sw - 100.0f) / 2.0f;
            float cardH = sh - 160.0f;

            // Left Setup Card
            DrawCard(Rectangle{ 40.0f, 120.0f, cardW, cardH });
            DrawTextCustom("THI TRẮC NGHIỆM", 60.0f, 145.0f, 26.0f, COLOR_PRIMARY);

            float fW = cardW - 40.0f;
            DrawTextCustom("Mã Môn Học:", 60.0f, 195.0f, 20.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ 60.0f, 222.0f, fW, 46.0f }, examMamhBuf, 15, mamhActive, "VD: CTDL");

            DrawTextCustom("Số câu hỏi thi:", 60.0f, 280.0f, 20.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ 60.0f, 307.0f, fW, 46.0f }, examNumQBuf, 5, numQActive, "Số câu (VD: 10)");

            DrawTextCustom("Thời gian thi (Phút):", 60.0f, 365.0f, 20.0f, COLOR_TEXT);
            DrawTextInput(Rectangle{ 60.0f, 392.0f, fW, 46.0f }, examTimeBuf, 5, timeActive, "Thời gian (VD: 15)");

            if (!examErrorMsg.empty()) {
                DrawTextCustom(examErrorMsg.c_str(), 60.0f, 450.0f, 18.0f, COLOR_DANGER);
            }

            if (DrawButton(Rectangle{ 60.0f, 490.0f, fW, 54.0f }, "BẮT ĐẦU LÀM BÀI THI", COLOR_SUCCESS, COLOR_BG)) {
                string mamh = ToUpper(string(examMamhBuf));
                SubjectNode* sub = SearchSubject(rootSubjects, mamh);

                if (sub == nullptr) {
                    examErrorMsg = "Lỗi: Mã Môn Học không tồn tại!";
                } else if (FindScore(loggedInStudent->data.scores, mamh) != nullptr) {
                    examErrorMsg = "Lỗi: Bạn đã thi môn này rồi!";
                } else {
                    int numQ = atoi(examNumQBuf);
                    int timeMin = atoi(examTimeBuf);
                    int availableQ = CountQuestions(sub->data.questions);

                    if (numQ <= 0 || timeMin <= 0) {
                        examErrorMsg = "Lỗi: Số câu và thời gian phải > 0!";
                    } else if (numQ > availableQ) {
                        int missing = numQ - availableQ;
                        examErrorMsg = "Lỗi: Môn này chưa đủ câu! (Hiện có " + to_string(availableQ) + ", thiếu " + to_string(missing) + ")";
                    } else {
                        activeExamSubject = sub;
                        examTotalQ = numQ;
                        examDurationMin = timeMin;

                        int actualCount = 0;
                        currentExamQuestions = GetRandomQuestions(sub, numQ, actualCount);
                        currentStudentAnswers = new char[actualCount];
                        for (int i = 0; i < actualCount; i++) currentStudentAnswers[i] = ' ';

                        examStartTime = time(0);
                        examEndTime = examStartTime + (timeMin * 60);
                        currentQIndex = 0;
                        currentState = APP_TAKE_EXAM;
                        examErrorMsg = "";
                    }
                }
            }

            // Right Scores Card
            float rX = 60.0f + cardW;
            DrawCard(Rectangle{ rX, 120.0f, cardW, cardH });
            DrawTextCustom("BẢNG ĐIỂM CÁC MÔN ĐÃ THI", rX + 20.0f, 145.0f, 26.0f, COLOR_PRIMARY);
            DrawLine((int)(rX + 20.0f), 180, (int)(rX + cardW - 20.0f), 180, COLOR_BORDER);

            float y = 200.0f;
            DrawTextCustom("MÃ MÔN HỌC", rX + 20.0f, y, 19.0f, COLOR_MUTED);
            DrawTextCustom("ĐIỂM SỐ", rX + cardW - 140.0f, y, 19.0f, COLOR_MUTED);
            DrawLine((int)(rX + 20.0f), (int)(y + 24.0f), (int)(rX + cardW - 20.0f), (int)(y + 24.0f), COLOR_BORDER);
            y += 36.0f;

            ScoreNode* sc = loggedInStudent->data.scores;
            if (sc == nullptr) {
                DrawTextCustom("(Bạn chưa thi môn nào)", rX + 20.0f, y, 19.0f, COLOR_MUTED);
            }
            while (sc != nullptr && y < (120.0f + cardH - 35.0f)) {
                DrawTextCustom(sc->data.Mamh.c_str(), rX + 20.0f, y, 20.0f, COLOR_TEXT);
                DrawTextCustom((to_string(sc->data.Diem) + " / 10.0").c_str(), rX + cardW - 140.0f, y, 20.0f, COLOR_SUCCESS);
                y += 36.0f;
                sc = sc->next;
            }
        }

        // ==========================================
        // 8. REAL-TIME EXAM HUD SCREEN
        // ==========================================
        else if (currentState == APP_TAKE_EXAM) {
            time_t now = time(0);
            int secondsLeft = (int)difftime(examEndTime, now);
            if (secondsLeft <= 0) secondsLeft = 0;

            DrawCard(Rectangle{ 40.0f, 20.0f, sw - 80.0f, 75.0f }, COLOR_CARD, COLOR_PRIMARY);
            DrawTextCustom(("MÔN THI: " + activeExamSubject->data.TENMH).c_str(), 60.0f, 42.0f, 24.0f, COLOR_PRIMARY);

            string timerStr = "THỜI GIAN CÒN LẠI: " + to_string(secondsLeft / 60) + " phút " + to_string(secondsLeft % 60) + " giây";
            DrawTextCustom(timerStr.c_str(), sw - 520.0f, 42.0f, 22.0f, (secondsLeft < 60) ? COLOR_DANGER : COLOR_WARNING);

            float qW = sw - 420.0f;
            float qH = sh - 130.0f;
            DrawCard(Rectangle{ 40.0f, 110.0f, qW, qH });
            Question& q = currentExamQuestions[currentQIndex];

            DrawTextCustom(("Câu hỏi " + to_string(currentQIndex + 1) + " / " + to_string(examTotalQ) + ":").c_str(), 60.0f, 135.0f, 22.0f, COLOR_PRIMARY);
            DrawTextCustom(q.content.c_str(), 60.0f, 175.0f, 22.0f, COLOR_TEXT);

            auto DrawChoiceBtn = [&](int idx, char key, const string& val, float yPos) {
                bool isSelected = (currentStudentAnswers[currentQIndex] == key);
                Color btnBg = isSelected ? COLOR_PRIMARY : COLOR_CARD_HOVER;
                Color txtCol = isSelected ? COLOR_BG : COLOR_TEXT;

                string label = string(1, key) + ". " + val;
                if (DrawButton(Rectangle{ 60.0f, yPos, qW - 40.0f, 56.0f }, label.c_str(), btnBg, txtCol)) {
                    currentStudentAnswers[currentQIndex] = key;
                }
            };

            DrawChoiceBtn(0, 'A', q.A, 235.0f);
            DrawChoiceBtn(1, 'B', q.B, 305.0f);
            DrawChoiceBtn(2, 'C', q.C, 375.0f);
            DrawChoiceBtn(3, 'D', q.D, 445.0f);

            if (currentQIndex > 0) {
                if (DrawButton(Rectangle{ 60.0f, 110.0f + qH - 65.0f, 160.0f, 48.0f }, "< CÂU TRƯỚC", COLOR_CARD, COLOR_TEXT)) {
                    currentQIndex--;
                }
            }
            if (currentQIndex < examTotalQ - 1) {
                if (DrawButton(Rectangle{ 40.0f + qW - 200.0f, 110.0f + qH - 65.0f, 160.0f, 48.0f }, "CÂU TIẾP >", COLOR_CARD, COLOR_TEXT)) {
                    currentQIndex++;
                }
            }

            // Right Nav Grid Sidebar
            float rX = 60.0f + qW;
            float rW = sw - rX - 40.0f;
            DrawCard(Rectangle{ rX, 110.0f, rW, qH });
            DrawTextCustom("DANH SÁCH CÂU HỎI", rX + 15.0f, 135.0f, 20.0f, COLOR_PRIMARY);

            float startX = rX + 15.0f;
            float startY = 175.0f;
            int col = 0;
            int row = 0;

            for (int i = 0; i < examTotalQ; i++) {
                Rectangle btnRec = { startX + col * 56.0f, startY + row * 50.0f, 48.0f, 42.0f };
                bool isAns = (currentStudentAnswers[i] != ' ');
                bool isCurr = (i == currentQIndex);

                Color bg = isCurr ? COLOR_PRIMARY : (isAns ? COLOR_SUCCESS : COLOR_CARD_HOVER);
                Color fg = (isCurr || isAns) ? COLOR_BG : COLOR_TEXT;

                if (DrawButton(btnRec, to_string(i + 1).c_str(), bg, fg)) {
                    currentQIndex = i;
                }

                col++;
                if ((startX + (col + 1) * 56.0f) > (rX + rW - 10.0f)) { col = 0; row++; }
            }

            if (DrawButton(Rectangle{ rX + 15.0f, 110.0f + qH - 65.0f, rW - 30.0f, 52.0f }, "NỘP BÀI THI (F10)", COLOR_SUCCESS, COLOR_BG) || secondsLeft <= 0) {
                int correctCount = 0;
                Score sc;
                sc.Mamh = activeExamSubject->data.MAMH;
                sc.details = nullptr;

                for (int i = 0; i < examTotalQ; i++) {
                    if (currentStudentAnswers[i] == currentExamQuestions[i].answer) {
                        correctCount++;
                    }

                    AnswerDetail ad;
                    ad.questionId = currentExamQuestions[i].id;
                    ad.content = currentExamQuestions[i].content;
                    ad.A = currentExamQuestions[i].A;
                    ad.B = currentExamQuestions[i].B;
                    ad.C = currentExamQuestions[i].C;
                    ad.D = currentExamQuestions[i].D;
                    ad.answer = currentExamQuestions[i].answer;
                    ad.studentSelection = currentStudentAnswers[i];
                    AddAnswerDetail(sc.details, ad);
                }

                sc.Diem = ((float)correctCount / examTotalQ) * 10.0f;
                AddScore(loggedInStudent->data.scores, sc);

                SaveScores(listClasses);

                delete[] currentStudentAnswers;
                delete[] currentExamQuestions;
                currentStudentAnswers = nullptr;
                currentExamQuestions = nullptr;

                currentState = APP_STUDENT_MENU;
                ShowToast("Nộp bài thành công! Điểm của bạn: " + to_string(sc.Diem), COLOR_SUCCESS);
            }
        }

        // Draw Overlay Toast
        if (toastTimer > 0.0f) {
            float toastW = 500.0f;
            float toastX = (sw - toastW) / 2.0f;
            DrawCard(Rectangle{ toastX, sh - 80.0f, toastW, 55.0f }, toastColor, COLOR_TEXT);
            float textW = MeasureTextCustom(toastMsg.c_str(), 20.0f);
            DrawTextCustom(toastMsg.c_str(), toastX + (toastW - textW) / 2.0f, sh - 63.0f, 20.0f, COLOR_BG);
        }

        EndDrawing();
    }

    // Clean Font Memory
    UnloadVietnameseFont();

    CloseWindow();
}
