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


const int so_item = 7;
const int dong =5;
const int cot = 10 ;
const int MAXLIST =100;
char thucdon [so_item][50] = {"1. Them sv vao DS co thu tu theo ma so",
			                  "2. Liet ke danh sach                  ",
			                  "3. Xoa sinh vien theo ma so           ",
							  "4. Save DSSV                          ", 
							  "5. Load DSSV                          ",
							  "6. Xoa sinh vien theo ten             ",
							  "7.Ket thuc chuong trinh              "};
							  
							  
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

int ThemSVTheoViTri (PTR &First, int i, Sinhvien sv) {
	if (i <0 || Search (First, sv.maso)!= NULL) return 0;
	PTR p = new node ;
	p->sv = sv;
	
	if ( i==0 ) { p->next= First ; First = p; return 1;	}
	int n = Reccount (First) ; 
	if (i > n) i =n; 
	PTR q=First;
	for ( int j = 1; j <=i-1; j++ ) q=q->next ;
	p->next= q->next;
	q->next= p;
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
	int count =0; PTR p;
	while (First !=NULL && stricmp(First->sv.ten, tensv)==0) {
		p=First; First = First->next; 
		delete p; count ++; 
	}
	if (First ==NULL) return count;
	PTR q;
	for (p=First ; p->next != NULL ; )
	  if (stricmp(p->next->sv.ten, tensv)==0) {
	  	q=p->next; p->next= q->next; delete q; count ++;
	  }
	  else p=p->next;
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



void SelectionSortDescending (PTR &First)
{ PTR p, q, pmax;
	float max; Sinhvien sv;   
	if (First==NULL) return;
	for(p = First; p->next != NULL; p = p->next)
	{	max = p->sv.dtb;
		pmax = p;
		for(q = p->next; q != NULL; q = q->next)
			if(q->sv.dtb  > max )
			{
				max = q->sv.dtb;
				pmax = q;
			}
		sv=pmax->sv;
		pmax->sv = p->sv;
		p->sv = sv;
	}
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
    case 1:  break;
    case 2: LietKe(First) ; break;
    case 3: break;
	
    case 4: system ("cls");
	        if (SaveFile(First, filename)==0) cout << "Loi mo file de ghi ";
	        else cout << "Da ghi xong danh sach sinh vien vao file.";
	        Sleep(2000);
	        break;
    case 5:  system ("cls");
	        if (OpenFile(First, filename)==0) cout << "Loi mo file de doc ";
	        else cout << "Da doc xong danh sasch sinh vien tu file.";
	        Sleep(2000);
	        break; 

	case 6:  system ("cls");
	         cout << "Ten SV muon xoa :" ; fflush(stdin); gets (tensv);
	         cout << "So SV da xoa =" << Delete_All_Info(First,tensv);
			 getch(); 
	        break;		       
    case so_item : return 0;
   }
  } while (1);
  return 0;
  }			 
