#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

// ============================================================================
// BÀI 1: QUẢN LÝ DANH SÁCH SINH VIÊN
// ============================================================================
struct Sinhvien {
    int mssv;
    char ho[31];
    char ten[11];
};

struct NodeSV {
    Sinhvien info;
    NodeSV* next;
};
typedef NodeSV* NODEPTR;

// a. Thêm cuối
void Add_Last(NODEPTR &First, Sinhvien sv) {
    NODEPTR p = new NodeSV{sv, NULL};
    if (First == NULL) {
        First = p;
    } else {
        NODEPTR curr = First;
        while (curr->next != NULL) curr = curr->next;
        curr->next = p;
    }
}

// b. Liệt kê
void Print_List(NODEPTR First) {
    if (!First) {
        cout << "Danh sach trong.\n"; return;
    }
    NODEPTR curr = First;
    while (curr) {
        cout << curr->info.mssv << " - " << curr->info.ho << " " << curr->info.ten << "\n";
        curr = curr->next;
    }
}

// c. Đếm số nút
int Reccount(NODEPTR First) {
    int count = 0;
    while (First) {
        count++;
        First = First->next;
    }
    return count;
}

// d. Thêm vị trí i (1-based)
void Insert_At(NODEPTR &First, Sinhvien sv, int i) {
    int count = Reccount(First);
    NODEPTR p = new NodeSV{sv, NULL};
    if (i == 0 || First == NULL) { // Thêm vào đầu
        p->next = First;
        First = p;
    } else if (i > count) { // Thêm vào cuối
        Add_Last(First, sv);
        delete p; // Bỏ node dư vì Add_Last tự tạo
    } else { // Thêm vào giữa
        NODEPTR curr = First;
        for (int k = 1; k < i - 1; k++) {
            curr = curr->next;
        }
        p->next = curr->next;
        curr->next = p;
    }
}

// e. In ra họ tên theo mssv
void Print_Name_By_MSSV(NODEPTR First, int mssv) {
    while (First) {
        if (First->info.mssv == mssv) {
            cout << "Ho ten: " << First->info.ho << " " << First->info.ten << "\n";
            return;
        }
        First = First->next;
    }
    cout << "Khong tim thay sinh vien co ma " << mssv << "\n";
}

// f. Loại bỏ nút theo mssv
void Delete_By_MSSV(NODEPTR &First, int mssv) {
    NODEPTR curr = First, prev = NULL;
    while (curr && curr->info.mssv != mssv) {
        prev = curr;
        curr = curr->next;
    }
    if (curr) {
        char confirm;
        cout << "Ban that su muon xoa (Y/N) ? ";
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            if (prev == NULL) First = curr->next;
            else prev->next = curr->next;
            delete curr;
            cout << "Da xoa thanh cong.\n";
        } else {
            cout << "Da huy viec xoa.\n";
        }
    } else {
        cout << "Khong tim thay ma sinh vien de xoa.\n";
    }
}

// g. Sắp xếp mã số tăng dần (Selection Sort trên Info)
void Sort_By_MSSV(NODEPTR First) {
    for (NODEPTR i = First; i != NULL; i = i->next) {
        for (NODEPTR j = i->next; j != NULL; j = j->next) {
            if (i->info.mssv > j->info.mssv) {
                Sinhvien temp = i->info;
                i->info = j->info;
                j->info = temp;
            }
        }
    }
}

// h. Ghi file
void Save_To_File(NODEPTR First, const char* filename) {
    ofstream out(filename, ios::binary);
    if (!out) { cout << "Loi mo file ghi.\n"; return; }
    while (First) {
        out.write((char*)&First->info, sizeof(Sinhvien));
        First = First->next;
    }
    out.close();
    cout << "Da ghi danh sach vao file " << filename << ".\n";
}

// i. Nạp file
void Load_From_File(NODEPTR &First, const char* filename) {
    // Xoá danh sách hiện tại
    while (First) {
        NODEPTR temp = First;
        First = First->next;
        delete temp;
    }
    ifstream in(filename, ios::binary);
    if (!in) { cout << "Loi mo file doc hoac file chua ton tai.\n"; return; }
    Sinhvien sv;
    while (in.read((char*)&sv, sizeof(Sinhvien))) {
        Add_Last(First, sv);
    }
    in.close();
    cout << "Da nap danh sach tu file " << filename << ".\n";
}

