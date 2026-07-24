#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

struct Sinhvien {
    int maso;
    char ho[51];
    char ten[11];
};

struct node {
    Sinhvien sv;
    node *lk1, *lk2;  
};
typedef node *PTR;

void nhapSinhVien(Sinhvien &sv) {
    cout << "Nhap ma so: ";
    cin >> sv.maso;
    cin.ignore();
    cout << "Nhap ho: ";
    cin.getline(sv.ho, 51);
    cout << "Nhap ten: ";
    cin.getline(sv.ten, 11);
}

void inSinhVien(Sinhvien sv) {
    cout << left << setw(10) << sv.maso 
         << setw(30) << sv.ho 
         << setw(15) << sv.ten << endl;
}

// Hàm tìm kiếm sinh viên theo mã số (tận dụng danh sách lk1 đã sắp xếp)
PTR timKiemNhanhTheoMaSo(PTR first_maso, int maso) {
    PTR p;
    for (p = first_maso; p != NULL && p->sv.maso < maso; p = p->lk1);
    return (p != NULL && p->sv.maso == maso) ? p : NULL;
}

// Hàm nhập thông tin 1 sinh viên
void themSinhVien(PTR &first_maso, PTR &first_ten, Sinhvien sv) {
    if (timKiemNhanhTheoMaSo(first_maso, sv.maso) != NULL) {
        cout << "Loi: Ma so sinh vien (" << sv.maso << ") da ton tai!" << endl;
        return;
    }

    PTR p = new node;
    p->sv = sv;
    p->lk1 = NULL;
    p->lk2 = NULL;

    PTR curr1 = first_maso, prev1 = NULL;
    while (curr1 != NULL && curr1->sv.maso < sv.maso) {
        prev1 = curr1;
        curr1 = curr1->lk1;
    }
    p->lk1 = curr1;
    if (prev1 == NULL) {
        first_maso = p;
    } else {
        prev1->lk1 = p;
    }

    PTR curr2 = first_ten, prev2 = NULL;
    while (curr2 != NULL) {
        int cmp_ten = strcmp(curr2->sv.ten, sv.ten);
        if (cmp_ten < 0) { 
            prev2 = curr2;
            curr2 = curr2->lk2;
        } else if (cmp_ten == 0) { 
            int cmp_ho = strcmp(curr2->sv.ho, sv.ho);
            if (cmp_ho < 0) {
                prev2 = curr2;
                curr2 = curr2->lk2;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    p->lk2 = curr2;
    if (prev2 == NULL) {
        first_ten = p;
    } else {
        prev2->lk2 = p;
    }
    cout << "Them sinh vien thanh cong!" << endl;
}

// 2. Hàm xóa sinh viên theo mã số
void xoaSinhVien(PTR &first_maso, PTR &first_ten, int maso) {
    PTR curr1 = first_maso, prev1 = NULL;
    while (curr1 != NULL && curr1->sv.maso < maso) {
        prev1 = curr1;
        curr1 = curr1->lk1;
    }

    if (curr1 == NULL || curr1->sv.maso != maso) {
        cout << "Khong tim thay sinh vien co ma so " << maso << endl;
        return;
    }

    PTR del_node = curr1;

    if (prev1 == NULL) {
        first_maso = del_node->lk1;
    } else {
        prev1->lk1 = del_node->lk1;
    }

    PTR curr2 = first_ten, prev2 = NULL;
    while (curr2 != NULL && curr2 != del_node) {
        prev2 = curr2;
        curr2 = curr2->lk2;
    }
    
    if (curr2 == del_node) {
        if (prev2 == NULL) {
            first_ten = del_node->lk2;
        } else {
            prev2->lk2 = del_node->lk2;
        }
    }

    delete del_node;
    cout << "Da xoa sinh vien co ma so " << maso << endl;
}

// 3. Hàm tìm sinh viên theo mã số
void timSinhVien(PTR first_maso, int maso) {
    PTR curr = timKiemNhanhTheoMaSo(first_maso, maso);
    
    if (curr != NULL) {
        cout << "\nThong tin sinh vien tim thay:\n";
        cout << left << setw(10) << "Ma so" << setw(30) << "Ho" << setw(15) << "Ten" << endl;
        inSinhVien(curr->sv);
    } else {
        cout << "Khong tim thay sinh vien co ma so " << maso << endl;
    }
}

// 4. Hàm liệt kê DSSV tăng dần theo mã số (dùng lk1)
void lietKeTheoMaSo(PTR first_maso) {
    if (first_maso == NULL) {
        cout << "Danh sach trong!" << endl;
        return;
    }
    cout << "\n--- DANH SACH SINH VIEN THEO MA SO TANG DAN ---\n";
    cout << left << setw(10) << "Ma so" << setw(30) << "Ho" << setw(15) << "Ten" << endl;
    PTR curr = first_maso;
    while (curr != NULL) {
        inSinhVien(curr->sv);
        curr = curr->lk1;
    }
}

// 5. Hàm liệt kê DSSV tăng dần theo tên (dùng lk2)
void lietKeTheoTen(PTR first_ten) {
    if (first_ten == NULL) {
        cout << "Danh sach trong!" << endl;
        return;
    }
    cout << "\n--- DANH SACH SINH VIEN THEO TEN TANG DAN ---\n";
    cout << left << setw(10) << "Ma so" << setw(30) << "Ho" << setw(15) << "Ten" << endl;
    PTR curr = first_ten;
    while (curr != NULL) {
        inSinhVien(curr->sv);
        curr = curr->lk2;
    }
}

// 6. Hàm giải phóng toàn bộ danh sách khi kết thúc chương trình
void giaiPhongDanhSach(PTR &first_maso) {
    while (first_maso != NULL) {
        PTR temp = first_maso;
        first_maso = first_maso->lk1;
        delete temp;
    }
}

int main() {
    PTR first_maso = NULL;  
    PTR first_ten = NULL;   
    int choice;

    do {
        cout << "\n===== QUAN LY SINH VIEN (DA LIEN KET) =====\n";
        cout << "1. Nhap danh sach sinh vien\n";
        cout << "2. Xoa sinh vien theo ma so\n";
        cout << "3. Tim sinh vien theo ma so\n";
        cout << "4. Liet ke DSSV tang dan theo ma so\n";
        cout << "5. Liet ke DSSV tang dan theo ten\n";
        cout << "6. Ket thuc\n";
        cout << "===========================================\n";
        cout << "Nhap lua chon (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Sinhvien sv;
                cout << "\n[Nhap thong tin sinh vien]\n";
                nhapSinhVien(sv);
                themSinhVien(first_maso, first_ten, sv);
                break;
            }
            case 2: {
                int maso;
                cout << "\nNhap ma so sinh vien can xoa: ";
                cin >> maso;
                xoaSinhVien(first_maso, first_ten, maso);
                break;
            }
            case 3: {
                int maso;
                cout << "\nNhap ma so sinh vien can tim: ";
                cin >> maso;
                timSinhVien(first_maso, maso);
                break;
            }
            case 4:
                lietKeTheoMaSo(first_maso);
                break;
            case 5:
                lietKeTheoTen(first_ten);
                break;
            case 6:
                cout << "Ket thuc chuong trinh. Tam biet!\n";
                giaiPhongDanhSach(first_maso);
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap tu 1 den 6!\n";
        }
    } while (choice != 6);

    return 0;
}
