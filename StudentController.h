#pragma once
#include "Models.h"
#include "SubjectManager.h"
#include "StudentManager.h"
#include "UIController.h"

// Entry point for Student Login
void StudentLoginFlow(ClassList& listClasses, SubjectTree& rootSubjects);

// Main Student Menu
void StudentMainMenu(StudentNode* student, SubjectTree& rootSubjects);

// Exam Engine (Using raw dynamic arrays - No STL vector/stack/queue)
Question* GetRandomQuestions(SubjectNode* subject, int numQuestions, int& actualCount);
void SetupExamUI(StudentNode* student, SubjectTree rootSubjects);
void TakeExamLoop(StudentNode* student, SubjectNode* subject, Question* examQuestions, int totalQuestions, int durationMinutes);
