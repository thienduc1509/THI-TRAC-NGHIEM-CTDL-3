#include "SubjectManager.h"

// ==========================================
// QUESTION MANAGEMENT (Singly Linked List)
// ==========================================

int CountQuestions(QuestionNode* head) {
    int count = 0;
    while (head != nullptr) {
        count++;
        head = head->next;
    }
    return count;
}

QuestionNode* FindQuestion(QuestionNode* head, int id) {
    while (head != nullptr) {
        if (head->data.id == id) return head;
        head = head->next;
    }
    return nullptr;
}

void AddQuestion(QuestionNode*& head, Question data) {
    QuestionNode* newNode = new QuestionNode{data, nullptr};
    if (head == nullptr) {
        head = newNode;
        return;
    }
    QuestionNode* p = head;
    while (p->next != nullptr) p = p->next;
    p->next = newNode;
}

bool EditQuestion(QuestionNode* head, int id, string content, string A, string B, string C, string D, char ans) {
    QuestionNode* p = FindQuestion(head, id);
    if (p != nullptr) {
        p->data.content = content;
        p->data.A = A;
        p->data.B = B;
        p->data.C = C;
        p->data.D = D;
        p->data.answer = ans;
        return true;
    }
    return false;
}

bool DeleteQuestion(QuestionNode*& head, int id) {
    if (head == nullptr) return false;
    if (head->data.id == id) {
        QuestionNode* temp = head;
        head = head->next;
        delete temp;
        return true;
    }
    QuestionNode* p = head;
    while (p->next != nullptr && p->next->data.id != id) {
        p = p->next;
    }
    if (p->next != nullptr) {
        QuestionNode* temp = p->next;
        p->next = temp->next;
        delete temp;
        return true;
    }
    return false;
}

void FreeQuestionList(QuestionNode*& head) {
    while (head != nullptr) {
        QuestionNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// ==========================================
// SUBJECT MANAGEMENT (Binary Search Tree)
// ==========================================

SubjectNode* CreateSubjectNode(Subject data) {
    SubjectNode* p = new SubjectNode;
    p->data = data;
    p->left = nullptr;
    p->right = nullptr;
    return p;
}

SubjectNode* SearchSubject(SubjectTree root, string mamh) {
    if (root == nullptr) return nullptr;
    if (root->data.MAMH == mamh) return root;
    if (mamh < root->data.MAMH) return SearchSubject(root->left, mamh);
    return SearchSubject(root->right, mamh);
}

SubjectNode* SearchSubjectByName(SubjectTree root, string tenmh) {
    if (root == nullptr) return nullptr;
    if (root->data.TENMH == tenmh) return root;
    SubjectNode* leftRes = SearchSubjectByName(root->left, tenmh);
    if (leftRes != nullptr) return leftRes;
    return SearchSubjectByName(root->right, tenmh);
}

bool InsertSubject(SubjectTree& root, Subject data) {
    if (SearchSubjectByName(root, data.TENMH) != nullptr) return false; // Duplicate Subject Name
    if (root == nullptr) {
        root = CreateSubjectNode(data);
        return true;
    }
    if (root->data.MAMH == data.MAMH) return false; // Duplicate Subject Code
    if (data.MAMH < root->data.MAMH) return InsertSubject(root->left, data);
    return InsertSubject(root->right, data);
}

bool EditSubject(SubjectTree root, string oldMamh, string newTenMh) {
    SubjectNode* node = SearchSubject(root, oldMamh);
    if (node) {
        node->data.TENMH = newTenMh;
        return true;
    }
    return false;
}

void FindAndReplaceSuccessor(SubjectTree& p, SubjectTree& succ) {
    if (succ->left != nullptr) {
        FindAndReplaceSuccessor(p, succ->left);
    } else {
        p->data = succ->data;
        p = succ;
        succ = succ->right;
    }
}

bool DeleteSubject(SubjectTree& root, string mamh) {
    if (root == nullptr) return false;
    if (mamh < root->data.MAMH) return DeleteSubject(root->left, mamh);
    if (mamh > root->data.MAMH) return DeleteSubject(root->right, mamh);
    
    // Found it
    SubjectNode* p = root;
    if (root->left == nullptr) root = root->right;
    else if (root->right == nullptr) root = root->left;
    else FindAndReplaceSuccessor(p, root->right);
    
    // Cleanup questions before deleting node
    FreeQuestionList(p->data.questions);
    delete p;
    return true;
}

void CollectSubjectsInOrder(SubjectTree root, SubjectNode* arr[], int& count) {
    if (root != nullptr) {
        CollectSubjectsInOrder(root->left, arr, count);
        arr[count++] = root;
        CollectSubjectsInOrder(root->right, arr, count);
    }
}

void FreeSubjectTree(SubjectTree& root) {
    if (root != nullptr) {
        FreeSubjectTree(root->left);
        FreeSubjectTree(root->right);
        FreeQuestionList(root->data.questions); // Free inner list
        delete root;
        root = nullptr;
    }
}
