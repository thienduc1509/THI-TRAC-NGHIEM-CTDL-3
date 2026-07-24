#include <iostream>
using namespace std;

//1. Tìm min của dãy số nguyên A có n số (int TimMin ( int *A, int n))
int timMin(int *A, int n){
    int min = A[0];
    for(int i = 1; i < n; i++){
        if(A[i] < min){
            min = A[i];
        }
    }
    return min;
}

//2. Tìm vị trí chứa giá trị min của dãy số nguyên A có n số
int timVtMin(int *A, int n){
    int vt = 0;
    for(int i = 1; i < n; i++){
        if(A[i] < A[vt]){
            vt = i;
        }
    }
    return vt;
}

//3. Liệt kê tần suất xuất hiện của các số trong dãy số nguyên A có n số, biết rằng Ai € [1..20]
void lietKeTanSuat(int *A, int n){
    int tanSuat[21] = {0};  
    for(int i = 0; i < n; i++){
        tanSuat[A[i]]++;
    }
    for(int i = 1; i <= 20; i++){
        if(tanSuat[i] > 0){
            cout << "Số " << i << " xuất hiện " << tanSuat[i] << " lần" << endl;
        }
    }
}

//4. Cho số tự nhiên n. Đảo các chữ số trong n
int daoChuSo(int n){
    int dao = 0;
    while(n > 0){
        dao = dao * 10 + n % 10;
        n /= 10;
    }
    return dao;
}

//5. Tìm 1 số trong dãy n số nguyên A có giá trị gần bằng với số nguyên x nhất (xét theo giá trị tuyệt đối)
int timSo(int *A, int n, int x){
    int minDiff = abs(A[0] - x);
    int closestNum = A[0];
    for(int i = 1; i < n; i++){
        int diff = abs(A[i] - x);
        if(diff < minDiff){
            minDiff = diff;
            closestNum = A[i];
        }
    }
    return closestNum;
}

//6. Viết CTC kiểm tra dãy A có n số nguyên có phải là dãy đối xứng hay không ?
bool kiemTraDoiXung(int *A, int n){
    for(int i = 0; i < n / 2; i++){
        if(A[i] != A[n - 1 - i]){
            return false;
        }
    }
    return true;
}

//7. Viết chương trình con đảo ngược các phần tử trong dãy A có n số.
void daoPhanTu(int *a, int n){
    for(int i = 0; i < n / 2; i++){
        int temp = a[i]; a[i] = a[n - 1 - i]; a[n - 1 - i] = temp;
    }
}

//8. Viết CTC tạo ngẫu nhiên m ( 1<=m <=100000) số với miền giá trị từ 1..100000 phân biệt khác nhau sao cho thời gian tạo dãy là nhanh nhất
void taoNgauNhien(int *a, int n,int m){ 
    int n = 100000;
    int a[n+1];

    for (int i = 0; i < n; i++) a[i] = i + 1;

    for (int i = 0; i < m; i++) {
        int j = i + rand() % (n - i);

        int temp = a[i]; a[i] = a[j]; a[j] = temp;
    }
}


//9. Viết CTC kiểm tra số tự nhiên n có phải là số thuận nghịch hay không ?
bool kiemTraThuannghich(int n){
    return n == daoChuSo(n);
}