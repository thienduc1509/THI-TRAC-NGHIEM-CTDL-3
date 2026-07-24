#pragma once
#include "SubjectManager.h"
#include "StudentManager.h"
#include <string>

using namespace std;

// ==========================================
// FILE I/O MANAGER
// ==========================================
// Sinh vien beginner note: 
// We use a pipe character '|' to separate fields instead of commas or spaces. 
// This prevents reading errors when a Student Name or Question Content contains spaces or commas.

bool LoadSubjects(SubjectTree& root);
bool SaveSubjects(SubjectTree root);

bool LoadQuestions(SubjectTree root);
bool SaveQuestions(SubjectTree root);

bool LoadClasses(ClassList& dsLop);
bool SaveClasses(ClassList dsLop);

bool LoadStudents(ClassList& dsLop);
bool SaveStudents(ClassList dsLop);

bool LoadScores(ClassList& dsLop);
bool SaveScores(ClassList dsLop);
