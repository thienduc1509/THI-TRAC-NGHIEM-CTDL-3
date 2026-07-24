#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <mylib.h>
using namespace std;
 
// =====================================================================
// CẤU TRÚC DỮ LIỆU DANH SÁCH SINH VIÊN
// =====================================================================
struct Sinhvien {
  int maso;
  char ho[51];
  char ten[11];
  float dtb;
  int hang;
};

struct node {
  Sinhvien sv;
  node *next;
};
typedef struct node *PTR;

const int so_item = 13;
const int dong = 5;
const int cot = 10;
const int MAXLIST = 100;

char thucdon [so_item][50] = {"1. Nhap danh sach sinh vien (cuoi)  ",
			                  "2. Liet ke danh sach                ",
			                  "3. Dem so luong sinh vien           ",
			                  "4. Them sv o vi tri i               ",
			                  "5. Xoa sinh vien theo ma so         ",
			                  "6. Them 1 sv vao danh sach co thu tu",
			                  "7. Loc SV trung theo DTB            ",
			                  "8. Sap xep dssv theo ma so tang dan ",
							  "9. Save DSSV                        ", 
							  "10. Load DSSV                       ",
							  "11. Xep hang theo DTB               ",
							  "12. Xoa sinh vien theo ten          ",
							  "13. Ket thuc chuong trinh           "};
							  
/**
 * Hiển thị Menu chức năng dạng đồ họa console
 */
int Menu (char td [so_item][50]){
  system("cls");   
  int chon = 0;
  int i;
  for (i = 0; i < so_item; i++) {
    gotoxy(cot, dong + i);
    cout << td[i];
  }
Nhaplai:
  gotoxy (cot, dong + so_item);
  cout << "Ban chon 1 so (1..13) :    ";
  gotoxy (wherex() - 4, wherey());
  cin >> chon;
  if (chon < 1 || chon > so_item) goto Nhaplai;
  return chon;
}

/**
 * Hiển thị thông báo lỗi trên dòng 24 màn hình console
 */
void BaoLoi (const char* s){
  int x = wherex(), y = wherey();
  gotoxy (10, 24);
  cout << s;
  Sleep(4000);
  gotoxy(10, 24);
  clreol();
  gotoxy(x, y);
}

/**
 * Tìm kiếm sinh viên theo Mã Số
 */
PTR Search(PTR First, int x) {
  for (PTR p = First; p != NULL; p = p->next)
    if (p->sv.maso == x) return p;
  return NULL;
}

/**
 * Nhập thông tin 1 sinh viên (có kiểm tra trùng MSSV)
 */
int NhapSV(PTR First, Sinhvien &sv) {
NhaplaiMaSV:
  cout << "\nMa so sinh vien :";
  cin >> sv.maso;
  if (sv.maso <= 0) return 1;
  if (Search(First, sv.maso) != NULL) {
    BaoLoi("Ma so sinh vien bi trung. Ban nhap lai. ");
    goto NhaplaiMaSV;
  }
  fflush(stdin);
  cout << "Ho sinh vien  :"; gets(sv.ho);
  fflush(stdin);
  cout << "Ten sinh vien :"; gets(sv.ten);
  cout << "Diem TB :" ; cin >> sv.dtb;
  return 0;
}

/**
 * Thêm sinh viên liên tục vào ĐẦU danh sách
 */
void NhapSV_VeDau (PTR &First) {
  system("cls");
  PTR p; Sinhvien sv; int err;
  while (1) {
    err = NhapSV(First, sv);
    if (err == 1) return;
    p = new node;
    p->sv = sv; 
    p->next = First;
    First = p;
  }
}

/**
 * Thêm sinh viên liên tục vào CUỐI danh sách
 */
void Nhap_DSSV (PTR &First) {
  system("cls");
  PTR p, Last; Sinhvien sv; int err;
  if (First != NULL)
    for (Last = First; Last->next != NULL; Last = Last->next);
  while (1) {
    err = NhapSV(First, sv);
    if (err == 1) return;
    p = new node;
    p->sv = sv; p->next = NULL;
    if (First == NULL) First = p;
    else Last->next = p;
    Last = p;
  }
}

/**
 * Liệt kê toàn bộ danh sách sinh viên
 */
