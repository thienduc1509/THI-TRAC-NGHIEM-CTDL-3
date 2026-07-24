#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <mylib.h>
using namespace std;
 
// Cấu trúc lưu trữ thông tin sinh viên
struct Sinhvien {
  int maso;
  char ho [51];
  char ten[11];
  float dtb;
  int hang;
};

// Cấu trúc một nút (Node) trong Danh sách liên kết đơn
struct node {
  Sinhvien sv;
  node *next;
};
typedef struct node *PTR;


const int so_item = 13;
const int dong = 5;
const int cot = 10;
const int MAXLIST = 100;
char thucdon [so_item][50] = {"1. Nhap danh sach sinh vien         ",
			                  "2. Liet ke danh sach                ",
			                  "3. Them ve dau danh sach            ",
			                  "4. Them sv o vi tri i               ",
			                  "5. Xoa sinh vien theo ma so         ",
			                  "6. Them 1 sv vao danh sach co thu tu",
			                  "7. Loc SV trung theo ten            ",
			                  "8. Sap xep dssv theo ma so tang dan ",
							  "9. Save DSSV                        ", 
							  "10. Load DSSV                       ",
							  "11. Xep hang theo DTB               ",
							  "12. Xoa sinh vien theo ten          ",
							  "13.Ket thuc chuong trinh            "};
							  
							  
/**
 * Hiển thị menu chức năng và nhận lựa chọn từ người dùng.
 * @param td Mảng 2 chiều chứa danh sách các câu lệnh menu.
 * @return Giá trị số nguyên tương ứng với chức năng được chọn (1 -> 13).
 */
int Menu (char td [so_item][50]){
  system("cls");   int chon =0;
  int i;
  for ( i=0; i< so_item ; i++)
  { gotoxy(cot, dong +i);
    cout << td[i];
  }
Nhaplai:
  gotoxy (cot, dong + so_item);
  cout << "Ban chon 1 so (1..13) :    ";
  gotoxy (wherex()-4, wherey());
  cin >> chon;
  if (chon <1 || chon >so_item) goto Nhaplai;
  return chon;
}

/**
 * Hiển thị thông báo lỗi tại vị trí cố định trên màn hình console (dòng 24).
 * Tự động xóa thông báo lỗi sau 4 giây và khôi phục vị trí con trỏ.
 * @param s Thống báo lỗi cần hiển thị.
 */
void BaoLoi (const char* s){
  int x=wherex() , y=wherey();
  gotoxy (10,24);
  cout <<s;
  Sleep(4000);
  gotoxy(10,24);
  clreol();
  gotoxy(x,y);
}

/**
 * Tìm kiếm sinh viên theo mã số trong danh sách liên kết.
 * @param First Con trỏ quản lý nút đầu tiên của danh sách.
 * @param x Mã số sinh viên cần tìm.
 * @return Con trỏ PTR đến nút chứa sinh viên nếu tìm thấy, ngược lại trả về NULL.
 */
PTR Search(PTR First, int x)
{
	for (PTR p = First;  p  != NULL ; p=p->next )
              if ( p->sv.maso == x  ) return p;
	return NULL;
}

/**
 * Nhập thông tin chi tiết cho một sinh viên từ bàn phím.
 * Kiểm tra mã số hợp lệ và đảm bảo không bị trùng lặp mã số trong danh sách.
 * @param First Danh sách liên kết hiện tại (để tra cứu trùng mã số).
 * @param sv Biến tham chiếu chứa thông tin sinh viên nhập vào.
 * @return 0 nếu nhập thành công, 1 nếu dừng nhập (khi nhập mã số <= 0).
 */
int NhapSV(PTR First , Sinhvien &sv){
 NhaplaiMaSV:
		cout << "\nMa so sinh vien :" ;
		cin >> sv.maso ;
		if (sv.maso <=0 ) return 1;
		if (Search (First, sv.maso)!= NULL)  {
			BaoLoi("Ma so sinh vien bi trung. Ban nhap lai. ");
			goto NhaplaiMaSV;
		}
		fflush (stdin);
		cout << "Ho sinh vien  :" ; gets (sv. ho);
		fflush (stdin);
		cout << "Ten sinh vien :" ; gets (sv. ten);
		cout << "Diem TB :" ; cin >>sv.dtb;
		return 0;
}

/**
 * Nhập danh sách sinh viên liên tục và luôn chèn nút mới vào ĐẦU danh sách.
 * Dừng nhập khi người dùng nhập mã số sinh viên <= 0.
 * @param First Tham chiếu con trỏ đầu danh sách.
 */
