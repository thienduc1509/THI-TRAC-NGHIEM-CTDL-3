#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <windows.h>
#include <mylib.h>

using namespace std;
const int MAXLIST =2000;

const int so_item = 9;
const int dong =5;
const int cot = 10 ;
const int Up = 72; // Extended code 
const int Down = 80;

char thucdon [so_item][50] = {"1. Nhap danh sach hoc vien  ",
			                  "2. Liet ke danh sach        ",
			                  "3. Tim kiem hoc vien theo ma",
			                  "4. Xoa hoc vien theo ma so  ",
			                  "5. Xoa hoc vien theo ten    ",
			                  "6. Ghi danh sach            ",
			                  "7. Mo  danh sach            ",
			                  "8. Them sv co thu tu theo ma",
			                  "9. Ket thuc chuong trinh    "};

struct Sinhvien {
  int maso;
  char ho [50];
  char ten[10];
   
};

 struct list {
  int n=0;
  Sinhvien *nodes [MAXLIST];
};

void Normal () {
	SetColor(15); // white 
	SetBGColor(0);// black
}
void HighLight () {
	SetColor(15);
	SetBGColor(1); // blue
}
int MenuDong(char td [so_item][50]){

  Normal(); system("cls");
  int chon =0;
  int i; 
  for ( i=0; i< so_item ; i++)
  { gotoxy(cot, dong +i);
    cout << td[i];
  }
  HighLight();
  gotoxy(cot,dong+chon);
  cout << td[chon];
  char kytu;
  do {
  kytu = getch();
  if (kytu==0) kytu = getch();
  switch (kytu) {
    case Up :if (chon >0)
  			  {
  		        Normal(); 
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon --;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  				
  			  }
  			  break;
  	case Down :if (chon <so_item-1)
  			  {
  		        Normal();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon ++;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  				
  			  }
  			  break;
  	case Enter : return chon+1;
  }  // end switch
  } while (1);
}


int Search(list &ds, int x) {
  for (int i =0; i < ds.n ; i++)
     if (ds.nodes[i]->maso == x) return i;
  return -1;
}


void BaoLoi (char *s){
  int x=wherex() , y=wherey();
  gotoxy (10,24);
  cout <<s;
  Sleep(2000);
  gotoxy(10,24);
  clreol();
  gotoxy(x,y);
}
void NhapChuoi (char *tieude, char *S) {
	cout << tieude ; 
	int x=wherex(); gotoxy(x, wherey()+1);  fflush(stdin);
    do {
	  gotoxy(x, wherey()-1);
	  gets(S); 
    }
    while (strcmp(S,"")==0)  ;	
}

int  NhapSV ( list &ds, Sinhvien &sv){
while (true){
   cout << "Nhap ma so (<=0 la ket thuc) : " ;  
   cin >>sv.maso;                                
   if (sv.maso <=0) return 0;                    
   if (Search(ds, sv.maso)>=0){
       BaoLoi ("Ma so sinh vien bi trung");
       continue;
   }
    NhapChuoi (  "Nhap Ho: ", sv.ho);  // cout << "Ho : " ; gets(sv.ho);
    NhapChuoi (  "Nhap Ten:", sv.ten);
    return 1;
}
}
void NhapDSSV (list &ds, int flag){
 Sinhvien sv;
 
 if (flag==0) // nhap moi tu dau
 	while (ds.n> 0){
 	   delete ds.nodes[ds.n-1];
       ds.n-- ;
    } 
 
 while (ds.n <MAXLIST)
 {  
    if (NhapSV(ds, sv)==0)  return;
    ds.nodes[ds.n]= new Sinhvien;
    *ds.nodes [ds.n]=sv;
    ds.n++;
 }
 if (ds.n==MAXLIST) BaoLoi ("Danh sach day");
}

void LietKe (list ds) {
 system("cls") ;
 printf ("                     DANH SACH SINH VIEN \n");
 printf ("   Ma so       Ho   va ten \n");
 for (int i =0 ; i < ds.n ; i++)
  printf ("%8d %-30s%-10s\n", ds.nodes[i]->maso, 
     ds.nodes[i]->ho, ds.nodes[i]->ten);
 printf("So luong sinh vien :%d",ds.n);
 getch();
}