void LietKe (PTR First) {
  int dem = 0; system("cls");
  if (First == NULL) {
    cout << "Danh sach sinh vien dang trong!\n";
    getch();
    return;
  }
  printf("%5s %-30s %-10s %-5s\n", "MSSV", "Ho", "Ten", "DTB");
  cout << "---------------------------------------------------------\n";
  for (PTR p = First; p != NULL; p = p->next) {
    printf("%5d %-30s %-10s %.1f\n", p->sv.maso, p->sv.ho, p->sv.ten, p->sv.dtb);
    dem++;
  }
  cout << "---------------------------------------------------------\n";
  cout << "Tong so sinh vien: " << dem << "\n";
  getch();
}

/**
 * Đếm tổng số nút (sinh viên) trong danh sách
 */
int Reccount (PTR First) {
  int dem = 0;  
  for (PTR p = First; p != NULL; p = p->next)
    dem++;
  return dem;
}

/**
 * Hàm hỗ trợ: Chèn vào đầu danh sách
 */
void Insert_First(PTR &First, Sinhvien sv) {
  PTR p = new node;
  p->sv = sv;
  p->next = First;
  First = p;
}

/**
 * Hàm hỗ trợ: Chèn vào cuối danh sách
 */
void InsertLast (PTR &First, Sinhvien sv) {
  PTR p = new node;
  p->sv = sv; p->next = NULL;
  if (First == NULL) First = p;
  else { 
    PTR Last;
    for (Last = First; Last->next != NULL; Last = Last->next);		
    Last->next = p;
  }
}

/**
 * [Bài 1d]: Thêm 1 sinh viên vào vị trí i (1-based)
 */
bool ThemSVTheoViTri(PTR &First, int i, Sinhvien sv) {
  if (i <= 0) return false;
  if (i == 1) {
    Insert_First(First, sv);
    return true;
  }
  if (Reccount(First) < i) {
    InsertLast(First, sv);
    return true;
  }
  PTR X = new node;
  X->sv = sv;
  X->next = NULL;
  int dem = 1;
  PTR pre = NULL; 
  for (PTR temp = First; temp != NULL; temp = temp->next) {
    if (dem == i) {
      pre->next = X;
      X->next = temp;
      return true;
    }
    pre = temp;
    dem++;
  }
  return false;
}

/**
 * Lưu danh sách sinh viên ra File nhị phân
 */
int SaveFile (PTR First, const char *tenfile) {
  FILE *f = fopen (tenfile, "wb");
  if (f == NULL) return 0;
  for (PTR p = First; p != NULL; p = p->next)
    fwrite (&p->sv, sizeof (Sinhvien), 1, f);
  fclose (f);
  return 1;  
}

/**
 * Nạp danh sách sinh viên từ File nhị phân
 */
int OpenFile (PTR &First, const char *tenfile) {
  PTR p; Sinhvien sv;
  FILE *f = fopen (tenfile, "rb");
  if (f == NULL) return 0;
  while (First != NULL) {
    p = First; First = p->next; delete p;
  }
  while (fread (&sv, sizeof (Sinhvien), 1, f) == 1) 
    InsertLast (First, sv); 
  fclose (f);
  return 1;  
}

/**
 * Hỏi xác nhận thao tác (Y/N)
 */
int XacNhan (const char *S) {
  cout << S; char kt;
  do {
    kt = toupper(getch());	
  } while (kt != 'Y' && kt != 'N');
  cout << kt;
  return kt == 'Y';
}

/**
 * [Bài 1f]: Xóa sinh viên theo Mã Số
 */
void DeleteSV (PTR &First) {
  char masv[10]; PTR p;
Xoatiep:
  cout << "\nMa so sinh vien muon xoa :" ;
  fflush(stdin); gets(masv);
  if (strlen(masv) == 0) return;
  int maso = atoi(masv);
  if (First != NULL && First->sv.maso == maso) {
    int yn = XacNhan ("Ban co that su muon xoa hay khong (Y/N) ");
    if (yn == 1) { 
      PTR p = First; First = p->next; 
      delete p; 
    }  
    goto Xoatiep;
  }
  if (First == NULL) return;
  for (p = First; p->next != NULL && p->next->sv.maso != maso; p = p->next);
  if (p->next != NULL) {
    int yn = XacNhan ("Ban co that su muon xoa hay khong (Y/N) ");
    if (yn == 1) { PTR q = p->next; p->next = q->next; delete q; }
  } else 
    cout << "Ma so sinh vien muon xoa khong ton tai";
  goto Xoatiep;  
}

/**
 * [Bài 1g]: Sắp xếp danh sách theo Mã số tăng dần
 */
