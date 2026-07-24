#include "FileManager.h"
#include <fstream>
#include <sstream>

// Helper to safely split strings by a delimiter into a plain array (No STL containers)
int SplitString(const string& str, char delimiter, string tokens[], int maxTokens) {
    int count = 0;
    string token = "";
    for (char ch : str) {
        if (ch == delimiter) {
            if (count < maxTokens) {
                tokens[count++] = token;
                token = "";
            }
        } else {
            token += ch;
        }
    }
    if (count < maxTokens) {
        tokens[count++] = token;
    }
    return count;
}

// ==========================================
// SUBJECTS
// ==========================================
bool LoadSubjects(SubjectTree& root) {
    ifstream file("monhoc.txt");
    if (!file.is_open()) return false;
    string line;
    string tokens[10];
    while (getline(file, line)) {
        if (line.empty()) continue;
        int count = SplitString(line, '|', tokens, 10);
        if (count >= 2) {
            Subject s;
            s.MAMH = tokens[0];
            s.TENMH = tokens[1];
            InsertSubject(root, s);
        }
    }
    file.close();
    return true;
}

// Helper for saving BST (Pre-order traversal writing)
void SaveSubjectRecursive(SubjectTree root, ofstream& file) {
    if (root != nullptr) {
        file << root->data.MAMH << "|" << root->data.TENMH << "\n";
        SaveSubjectRecursive(root->left, file);
        SaveSubjectRecursive(root->right, file);
    }
}

bool SaveSubjects(SubjectTree root) {
    ofstream file("monhoc.txt");
    if (!file.is_open()) return false;
    SaveSubjectRecursive(root, file);
    file.close();
    return true;
}

// ==========================================
// QUESTIONS
// ==========================================
bool LoadQuestions(SubjectTree root) {
    ifstream file("cauhoi.txt");
    if (!file.is_open()) return false;
    string line;
    string tokens[10];
    while (getline(file, line)) {
        if (line.empty()) continue;
        int count = SplitString(line, '|', tokens, 10);
        // Format: MAMH | ID | Content | A | B | C | D | Ans
        if (count >= 8) {
            string mamh = tokens[0];
            Question q;
            q.id = stoi(tokens[1]);
            q.content = tokens[2];
            q.A = tokens[3];
            q.B = tokens[4];
            q.C = tokens[5];
            q.D = tokens[6];
            q.answer = tokens[7][0]; // char
            
            // Find subject and inject question into its linked list
            SubjectNode* subNode = SearchSubject(root, mamh);
            if (subNode != nullptr) {
                AddQuestion(subNode->data.questions, q);
            }
        }
    }
    file.close();
    return true;
}

void SaveQuestionsRecursive(SubjectTree root, ofstream& file) {
    if (root != nullptr) {
        QuestionNode* p = root->data.questions;
        while (p != nullptr) {
            file << root->data.MAMH << "|"
                 << p->data.id << "|"
                 << p->data.content << "|"
                 << p->data.A << "|"
                 << p->data.B << "|"
                 << p->data.C << "|"
                 << p->data.D << "|"
                 << p->data.answer << "\n";
            p = p->next;
        }
        SaveQuestionsRecursive(root->left, file);
        SaveQuestionsRecursive(root->right, file);
    }
}

bool SaveQuestions(SubjectTree root) {
    ofstream file("cauhoi.txt");
    if (!file.is_open()) return false;
    SaveQuestionsRecursive(root, file);
    file.close();
    return true;
}

// ==========================================
// CLASSES
// ==========================================
bool LoadClasses(ClassList& dsLop) {
    ifstream file("lop.txt");
    if (!file.is_open()) return false;
    string line;
    string tokens[10];
    while (getline(file, line)) {
        if (line.empty()) continue;
        int count = SplitString(line, '|', tokens, 10);
        if (count >= 2) {
            Class c;
            c.MALOP = tokens[0];
            c.TENLOP = tokens[1];
            AddClass(dsLop, c);
        }
    }
    file.close();
    return true;
}