void NhapSV_VeDau (PTR &First) {
	system ("cls");
	PTR p ; Sinhvien sv; int err;
	while (1) {
		err=NhapSV(First, sv) ;
		if (err==1) return;
		p = new node ;
		p->sv = sv ; 
		p->next=First;
		 First = p;
   }
}

/**
 * Nhập danh sách sinh viên liên tục và chèn nút mới vào CUỐI danh sách.
 * Dừng nhập khi người dùng nhập mã số sinh viên <= 0.
 * @param First Tham chiếu con trỏ đầu danh sách.
 */
void Nhap_DSSV (PTR &First) {
	system ("cls");
	PTR p, Last ; Sinhvien sv; int err;
    if(First !=NULL)
       for (Last = First ; Last->next !=NULL; Last=Last->next) ;
	while (1) {
		err=NhapSV(First, sv) ;
		if (err==1) return;
		p = new node ;
		p->sv = sv ; p->next=NULL;
		if (First==NULL) First = p;
		else Last -> next = p;
		Last = p;
   }
}

/**
 * In ra màn hình toàn bộ danh sách sinh viên (Mã số, Họ, Tên, ĐTB).
 * @param First Con trỏ trỏ tới đầu danh sách liên kết.
 */
void LietKe (PTR First) {
	int dem =0 ; system ("cls");
	for (PTR p = First ; p !=NULL; p=p->next)
	{	printf ("%5d %-30s %-10s %.1f\n",p->sv.maso , p->sv.ho, p->sv.ten, p->sv.dtb );
		dem ++;
	}
	getch();
}

/**
 * Đếm tổng số lượng sinh viên (số nút) hiện có trong danh sách.
 * @param First Con trỏ trỏ tới đầu danh sách liên kết.
 * @return Số lượng phần tử trong danh sách.
 */
int Reccount (PTR First) {
	int dem =0 ;  
	for (PTR p = First ; p !=NULL; p=p->next)
		dem ++;
	return dem;
}

/**
 * Lưu (xuất) toàn bộ dữ liệu danh sách sinh viên ra file nhị phân.
 * @param First Con trỏ trỏ tới đầu danh sách liên kết.
 * @param tenfile Đường dẫn/Tên file dùng để lưu dữ liệu.
 * @return 1 nếu ghi thành công, 0 nếu mở file thất bại.
 */
int SaveFile (PTR First, const char *tenfile) {
	FILE *f = fopen (tenfile, "wb");
	if (f==NULL) return 0;
	for (PTR p=First ; p!=NULL ; p=p->next)
	  fwrite (&p->sv, sizeof (Sinhvien), 1, f);
	fclose (f);
	return 1;  
}

/**
 * Hàm hỗ trợ: Chèn một sinh viên đã có thông tin vào CUỐI danh sách liên kết.
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param sv Cấu trúc sinh viên cần thêm.
 */
void InsertLast (PTR &First , Sinhvien sv) {
	    PTR p = new node ;
		p->sv = sv ; p->next=NULL;
		if (First==NULL) First = p;
		else { PTR Last;
		       for (Last = First;Last->next !=NULL; Last=Last->next) ;		
		       Last->next = p;
		     }
}

/**
 * Nạp (đọc) danh sách sinh viên từ file nhị phân vào danh sách liên kết.
 * (Xóa toàn bộ danh sách cũ trước khi nạp dữ liệu từ file).
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param tenfile Đường dẫn/Tên file cần đọc dữ liệu.
 * @return 1 nếu đọc file thành công, 0 nếu mở file thất bại.
 */
int OpenFile (PTR &First, const char *tenfile) {
	PTR p;  Sinhvien sv ;
	FILE *f = fopen (tenfile, "rb");
	if (f==NULL) return 0;
	while (First!= NULL) {
		p= First ; First = p->next ; delete p ;
	}
	while  (fread (&sv, sizeof (Sinhvien), 1, f)== 1 ) 
	   InsertLast (First , sv); 
	fclose (f);
	return 1;  
}

/**
 * Hiển thị câu hỏi xác nhận thao tác từ người dùng (Y/N).
 * @param S Chuỗi thông báo hỏi xác nhận (Ví dụ: "Ban co thuc su muon xoa (Y/N)?").
 * @return 1 nếu chọn 'Y' (Yes), 0 nếu chọn 'N' (No).
 */
int XacNhan (const char *S) {
	cout << S;  char kt;
	do {
	   kt = toupper(getch());	
	} while (kt!='Y' && kt !='N');
	cout <<kt  ;
	return kt=='Y';
}

/**
 * Xóa một sinh viên ra khỏi danh sách theo Mã số sinh viên nhập từ bàn phím.
 * Có hỏi xác nhận (Y/N) trước khi thực hiện xóa.
 * @param First Tham chiếu con trỏ đầu danh sách.
 */
