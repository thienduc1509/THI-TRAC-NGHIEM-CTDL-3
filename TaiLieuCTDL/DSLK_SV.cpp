#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <mylib.h>
using namespace std;
 
struct Sinhvien {
  int maso;
  char ho [51];
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
const int dong =5;
const int cot = 10 ;
const int MAXLIST =100;
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
							  
							  
int Menu (char td [so_item][50]){
  system("cls");   int chon =0;
  int i;
  for ( i=0; i< so_item ; i++)
  { gotoxy(cot, dong +i);
    cout << td[i];
  }
Nhaplai:
  gotoxy (cot, dong + so_item);
  cout << "Ban chon 1 so (1..11) :    ";
  gotoxy (wherex()-4, wherey());
  cin >> chon;
  if (chon <1 || chon >so_item) goto Nhaplai;
  return chon;

}

void BaoLoi (char s[100]){
  int x=wherex() , y=wherey();
  gotoxy (10,24);
  cout <<s;
  Sleep(4000);
  gotoxy(10,24);
  clreol();
  gotoxy(x,y);
}
PTR Search(PTR First, int x)
{
	for (PTR p = First;  p  != NULL ; p=p->next )
              if ( p->sv.maso == x  ) return p;
	return NULL;
}
int NhapSV(PTR First , Sinhvien &sv){
 NhaplaiMaSV:
		cout << "\nMa so sinh vien :" ;
		cin >> sv.maso ;
		if (sv.maso <=0 ) return 1;
		if (Search (First, sv.maso)!= NULL)  {
			BaoLoi( "Ma so sinh vien bi trung. Ban nhap lai. ");
			goto NhaplaiMaSV;
		}
		fflush (stdin);
		cout << "Ho sinh vien  :" ; gets (sv. ho);
		fflush (stdin);
		cout << "Ten sinh vien :" ; gets (sv. ten);
		cout << "Diem TB :" ; cin >>sv.dtb;
		return 0;
}
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
void Nhap_DSSV (PTR &First) { // them ve cuoi
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

void LietKe (PTR First) {
	int dem =0 ; system ("cls");
	for (PTR p = First ; p !=NULL; p=p->next)
	{	printf ("%5d %-30s %-10s %.1f\n",p->sv.maso , p->sv.ho, p->sv.ten, p->sv.dtb );
		dem ++;
	/*	if (dem %5 ==0 && p->next !=NULL) {
			printf ("\n An phim bat ky de tiep tuc");
			getch();
			system ("cls");
		}
	*/
	}
	getch();
}

int Reccount (PTR First) {
	int dem =0 ;  
	for (PTR p = First ; p !=NULL; p=p->next)
		dem ++;
	return dem;
}

int SaveFile (PTR First, char *tenfile) {
	FILE *f = fopen (tenfile, "wb");
	if (f==NULL) return 0;
	for (PTR p=First ; p!=NULL ; p=p->next)
	  fwrite (&p->sv, sizeof (Sinhvien), 1, f);
	fclose (f);
	return 1;  
}
void InsertLast (PTR &First , Sinhvien sv) {
	    PTR p = new node ;
		p->sv = sv ; p->next=NULL;
		if (First==NULL) First = p;
		else { PTR Last;
		       for (Last = First;Last->next !=NULL; Last=Last->next) ;		
		       Last->next = p;
		     }
}

int OpenFile (PTR &First, char *tenfile) {
	PTR p;  Sinhvien sv ;
	FILE *f = fopen (tenfile, "rb");
	if (f==NULL) return 0; // mo file khong duoc
	while (First!= NULL) {
		p= First ; First = p->next ; delete p ;
	}
	while  (fread (&sv, sizeof (Sinhvien), 1, f)== 1 ) 
	   InsertLast (First , sv); 
	fclose (f);
	return 1;  
}


int XacNhan (char *S) {
	cout << S;  char kt;
	do {
	   kt = toupper(getch());	
	} while (kt!='Y' && kt !='N');
	cout <<kt  ;
	return kt=='Y';
}
void DeleteSV (PTR &First ){
	 char masv [10];  PTR p;
Xoatiep	 :
	 cout << "\nMa so sinh vien muon xoa :" ;
	 fflush(stdin); gets(masv) ;
	 if (strlen(masv)==0) return;
	 int maso = atoi(masv);
	 if (First ->sv.maso==maso) {
	 	int yn=XacNhan ("Ban co that su muon xoa hay khong (Y/N) ");
		if (yn==1) { PTR p = First ; First = p ->next ; 
		             delete p ; 
		}  
		goto Xoatiep;
	 }
	 for (p=First ; p->next!=NULL && p->next->sv.maso !=maso ; p=p->next);
	 if (p->next!=NULL) {
	 	int yn=XacNhan ("Ban co that su muon xoa hay khong ");
		if (yn==1) { PTR q = p->next ; p->next= q->next; delete q; }
	 }
	 else 
	   cout << "Ma so sinh vien muon xoa khong ton tai";
	 goto Xoatiep;  
}
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
		// hoan doi truong info cua hai nut p va pmin
		sv=pmin->sv;
		pmin->sv = p->sv;
		p->sv = sv;
	}
}

void Insert_Order(PTR &First, Sinhvien x)
{
   PTR p, t, s;  // t la nut truoc, s la nut sau
   p=new node;
   p->sv=x;
   for(s = First; s != NULL && s->sv.maso < x.maso  ; t=s, s = s->next)   ;
   if(s == First)  // them nut vao dau danh sach lien ket
   {
      p->next = First;
      First = p;
   }
   else           // them nut p vao truoc nut s
   {
      p->next= s;
      t->next= p;
   }
}


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
int  Delete_after(PTR p)
{   PTR q;
  
   if((p == NULL) || (p->next == NULL))	
      return 0;
     q = p->next;  // q chi nut can xoa
     p->next = q->next;
     delete q;
     return 1;
}





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
    case 3: break;
	case 4: system ("cls");
	        cout << "Vi tri can them :" ; cin >> i ;
	        cout << "\nMa so sinh vien :" ;
		    cin >> sv.maso ;
	   	    fflush (stdin);
	        cout << "Ho sinh vien  :" ; gets (sv. ho);
		    fflush (stdin);
		    cout << "Ten sinh vien :" ; gets (sv. ten);
		    if (ThemSVTheoViTri (First, i, sv)==0) 
		      cout << "Loi them sv theo vi tri";
		    else cout << "Da them xong"; 
			Sleep(3000);
			break; 
    case 5: 
            DeleteSV (First); break;
           
    case 7: Locpttrungtheotensv(First) ;
            cout << "Da Loc xong .";
	        Sleep(2000);
	        break;

    case 8: SapXepTheoMaso(First); break;
    case 9: system ("cls");
	        if (SaveFile(First, filename)==0) cout << "Loi mo file de ghi ";
	        else cout << "Da ghi xong danh sasch sinh vien vao file.";
	        Sleep(2000);
	        break;
    case 10:  system ("cls");
	        if (OpenFile(First, filename)==0) cout << "Loi mo file de doc ";
	        else cout << "Da doc xong danh sasch sinh vien tu file.";
	        Sleep(2000);
	        break; 
	case 11:  system ("cls");
	       // XepHang(First);
	        break;  
	case 12:  system ("cls");
	         cout << "Ten SV muon xoa :" ; fflush(stdin); gets (tensv);
	         cout << "So SV da xoa =" << Delete_All_Info(First,tensv);
			 getch(); 
	        break;		       
    case so_item : return 0;
   }
  } while (1);
  return 0;
  }			 