void SapXepTheoMaso(PTR &First) { 
  PTR p, q, pmin;
  Sinhvien sv;   
  if (First == NULL) return;
  for(p = First; p->next != NULL; p = p->next) {
    pmin = p;
    for(q = p->next; q != NULL; q = q->next)
      if(q->sv.maso < pmin->sv.maso) {
        pmin = q;
      }
    if (pmin != p) {
      sv = pmin->sv;
      pmin->sv = p->sv;
      p->sv = sv;
    }
  }
}

/**
 * [Bài 12]: Chèn sinh viên x vào danh sách đã sắp xếp mã số tăng dần
 */
void Insert_Order(PTR &First, Sinhvien x) {
  PTR p, t = NULL, s;
  p = new node;
  p->sv = x;
  for(s = First; s != NULL && s->sv.maso < x.maso; t = s, s = s->next);
  if(s == First) {
    p->next = First;
    First = p;
  } else {
    p->next = s;
    t->next = p;
  }
}

/**
 * [Bài 13 áp dụng cho Sinh viên]: Xóa tất cả SV có tên là x trong danh sách liên kết có thứ tự tăng dần theo tên.
 * Giải thuật tối ưu dừng sớm khi gặp tên lớn hơn x (stricmp > 0).
 */
void XoaTatCaX(PTR &First, const char x[]) {
    // 1. Xóa các node ở đầu danh sách nếu trùng tên x
    while (First != NULL && stricmp(First->sv.ten, x) == 0) {
        PTR p = First;
        First = First->next;
        delete p;
    }

    // 2. Xóa các node còn lại trong danh sách
    PTR p = First;
    while (p != NULL && p->next != NULL) {
        if (stricmp(p->next->sv.ten, x) == 0) {
            PTR q = p->next;
            p->next = q->next;
            delete q;
        } else {
            // Nếu tên của node kế tiếp lớn hơn x -> Dừng tìm kiếm sớm vì danh sách đã sắp xếp tăng dần theo tên
            if (stricmp(p->next->sv.ten, x) > 0) 
                break;
            else 
                p = p->next;
        }
    }
}

/**
 * Xóa tất cả sinh viên theo Tên (Hàm bổ trợ)
 */
int Delete_All_Info(PTR &First, const char *tensv) {
  int count = 0; PTR p, q;
  if (First == NULL) return 0;
  for (p = First; p->next != NULL; ) {
    if (stricmp(p->next->sv.ten, tensv) == 0) {
      q = p->next; p->next = q->next; delete q; count++;
    } else p = p->next;
  }
  if (First != NULL && stricmp(First->sv.ten, tensv) == 0) {
    q = First; First = q->next; 
    delete q; count++;
  }
  return count;
}

/**
 * [Bài 6]: Lọc các sinh viên bị trùng Điểm Trung Bình
 */
void Distinct_List(PTR &First) {
  if (First == NULL) return;
  PTR p = First;
  while (p != NULL) {
    PTR prev = p;
    PTR q = p->next;
    while (q != NULL) {
      if (q->sv.dtb == p->sv.dtb) {
        prev->next = q->next;
        delete q;
        q = prev->next;
      } else {
        prev = q;
        q = q->next;
      }
    }
    p = p->next;
  }
}

/**
 * Sắp xếp danh sách sinh viên theo ĐTB giảm dần
 */
void SapXepTheoDTB(PTR &First) {
  for(PTR p = First; p && p->next != NULL; p = p->next) {
    PTR pmax = p;
    for(PTR q = p->next; q != NULL; q = q->next) {
      if(q->sv.dtb > pmax->sv.dtb) {
        pmax = q;
      }
    }
    if (pmax != p) {
      Sinhvien temp = pmax->sv;
      pmax->sv = p->sv;
      p->sv = temp;
    }
  }
}

/**
 * [Bài 10]: Xếp hạng sinh viên dựa trên Điểm Trung Bình (cùng ĐTB trùng hạng)
 */
void XepHang(PTR &First) {
  if (First == NULL) {
    cout << "Danh sach trong.\n";
    getch();
    return;
  }
  SapXepTheoDTB(First);
  int current_rank = 1;
  First->sv.hang = current_rank;
  for(PTR curr = First; curr->next != NULL; curr = curr->next) {
    if (curr->next->sv.dtb < curr->sv.dtb) {
      current_rank++;
    }
    curr->next->sv.hang = current_rank;
  }
  
  cout << "\n--- DANH SACH SAU KHI XEP HANG THEO DTB ---\n";
  printf("%5s %-30s %-10s %-5s %-5s\n", "MSSV", "Ho", "Ten", "DTB", "Hang");
  for(PTR p = First; p != NULL; p = p->next) {
    printf("%5d %-30s %-10s %5.1f %5d\n", p->sv.maso, p->sv.ho, p->sv.ten, p->sv.dtb, p->sv.hang);
  }
  getch();
}