void Menu_Bai1() {
    NODEPTR First = NULL;
    int choice;
    do {
        cout << "\n--- MENU BAI 1 ---\n"
             << "1. Them ve cuoi DS\n"
             << "2. Liet ke DS\n"
             << "3. Dem so nut (Reccount)\n"
             << "4. Them 1 phan tu vao vi tri thu i\n"
             << "5. In ho ten theo mssv\n"
             << "6. Xoa theo mssv\n"
             << "7. Sap xep tang dan theo mssv\n"
             << "8. Ghi vao DSSV.DAT\n"
             << "9. Nap tu DSSV.DAT\n"
             << "0. Tro ve menu chinh\n"
             << "Chon: ";
        cin >> choice;
        if (choice == 1) {
            Sinhvien sv; cout<<"MSSV: "; cin>>sv.mssv; cin.ignore();
            cout<<"Ho: "; cin.getline(sv.ho, 31);
            cout<<"Ten: "; cin.getline(sv.ten, 11);
            Add_Last(First, sv);
        } else if (choice == 2) Print_List(First);
        else if (choice == 3) cout << "So nut: " << Reccount(First) << "\n";
        else if (choice == 4) {
            Sinhvien sv; int i;
            cout<<"MSSV: "; cin>>sv.mssv; cin.ignore();
            cout<<"Ho: "; cin.getline(sv.ho, 31);
            cout<<"Ten: "; cin.getline(sv.ten, 11);
            cout<<"Vi tri i: "; cin>>i;
            Insert_At(First, sv, i);
        } else if (choice == 5) {
            int mssv; cout<<"Nhap MSSV can tim: "; cin>>mssv;
            Print_Name_By_MSSV(First, mssv);
        } else if (choice == 6) {
            int mssv; cout<<"Nhap MSSV can xoa: "; cin>>mssv;
            Delete_By_MSSV(First, mssv);
        } else if (choice == 7) {
            Sort_By_MSSV(First); cout << "Da sap xep xong.\n";
        } else if (choice == 8) {
            Save_To_File(First, "DSSV.DAT");
        } else if (choice == 9) {
            Load_From_File(First, "DSSV.DAT");
        }
    } while (choice != 0);
    
    // Giải phóng
    while (First) { NODEPTR t = First; First = First->next; delete t; }
}

// ============================================================================
// BÀI 2 - 9, 12 - 14: DANH SÁCH LIÊN KẾT ĐƠN SỐ NGUYÊN
// ============================================================================
struct NodeInt {
    int info;
    NodeInt* next;
};
typedef NodeInt* NODEINT;

// Bài 2: Tạo danh sách thêm vào đầu
void Add_First_Int(NODEINT &First, int x) {
    NODEINT p = new NodeInt{x, First};
    First = p;
}

// Bài 3a: Xóa nút có địa chỉ p
void Delete_Node(NODEINT &First, NODEINT p) {
    if (!First || !p) return;
    if (First == p) {
        First = First->next;
        delete p;
        return;
    }
    NODEINT curr = First;
    while (curr->next && curr->next != p) curr = curr->next;
    if (curr->next == p) {
        curr->next = p->next;
        delete p;
    }
}

// Bài 3b: Loại bỏ tất cả các nút có nội dung x
void Remove_All_X(NODEINT &First, int x) {
    while (First && First->info == x) {
        NODEINT t = First; First = First->next; delete t;
    }
    if (!First) return;
    NODEINT curr = First;
    while (curr->next) {
        if (curr->next->info == x) {
            NODEINT t = curr->next;
            curr->next = t->next;
            delete t;
        } else {
            curr = curr->next;
        }
    }
}

// Bài 4: Copy_List
NODEINT Copy_List(NODEINT First) {
    if (!First) return NULL;
    NODEINT newFirst = new NodeInt{First->info, NULL};
    NODEINT currNew = newFirst;
    First = First->next;
    while (First) {
        currNew->next = new NodeInt{First->info, NULL};
        currNew = currNew->next;
        First = First->next;
    }
    return newFirst;
}

// Bài 5: Ghép First2 vào First1 ngay sau vị trí thứ i (1-based)
void Merge_After_i(NODEINT &First1, NODEINT First2, int i) {
    if (i <= 0 || !First1) return;
    NODEINT curr = First1;
    for (int k = 1; k < i && curr->next; k++) {
        curr = curr->next;
    }
    // Gắn phần còn lại của First1 vào cuối First2
    NODEINT F2_tail = First2;
    if (F2_tail) {
        while (F2_tail->next) F2_tail = F2_tail->next;
        F2_tail->next = curr->next; // Nối đuôi First2 với phần còn lại của First1
        curr->next = First2;        // Cắt First1 và nối với First2
    }
}