void DeleteSV (PTR &First ){
	 char masv [10];  PTR p;
Xoatiep	 :
	 cout << "\nMa so sinh vien muon xoa :" ;
	 fflush(stdin); gets(masv) ;
	 if (strlen(masv)==0) return;
	 int maso = atoi(masv);
	 if (First != NULL && First ->sv.maso==maso) {
	 	int yn=XacNhan ("Ban co that su muon xoa hay khong (Y/N) ");
		if (yn==1) { PTR p = First ; First = p ->next ; 
		             delete p ; 
		}  
		goto Xoatiep;
	 }
	 if (First == NULL) return;
	 for (p=First ; p->next!=NULL && p->next->sv.maso !=maso ; p=p->next);
	 if (p->next!=NULL) {
	 	int yn=XacNhan ("Ban co that su muon xoa hay khong (Y/N) ");
		if (yn==1) { PTR q = p->next ; p->next= q->next; delete q; }
	 }
	 else 
	   cout << "Ma so sinh vien muon xoa khong ton tai";
	 goto Xoatiep;  
}

/**
 * Sắp xếp danh sách sinh viên theo Mã số tăng dần (sử dụng thuật toán Selection Sort).
 * @param First Tham chiếu con trỏ đầu danh sách.
 */
void SapXepTheoMaso(PTR &First)
{ PTR p, q, pmin;
	int min; Sinhvien sv;   
	if (First==NULL) return;
	for(p = First; p->next != NULL; p = p->next)
	{	min = p->sv.maso;
		pmin = p;
		for(q = p->next; q != NULL; q = q->next)
			if(q->sv.maso  < min )
			{
				min = q->sv.maso;
				pmin = q;
			}
		if (pmin != p) {
			sv=pmin->sv;
			pmin->sv = p->sv;
			p->sv = sv;
		}
	}
}

/**
 * Chèn một sinh viên mới vào danh sách đã được sắp xếp tăng dần theo mã số,
 * sao cho thứ tự sắp xếp của danh sách không bị thay đổi.
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param x Cấu trúc sinh viên cần chèn.
 */
void Insert_Order(PTR &First, Sinhvien x)
{
   PTR p, t, s;
   p=new node;
   p->sv=x;
   for(s = First; s != NULL && s->sv.maso < x.maso  ; t=s, s = s->next)   ;
   if(s == First)
   {
      p->next = First;
      First = p;
   }
   else
   {
      p->next= s;
      t->next= p;
   }
}

/**
 * Xóa tất cả các sinh viên có tên trùng với tên cần xóa (`tensv`).
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param tensv Tên sinh viên cần xóa (không phân biệt hoa/thường).
 * @return Số lượng sinh viên đã bị xóa.
 */
int Delete_All_Info(PTR &First, char *tensv){
	int count =0; PTR p,q;
	if (First ==NULL) return 0;
	for (p=First ; p->next != NULL ; )
	  if (stricmp(p->next->sv.ten, tensv)==0) {
	  	q=p->next; p->next= q->next;delete q; count ++;
	  }
	  else p=p->next;

	if (First !=NULL && stricmp(First->sv.ten, tensv)==0) {
		q=First; First = q->next; 
		delete q; count ++;
	}
	return count;
}

/**
 * Xóa nút nằm ngay sau nút p trong danh sách liên kết.
 * @param p Con trỏ trỏ tới nút đứng trước nút cần xóa.
 * @return 1 nếu xóa thành công, 0 nếu p bằng NULL hoặc nút sau p là NULL.
 */
int  Delete_after(PTR p)
{   PTR q;
  
   if((p == NULL) || (p->next == NULL))	
      return 0;
     q = p->next;
     p->next = q->next;
     delete q;
     return 1;
}

// =====================================================================
// CÁC HÀM BỔ SUNG ĐỂ HOÀN THIỆN THEO ĐỀ BÀI (BTVN)
// =====================================================================

/**
 * Hàm hỗ trợ: Chèn một phần tử mới chứa thông tin sinh viên vào ĐẦU danh sách.
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param sv Cấu trúc sinh viên cần thêm.
 */
void Insert_First(PTR &First, Sinhvien sv) {
    PTR p = new node;
    p->sv = sv;
    p->next = First;
    First = p;
}

