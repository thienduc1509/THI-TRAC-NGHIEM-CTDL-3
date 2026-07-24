#pragma once
#include "Models.h"
#include "SubjectManager.h"
#include "StudentManager.h"
#include "UIController.h"

// Entry point for the Teacher's UI System
void TeacherMainMenu(SubjectTree& rootSubjects, ClassList& listClasses);

// Phase 2.1: Subject UI
void ManageSubjectsUI(SubjectTree& rootSubjects);
void DrawSubjectList(SubjectTree rootSubjects);

// Phase 2.2: Question UI
void ManageQuestionsUI(SubjectTree rootSubjects);
void DrawQuestionList(SubjectNode* subject);
int GenerateNewQuestionId(SubjectTree rootSubjects);

// Phase 2.3: Class & Student UI
void ManageClassesAndStudentsUI(ClassList& listClasses);
void ManageClassesUI(ClassList& listClasses);
void DrawClassList(ClassList listClasses);

void ManageStudentsUI(ClassList& listClasses);
void DrawStudentList(Class* c);
