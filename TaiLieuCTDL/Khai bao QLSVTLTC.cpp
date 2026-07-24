struct Monhoc {
    string MAMH, TENMH;
    int STC_LT, STC_TH;
};
struct nodeMH {
    Monhoc mh;
    nodeMH *left;
    nodeMH *right;
};
typedef nodeMH* TreeMH;

//DS DANG KY
struct Dangky {
    string MASV;
    float DIEM;
    bool HUYDK = false;
};
struct nodeDK {
    Dangky dk;
    nodeDK *next;
};
typedef nodeDK* PTRDK;

//DSLTC
const int MAXLTC = 10000;
struct LopTinChi {
    int MALTC;
    string MAMH, TENMH,NienKhoa;
    int HocKy, Nhom, SoSVMax, SoSVMin;
    bool HUYLOP = false;
    PTDK DanhSachDK = NULL;
};
struct DT_LTC {
    LopTinChi *node[MAXLTC];
    int n=0;
};
struct SinhVien {
    string MASV, HO, TEN, PHAI, SODT;
    string MALOP;
};
struct nodeSV {
    SinhVien sv;
    nodeSV *next;
};
typedef nodeSV* PTRSV;
int main() {
    TreeMH root = NULL;
    DS_LTC dsltc;
}