// =====================================================================
// BỔ SUNG CÁC BÀI TẬP VỀ DANH SÁCH LIÊN KẾT SỐ NGUYÊN & ĐA THỨC
// (Tích hợp đầy đủ các bài tập 2->9, 11->14 từ BaiTapDanhSachLienKet_TongHop.cpp)
// =====================================================================

struct NodeInt {
  int info;
  NodeInt* next;
};
typedef NodeInt* NODEINT;

// Bài 2: Thêm vào đầu DS Số nguyên
void Add_First_Int(NODEINT &First, int x) {
  NODEINT p = new NodeInt{x, First};
  First = p;
}

// Bài 3a: Xóa nút chỉ định p
void Delete_Node_Int(NODEINT &First, NODEINT p) {
  if (!First || !p) return;
  if (First == p) {
    First = First->next; delete p; return;
  }
  NODEINT curr = First;
  while (curr->next && curr->next != p) curr = curr->next;
  if (curr->next == p) {
    curr->next = p->next; delete p;
  }
}

// Bài 3b: Xóa tất cả các nút có thông tin bằng x
void Remove_All_X(NODEINT &First, int x) {
  while (First && First->info == x) {
    NODEINT t = First; First = First->next; delete t;
  }
  if (!First) return;
  NODEINT curr = First;
  while (curr->next) {
    if (curr->next->info == x) {
      NODEINT t = curr->next; curr->next = t->next; delete t;
    } else curr = curr->next;
  }
}

// Bài 4: Sao chép danh sách liên kết
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

// Bài 5: Ghép First2 vào First1 ngay sau vị trí i
void Merge_After_i(NODEINT &First1, NODEINT First2, int i) {
  if (i <= 0 || !First1) return;
  NODEINT curr = First1;
  for (int k = 1; k < i && curr->next; k++) curr = curr->next;
  NODEINT F2_tail = First2;
  if (F2_tail) {
    while (F2_tail->next) F2_tail = F2_tail->next;
    F2_tail->next = curr->next;
    curr->next = First2;
  }
}

// Bài 7: Đảo ngược danh sách liên kết
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

// Bài 8: Duyệt ngược danh sách liên kết bằng đệ quy
void Left_Traverse(NODEINT First) {
  if (!First) return;
  Left_Traverse(First->next);
  cout << First->info << " ";
}

