#pragma once
#include "Models.h"
#include "RaylibUI.h"

enum AppState {
    APP_LOGIN,
    APP_SUBJECT_MANAGER,
    APP_QUESTION_MANAGER,
    APP_CLASS_MANAGER,
    APP_REPORT_DETAIL_EXAM,
    APP_REPORT_CLASS_SCORES,
    APP_STUDENT_MENU,
    APP_TAKE_EXAM
};

// Main Raylib Graphical Application Loop
void RunRaylibApp(SubjectTree& rootSubjects, ClassList& listClasses);
