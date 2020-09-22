#include <iostream>
#include <ctime> 
#include <vector>
using namespace std;

unsigned t0, t1;
const int MAX = 1052;
double C[MAX][MAX], A[MAX][MAX], B[MAX][MAX];
vector<double> v1;

void load(){
    /*Initialize A and x*/
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++){
            A[i][j] = rand() % 1000;
            B[i][j] = rand() % 1000;
        }
}

int main() {

    load();
    int temp = 2;
    while(temp<=MAX){
        //cout<< temp << " , ";
        t0=clock();
        for (int i = 0; i < temp; i++) {
            for (int j = 0; j < temp; j++) {
                for (int k = 0; k < temp; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
                //cout << C[i][j] << "  ";
            }
            //cout << "\n";
        }
        t1=clock();
        double t_1 = (double(t1-t0)/CLOCKS_PER_SEC);
        v1.push_back(t_1*1000);
        temp+=50;
    }
    /*
    cout << "\n";
    for(int i=0; i < v1.size(); i++)
        cout << v1[i] << " , ";
    */
}