/**
 * Thêm một sinh viên vào vị trí chỉ định `i` (chỉ số tính từ 1) trong danh sách.
 * Nếu i = 1: Thêm vào đầu danh sách.
 * Nếu i > số phần tử: Thêm vào cuối danh sách.
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param i Vị trí cần chèn.
 * @param sv Cấu trúc sinh viên cần thêm.
 * @return true nếu thêm thành công, false nếu vị trí i không hợp lệ (i <= 0).
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
 * Xóa một nút chỉ định `p` ra khỏi danh sách liên kết.
 * @param First Tham chiếu con trỏ đầu danh sách.
 * @param p Con trỏ đến nút cần xóa.
 * @return 1 nếu xóa thành công, 0 nếu nút cần xóa hoặc danh sách là NULL.
 */
int Delete_Node(PTR &First, PTR p)
{
    PTR q;
    if (First == NULL || p == NULL)
        return 0;

    if (First == p)
    {
        First = p->next;
        delete p;
        return 1;
    }

    for (q = First; q->next != p; q = q->next);

    q->next = p->next;
    delete p;
    return 1;
}

/**
 * Lọc bỏ các sinh viên có Điểm Trung Bình trùng nhau trong danh sách (chỉ giữ lại 1 sinh viên xuất hiện đầu tiên).
 * @param First Tham chiếu con trỏ đầu danh sách.
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
 * Sắp xếp danh sách sinh viên giảm dần theo Điểm Trung Bình (DTB) bằng thuật toán Selection Sort.
 * @param First Tham chiếu con trỏ đầu danh sách.
 */
void SapXepTheoDTB(PTR &First) {
    for(PTR p = First; p && p->next != NULL; p = p->next) {
        float max_dtb = p->sv.dtb;
        PTR pmax = p;
        for(PTR q = p->next; q != NULL; q = q->next) {
            if(q->sv.dtb > max_dtb) {
                max_dtb = q->sv.dtb;
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
 * Xếp hạng sinh viên dựa trên Điểm Trung Bình (ĐTB giảm dần).
 * Những sinh viên có điểm trung bình bằng nhau sẽ nhận cùng một thứ hạng.
 * In kết quả danh sách sinh viên kèm theo thứ hạng ra màn hình.
 * @param First Tham chiếu con trỏ đầu danh sách.
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

/**
 * Hàm main khởi chạy chương trình điều khiển menu chính.
 */
int main (){
  char filename[80]="D:\\DSSV.TXT";int x;
  char tensv[16];
  int chon;  PTR First; First=NULL; int so;
  
  int i ; Sinhvien sv;
  do
  {
    chon = Menu (thucdon);
    switch (chon ){
    case 1: Nhap_DSSV(First) ; break;
    case 2: LietKe(First) ; break;
    case 3: NhapSV_VeDau(First); break;
	case 4: system ("cls");
	        cout << "Vi tri can them :" ; cin >> i ;
	        cout << "\nMa so sinh vien :" ;
		    cin >> sv.maso ;
	   	    fflush (stdin);
	        cout << "Ho sinh vien  :" ; gets (sv. ho);
		    fflush (stdin);
		    cout << "Ten sinh vien :" ; gets (sv. ten);
            cout << "Diem TB :" ; cin >> sv.dtb;
		    if (ThemSVTheoViTri (First, i, sv)==0) 
		      cout << "Loi them sv theo vi tri";
		    else cout << "Da them xong"; 
			Sleep(3000);
			break; 
    case 5: 
            DeleteSV (First); break;
            
    case 6: {
            system("cls");
            cout << "Nhap thong tin sinh vien can them (vao danh sach da co thu tu):\n";
            Sinhvien sv_new;
            if (NhapSV(First, sv_new) == 0) {
                Insert_Order(First, sv_new);
                cout << "Da them vao danh sach.";
            }
            Sleep(2000);
            break;
    }
            
    case 7: Distinct_List(First) ;
            cout << "Da loc danh sach bang Distinct_List (loc trung DTB).";
	        Sleep(2000);
	        break;

    case 8: SapXepTheoMaso(First); 
            cout << "Da sap xep theo ma so tang dan.";
            Sleep(2000);
            break;
    case 9: system ("cls");
	        if (SaveFile(First, filename)==0) cout << "Loi mo file de ghi ";
	        else cout << "Da ghi xong danh sach sinh vien vao file.";
	        Sleep(2000);
	        break;
    case 10:  system ("cls");
	        if (OpenFile(First, filename)==0) cout << "Loi mo file de doc ";
	        else cout << "Da doc xong danh sach sinh vien tu file.";
	        Sleep(2000);
	        break; 
	case 11:  system ("cls");
	        XepHang(First);
	        break;  
	case 12:  system ("cls");
	         cout << "Ten SV muon xoa :" ; fflush(stdin); gets (tensv);
	         cout << "So SV da xoa = " << Delete_All_Info(First,tensv);
			 getch(); 
	        break;		       
    case so_item : return 0;
   }
  } while (1);
  return 0;
}
