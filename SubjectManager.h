#pragma once
#include "Models.h"

// ==========================================
// QUESTION MANAGEMENT (Singly Linked List)
// ==========================================
int CountQuestions(QuestionNode* head);
QuestionNode* FindQuestion(QuestionNode* head, int id);
void AddQuestion(QuestionNode*& head, Question data);
bool EditQuestion(QuestionNode* head, int id, string content, string A, string B, string C, string D, char ans);
bool DeleteQuestion(QuestionNode*& head, int id);
void FreeQuestionList(QuestionNode*& head); // Memory Cleanup

// ==========================================
// SUBJECT MANAGEMENT (Binary Search Tree)
// ==========================================
SubjectNode* CreateSubjectNode(Subject data);
SubjectNode* SearchSubject(SubjectTree root, string mamh);
SubjectNode* SearchSubjectByName(SubjectTree root, string tenmh);
bool InsertSubject(SubjectTree& root, Subject data);
bool EditSubject(SubjectTree root, string oldMamh, string newTenMh);
void FindAndReplaceSuccessor(SubjectTree& p, SubjectTree& succ); // Helper for Delete
bool DeleteSubject(SubjectTree& root, string mamh);
void CollectSubjectsInOrder(SubjectTree root, SubjectNode* arr[], int& count); // For UI menus
void FreeSubjectTree(SubjectTree& root); // Memory Cleanup