// Bài 6: Loại trùng (distinct)
void Distinct(NODEINT First) {
    while (First) {
        NODEINT runner = First;
        while (runner->next) {
            if (runner->next->info == First->info) {
                NODEINT t = runner->next;
                runner->next = t->next;
                delete t;
            } else {
                runner = runner->next;
            }
        }
        First = First->next;
    }
}

// Bài 7: Đảo ngược không dùng thêm vùng nhớ
void Reverse_List(NODEINT &First) {
    NODEINT prev = NULL, current = First, next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    First = prev;
}

// Bài 8: Duyệt ngược Left_Traverse
void Left_Traverse(NODEINT First) {
    if (!First) return;
    Left_Traverse(First->next);
    cout << First->info << " ";
}

// Bài 9: Tách thành DS số nguyên tố và DS phần tử còn lại
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); i++) 
        if (n % i == 0) return false;
    return true;
}

void Split_Prime(NODEINT &First, NODEINT &PrimeList, NODEINT &OtherList) {
    PrimeList = NULL; OtherList = NULL;
    NODEINT pTail = NULL, oTail = NULL;
    
    while (First) {
        NODEINT t = First;
        First = First->next;
        t->next = NULL; // Tách rời node t
        
        if (isPrime(t->info)) {
            if (!PrimeList) { PrimeList = t; pTail = t; }
            else { pTail->next = t; pTail = t; }
        } else {
            if (!OtherList) { OtherList = t; oTail = t; }
            else { oTail->next = t; oTail = t; }
        }
    }
}

// Bài 12: Thêm x vào DS có thứ tự tăng dần sao cho vẫn tăng
void Insert_Sorted(NODEINT &First, int x) {
    NODEINT p = new NodeInt{x, NULL};
    if (!First || First->info >= x) {
        p->next = First; First = p;
    } else {
        NODEINT curr = First;
        while (curr->next && curr->next->info < x) curr = curr->next;
        p->next = curr->next;
        curr->next = p;
    }
}

// Bài 13: Xóa x trong DS có thứ tự tăng dần
void Remove_X_Sorted(NODEINT &First, int x) {
    while (First && First->info == x) {
        NODEINT t = First; First = First->next; delete t;
    }
    if (!First || First->info > x) return; // Dừng sớm
    
    NODEINT curr = First;
    while (curr->next && curr->next->info <= x) {
        if (curr->next->info == x) {
            NODEINT t = curr->next;
            curr->next = t->next;
            delete t;
        } else {
            curr = curr->next;
        }
    }
}

// Bài 14: Trộn First1 (tăng) và First2 (giảm) -> thành DS tăng dần
NODEINT Merge_Sorted_Asc_Desc(NODEINT First1, NODEINT First2) {
    Reverse_List(First2); // Làm First2 thành tăng dần
    
    NODEINT dummy = new NodeInt{0, NULL};
    NODEINT tail = dummy;
    
    while (First1 && First2) {
        if (First1->info < First2->info) {
            tail->next = First1; First1 = First1->next;
        } else {
            tail->next = First2; First2 = First2->next;
        }
        tail = tail->next;
    }
    if (First1) tail->next = First1;
    if (First2) tail->next = First2;
    
    NODEINT res = dummy->next;
    delete dummy;
    return res;
}

// ============================================================================
// BÀI 10: DANH SÁCH LIÊN KẾT XẾP HẠNG HỌC VIÊN
// ============================================================================
struct HocVien {
    char ten[51];
    float diem;
    int hang;
};
struct NodeHV {
    HocVien info;
    NodeHV* next;
};
typedef NodeHV* NODEHV;

void Add_HV_Sorted_By_Diem(NODEHV &First, HocVien hv) {
    NODEHV p = new NodeHV{hv, NULL};
    // Sắp xếp giảm dần theo điểm (cao đứng trước) để dễ đánh hạng
    if (!First || First->info.diem < hv.diem) { 
        p->next = First; First = p;
    } else {
        NODEHV curr = First;
        while (curr->next && curr->next->info.diem >= hv.diem) curr = curr->next;
        p->next = curr->next;
        curr->next = p;
    }
}