bool SaveClasses(ClassList dsLop) {
    ofstream file("lop.txt");
    if (!file.is_open()) return false;
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr) {
            file << dsLop.nodes[i]->MALOP << "|" << dsLop.nodes[i]->TENLOP << "\n";
        }
    }
    file.close();
    return true;
}

// ==========================================
// STUDENTS
// ==========================================
bool LoadStudents(ClassList& dsLop) {
    ifstream file("sinhvien.txt");
    if (!file.is_open()) return false;
    string line;
    string tokens[10];
    while (getline(file, line)) {
        if (line.empty()) continue;
        int count = SplitString(line, '|', tokens, 10);
        // Format: MALOP | MASV | HO | TEN | PHAI | PASSWORD
        if (count >= 6) {
            string malop = tokens[0];
            Student s;
            s.MASV = tokens[1];
            s.HO = tokens[2];
            s.TEN = tokens[3];
            s.PHAI = tokens[4];
            s.password = tokens[5];
            
            Class* c = FindClass(dsLop, malop);
            if (c != nullptr) {
                AddStudent(c->students, s);
            }
        }
    }
    file.close();
    return true;
}

bool SaveStudents(ClassList dsLop) {
    ofstream file("sinhvien.txt");
    if (!file.is_open()) return false;
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr) {
            StudentNode* p = dsLop.nodes[i]->students;
            while (p != nullptr) {
                file << dsLop.nodes[i]->MALOP << "|"
                     << p->data.MASV << "|"
                     << p->data.HO << "|"
                     << p->data.TEN << "|"
                     << p->data.PHAI << "|"
                     << p->data.password << "\n";
                p = p->next;
            }
        }
    }
    file.close();
    return true;
}

// ==========================================
// SCORES & EXAM DETAILS
// ==========================================
bool LoadScores(ClassList& dsLop) {
    ifstream file("diem.txt");
    if (!file.is_open()) return false;
    string line;
    string tokens[10];
    string detailTokens[200];
    string pairTokens[5];
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        int count = SplitString(line, '|', tokens, 10);
        // Format: MASV | MAMH | Diem | QId:Ans,QId:Ans...
        if (count >= 3) {
            string masv = tokens[0];
            Score sc;
            sc.Mamh = tokens[1];
            sc.Diem = stof(tokens[2]);
            
            // Parse comma separated Answer details (if they exist)
            if (count >= 4 && !tokens[3].empty()) {
                int dCount = SplitString(tokens[3], ',', detailTokens, 200);
                for (int i = 0; i < dCount; i++) {
                    int pCount = SplitString(detailTokens[i], ':', pairTokens, 5);
                    if (pCount == 2) {
                        AnswerDetail ad;
                        ad.questionId = stoi(pairTokens[0]);
                        ad.studentSelection = pairTokens[1][0]; // e.g. 'A'
                        AddAnswerDetail(sc.details, ad);
                    }
                }
            }
            
            Class* belongingClass = nullptr;
            StudentNode* st = FindStudentGlobal(dsLop, masv, belongingClass);
            if (st != nullptr) {
                AddScore(st->data.scores, sc);
            }
        }
    }
    file.close();
    return true;
}

bool SaveScores(ClassList dsLop) {
    ofstream file("diem.txt");
    if (!file.is_open()) return false;
    for (int i = 0; i < dsLop.size; i++) {
        if (dsLop.nodes[i] != nullptr) {
            StudentNode* st = dsLop.nodes[i]->students;
            while (st != nullptr) {
                ScoreNode* sc = st->data.scores;
                while (sc != nullptr) {
                    file << st->data.MASV << "|" << sc->data.Mamh << "|" << sc->data.Diem;
                    
                    // Write answer details in format QId:Ans,QId:Ans
                    if (sc->data.details != nullptr) {
                        file << "|";
                        AnswerDetailNode* ad = sc->data.details;
                        while (ad != nullptr) {
                            file << ad->data.questionId << ":" << ad->data.studentSelection;
                            if (ad->next != nullptr) file << ",";
                            ad = ad->next;
                        }
                    } else {
                        file << "|"; 
                    }
                    file << "\n";
                    sc = sc->next;
                }
                st = st->next;
            }
        }
    }
    file.close();
    return true;
}