// Bài 9: Tách danh sách thành DS số nguyên tố và DS các phần tử còn lại
int laSoNguyenTo(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; (long long)i * i <= (long long)n; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void TachDSLK(NODEINT &First, NODEINT &DSNguyenTo, NODEINT &DSConLai) {
    DSNguyenTo = DSConLai = NULL;
    NODEINT lastNT = NULL, lastCL = NULL;
    NODEINT p = First;

    while (p != NULL) {
        NODEINT pNext = p->next; 
        p->next = NULL;

        if (laSoNguyenTo(p->info)) {
            if (DSNguyenTo == NULL) DSNguyenTo = lastNT = p;
            else { lastNT->next = p; lastNT = p; }
        } else {
            if (DSConLai == NULL) DSConLai = lastCL = p;
            else { lastCL->next = p; lastCL = p; }
        }

        p = pNext; 
    }
    First = NULL;
}

// Bài 12: Thêm phần tử x vào DS liên kết có thứ tự tăng dần sao cho vẫn giữ thứ tự tăng
void Insert_Sorted(NODEINT &First, int x) {
    NODEINT p = new NodeInt{x, NULL};
    if (!First || First->info >= x) { // Thêm vào đầu nếu DS rỗng hoặc x nhỏ hơn/bằng phần tử đầu
        p->next = First; 
        First = p;
    } else {
        NODEINT curr = First;
        while (curr->next && curr->next->info < x) 
            curr = curr->next;
        p->next = curr->next;
        curr->next = p;
    }
}

// Bài 13: Loại bỏ tất cả phần tử x trong DS liên kết có thứ tự tăng dần (tối ưu dừng sớm)
void Remove_X_Sorted(NODEINT &First, int x) {
    // Xóa các phần tử x ở đầu danh sách
    while (First && First->info == x) {
        NODEINT t = First; 
        First = First->next; 
        delete t;
    }
    // Nếu danh sách rỗng hoặc phần tử đầu > x thì không còn x trong danh sách (dừng sớm)
    if (!First || First->info > x) return; 
    
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

// Bài 14: Trộn 1 DS tăng dần (First1) và 1 DS giảm dần (First2) thành 1 DS tăng dần
NODEINT Merge_Sorted_Asc_Desc(NODEINT First1, NODEINT First2) {
  Reverse_List(First2); // Đảo ngược First2 từ giảm dần thành tăng dần
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

// Bài 11: Nhân hai đa thức bằng Danh sách liên kết
struct DonThuc {
  int heso;
  int somu;
};
struct NodeDT {
  DonThuc info;
  NodeDT* next;
};
typedef NodeDT* NODEDT;

void Insert_Polynomial(NODEDT &First, int hs, int sm) {
  if (hs == 0) return;
  NODEDT p = new NodeDT{{hs, sm}, NULL};
  if (!First || First->info.somu < sm) {
    p->next = First; First = p;
  } else {
    NODEDT curr = First, prev = NULL;
    while (curr && curr->info.somu > sm) {
      prev = curr; curr = curr->next;
    }
    if (curr && curr->info.somu == sm) {
      curr->info.heso += hs;
      if (curr->info.heso == 0) {
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

NODEDT Multiply_Polynomial(NODEDT P1, NODEDT P2) {
  NODEDT Result = NULL;
  for (NODEDT i = P1; i != NULL; i = i->next) {
    for (NODEDT j = P2; j != NULL; j = j->next) {
      Insert_Polynomial(Result, i->info.heso * j->info.heso, i->info.somu + j->info.somu);
    }
  }
  return Result;
}

// =====================================================================
// HÀM MAIN VỚI MENU QUẢN LÝ TỔNG HỢP
// =====================================================================
int main () {
  char filename[80] = "D:\\DSSV.TXT";
  char tensv[16];
  int chon; PTR First; First = NULL;
  int i; Sinhvien sv;
  
  do {
    chon = Menu (thucdon);
    switch (chon) {
    case 1: 
      Nhap_DSSV(First); 
      break;
    case 2: 
      LietKe(First); 
      break;
    case 3: 
      system("cls");
      cout << "Tong so sinh vien hien co trong danh sach = " << Reccount(First) << endl;
      getch();
      break;
    case 4: 
      system ("cls");
      cout << "Vi tri can them :" ; cin >> i;
      cout << "\nMa so sinh vien :" ; cin >> sv.maso;
      fflush (stdin);
      cout << "Ho sinh vien  :" ; gets (sv.ho);
      fflush (stdin);
      cout << "Ten sinh vien :" ; gets (sv.ten);
      cout << "Diem TB :" ; cin >> sv.dtb;
      if (ThemSVTheoViTri (First, i, sv) == false) 
        cout << "Loi them sv theo vi tri";
      else 
        cout << "Da them xong!"; 
      Sleep(2000);
      break; 
    case 5: 
      DeleteSV (First); 
      break;
    case 6: {
      system("cls");
      cout << "Nhap thong tin sinh vien can them (vao danh sach co thu tu MSSV):\n";
      Sinhvien sv_new;
      if (NhapSV(First, sv_new) == 0) {
        Insert_Order(First, sv_new);
        cout << "Da them xong!";
      }
      Sleep(2000);
      break;
    }
    case 7: 
      Distinct_List(First);
      cout << "Da loc bo cac sinh vien trung Diem Trung Binh.";
      Sleep(2000);
      break;
    case 8: 
      SapXepTheoMaso(First); 
      cout << "Da sap xep danh sach theo MSSV tang dan.";
      Sleep(2000);
      break;
    case 9: 
      system ("cls");
      if (SaveFile(First, filename) == 0) cout << "Loi mo file de ghi!";
      else cout << "Da ghi xong danh sach sinh vien vao file nhin phan.";
      Sleep(2000);
      break;
    case 10: 
      system ("cls");
      if (OpenFile(First, filename) == 0) cout << "Loi mo file de doc!";
      else cout << "Da doc xong danh sach sinh vien tu file nhin phan.";
      Sleep(2000);
      break; 
    case 11: 
      system ("cls");
      XepHang(First);
      break;  
    case 12: 
      system ("cls");
      cout << "Ten SV muon xoa :" ; fflush(stdin); gets (tensv);
      cout << "So SV da xoa = " << Delete_All_Info(First, tensv);
      getch(); 
      break;		       
    case so_item: 
      return 0;
   }
  } while (1);

  return 0;
}