void Bai10_HocVien() {
    NODEHV First = NULL;
    cout << "\n--- BAI 10: NHAP THONG TIN HOC VIEN ---\n";
    cout << "(De dung viec nhap, hay de trong 'Ten' va nhan Enter)\n";
    while (true) {
        HocVien hv;
        cout << "Ten: ";
        cin.getline(hv.ten, 51);
        if (strlen(hv.ten) == 0) break;
        cout << "Diem TB: ";
        cin >> hv.diem;
        cin.ignore();
        hv.hang = 0;
        Add_HV_Sorted_By_Diem(First, hv);
    }
    
    // Đánh hạng (Cùng điểm trung bình thì cùng hạng, hạng liên tục)
    if (First) {
        int current_rank = 1;
        First->info.hang = current_rank;
        NODEHV curr = First;
        while (curr->next) {
            if (curr->next->info.diem < curr->info.diem) {
                current_rank++; // Giảm điểm thì tăng hạng (số hạng tăng dần 1,2,3...)
            }
            curr->next->info.hang = current_rank;
            curr = curr->next;
        }
    }
    
    // In danh sách kết quả theo thứ tự hạng tăng dần (đã sort giảm điểm)
    cout << "\n--- DANH SACH KET QUA XEP HANG ---\n";
    NODEHV curr = First;
    while (curr) {
        cout << "Hang " << curr->info.hang << " \t| Diem: " << curr->info.diem << " \t| Ten: " << curr->info.ten << "\n";
        curr = curr->next;
    }
    
    // Xóa list
    while (First) { NODEHV t = First; First = First->next; delete t; }
}

// ============================================================================
// BÀI 11: NHÂN HAI ĐA THỨC BẰNG DANH SÁCH LIÊN KẾT
// ============================================================================
struct DonThuc {
    int heso;
    int somu;
};
struct NodeDT {
    DonThuc info;
    NodeDT* next;
};
typedef NodeDT* NODEDT;

// Chèn đơn thức vào đa thức (đảm bảo đa thức luôn giảm dần theo số mũ)
void Insert_Polynomial(NODEDT &First, int hs, int sm) {
    if (hs == 0) return; // Không lưu hệ số bằng 0
    NODEDT p = new NodeDT{{hs, sm}, NULL};
    if (!First || First->info.somu < sm) { // Chèn đầu
        p->next = First; First = p;
    } else {
        NODEDT curr = First, prev = NULL;
        while (curr && curr->info.somu > sm) {
            prev = curr; curr = curr->next;
        }
        if (curr && curr->info.somu == sm) { // Cùng số mũ, cộng hệ số
            curr->info.heso += hs;
            if (curr->info.heso == 0) { // Nếu tổng hệ số = 0, loại bỏ
                if (prev == NULL) First = curr->next;
                else prev->next = curr->next;
                delete curr;
            }
            delete p;
        } else {
            if (prev == NULL) { p->next = First; First = p; }
            else { p->next = curr; prev->next = p; }
        }
    }
}

// Nhân hai đa thức
NODEDT Multiply_Polynomial(NODEDT P1, NODEDT P2) {
    NODEDT Result = NULL;
    for (NODEDT i = P1; i != NULL; i = i->next) {
        for (NODEDT j = P2; j != NULL; j = j->next) {
            Insert_Polynomial(Result, i->info.heso * j->info.heso, i->info.somu + j->info.somu);
        }
    }
    return Result;
}

// In đa thức
void Print_Polynomial(NODEDT First) {
    if (!First) {
        cout << "0\n";
        return;
    }
    bool first_term = true;
    while (First) {
        if (First->info.heso > 0 && !first_term) cout << " + ";
        else if (First->info.heso < 0 && !first_term) { cout << " - "; }
        else if (First->info.heso < 0 && first_term) { cout << "-"; }
        
        if (abs(First->info.heso) != 1 || First->info.somu == 0) {
            cout << abs(First->info.heso);
        }
        
        if (First->info.somu > 0) {
            cout << "x";
            if (First->info.somu > 1) cout << "^" << First->info.somu;
        }
        
        first_term = false;
        First = First->next;
    }
    cout << "\n";
}

// ============================================================================
// HÀM MAIN: MENU TỔNG HỢP GỌI CÁC BÀI
// ============================================================================
int main() {
    int main_choice;
    do {
        cout << "\n========== CHUONG TRINH TONG HOP BAITAP CSDL ==========\n";
        cout << "1. Chay Bai 1 (Quan ly Sinh Vien co menu rieng)\n";
        cout << "2. Chay thu nghiem Bai 10 (Nhap hoc vien, xep hang)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "(Ghi chu: Cac bai tu 2 den 14 (tru Bai 10) deu da duoc cai dat \n";
        cout << " duoi dang cac ham chuan xac trong ma nguon cua file nay!)\n";
        cout << "=======================================================\n";
        cout << "Chon chuc nang: ";
        cin >> main_choice;
        if (main_choice == 1) {
            Menu_Bai1();
        } else if (main_choice == 2) {
            cin.ignore();
            Bai10_HocVien();
        }
    } while (main_choice != 0);
    return 0;
}
