struct Monhoc {
	string MAMH, TENMH,
	int STC_LT, STC_TH;
};
struct nodeMH{
	Monhoc mh;
	nodeMH *left;
	nodeMH *right;
};
typedef nodeMH* treeMH;

//  DANG KY
struct Dangky{
	string MASV ;
	float  DIEM ;
	bool HUYDK = false;
};
struct nodeDK {
	Dangky dk;
	nodeDK* next;
};
typedef nodeDK* PTRDK;
// DSLTC
const int MAXLTC=10000;
struct Loptinchi {
	int MALOPTC ; 
	string MAMH,  NIENKHOA;
	int HOCKY, NHOM, SOSVMIN, SOSVMAX; 
	bool HUYLOP=false;
	PTRDK dssvdk= NULL;
};
struct DS_LTC {
	Loptinchi *nodes[MAXLTC];
	int n=0;
};
struct Sinhvien {
	string MASV, HO, TEN,  PHAI, SODT;
	string MALOP ;
};
struct nodeSV {
  Sinhvien sv;
  nodeSV * next; 
};
typedef nodeSV* PTRSV;

int main {
  treeMH dsmh=NULL;	
  DS_LTC dsltc;
  PTRSV dssv=NULL;
}