void ThongTinSv ( list &ds, int maso ) {
  int i = Search (ds,maso ) ;
  if (i==-1) BaoLoi("Ma so sinh vien khong co trong danh sach");
  else { printf( " Ho ten sv : %30s%10s", ds.nodes[i]->ho, ds.nodes[i]->ten); 
          getch(); }
}
void SaveFile(list &ds,char *filename) {
 FILE * f; 
 if ((f=fopen(filename,"wb"))==NULL)
 {  BaoLoi ("Loi mo file de ghi"); return ;
 }
  
 for (int i=0; i < ds.n; i++)
   fwrite (ds.nodes[i], sizeof (Sinhvien), 1, f);
 fclose(f);
 BaoLoi ("Da ghi xong danh sach vao file");

}
void OpenFile(list &ds, char *filename) {
 FILE * f; Sinhvien sv;
 if ((f=fopen(filename,"rb"))==NULL)
 {  BaoLoi ("Loi mo file de doc"); return ;
 }
  
 while  (fread (&sv, sizeof (Sinhvien), 1, f)!=0) {
 	ds.nodes[ds.n]=new Sinhvien;
 	*ds.nodes[ds.n]=sv;
 	ds.n ++;
 }
    
 fclose(f);
 BaoLoi ("Da load xong danh sach vao bo nho");

}
void DeleteItem (list &ds, int i){
	delete  ds.nodes[i]; // chi dung trong mang con tro
    for (int j=i+1; j <ds.n; j++)
       ds.nodes[j-1]=ds.nodes[j];
    ds.n--; 
}
int XoaSv(list &ds, int maso){
   int i = Search (ds,maso ) ;
   if (i==-1) return 0;
   DeleteItem (ds,  i); 
   return 1;
}

int XoaSvTheoTen(list &ds, char *ten){
int dem=0;
for(int i=0; i<ds.n;   )
if(stricmp(ds.nodes[i]->ten,ten)==0)
{
	DeleteItem (ds,  i);
	dem++;
} else i++;
return dem;
}
  

bool TestIncrease (list &ds){
	for (int i=0; i <ds.n-1; i++)
	  if (ds.nodes [i]->maso > ds.nodes [i+1]->maso) return false;
	return true;
}
int InsertOrder (list &ds, Sinhvien &sv){
	int i, k;
	if (ds.n ==MAXLIST ) return 0;
    for ( i=0;i < ds.n && ds.nodes[i]->maso < sv.maso ; i++);
 
	for ( k = ds.n-1; k >=i  ; k--) 
	   ds.nodes[k+1]= ds.nodes[k];
	ds.nodes[i]= new Sinhvien;
	*ds.nodes[i]=sv; ds.n++;
	return 1;
	
}
bool compareSV(Sinhvien* sv1, Sinhvien* sv2) {
	int compareTen = stricmp(sv1->ten, sv2->ten);
	
	if (compareTen < 0) {
        return true;
    } else if (compareTen > 0) {
        return false;
    } else {
        return stricmp(sv1->ho, sv2->ho) < 0;
    }
}

void sort(list &list) {
	int n = list.n;
    //Sinhvien* arr[n];
    //copy( list.nodes, list.nodes + n, arr);

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (!compareSV(list.nodes[j], list.nodes[j + 1])) {
                swap(list.nodes[j], list.nodes[j + 1]);
            }
        }
    }

    //copy(arr, arr + n, list.nodes);
}
int main (){ system ("cls");
  char filename[80]="D:\\DSSV.TXT";
  char ten[20]; 
  int chon;  
  list ds;   int maso; Sinhvien sv; 
  OpenFile(ds,filename); 
  do
  {
    chon = MenuDong (thucdon); Normal();
    system ("cls");
    switch (chon ){
    case 1: NhapDSSV(ds,1) ; break;
    case 2: LietKe(ds); break;
    case 3: { printf ("\n Ban nhap ma so sinh vien : ") ;
	          cin >> maso ;
	          ThongTinSv(ds,maso); break;
	        };
	case 4: { 
	          cout <<"Ban nhap ma sinh vien muon xoa : "; cin >> maso ; 
	          cout << "Da xoa duoc " << XoaSv(ds,maso) << " sinh vien.";
	          
			  getch() ;
			  break;
	    }; 
	case 5: { 
	          NhapChuoi ("Ban nhap ten sinh vien muon xoa : ", ten) ; 
	          cout << "Da xoa duoc " << XoaSvTheoTen(ds,ten) << " sinh vien.";
	          
			  getch() ;
			  break;
	    };    
    case 6: SaveFile(ds,filename); break;
  //  case 7: OpenFile(ds,filename); break;
    case 8: { 
	          if (TestIncrease (ds)==false )
                BaoLoi( "Danh sach sinh vien chua co thu tu tang theo ma nen khong the them");
              else  
	             if (NhapSV(ds, sv)==1) InsertOrder (ds,sv);
              break;
            } ;
    
     case so_item : { SaveFile(ds,filename); return 0;}
   }
  } while (true);
  return 0;
  }
