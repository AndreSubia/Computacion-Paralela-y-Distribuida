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
            A[i][j] = int(rand() % 1000);
            B[i][j] = int(rand() % 1000);
        }
}

int main() {

    double b;
    load();
    int temp = 1052;
    while(temp<=MAX){
        //cout<< temp << " , ";
        b = temp/2;
        t0=clock();
        for(int i1=0; i1<temp; i1+=b)
            for(int j1=0; j1<temp; j1+=b)
                for(int k1=0; k1<temp; k1+=b)
                    for(int i=0; i<b; i++)
                        for(int j=0; j<b; j++)
                            for(int k=0; k<b; k++)
                                C[i1+i][j1+j] += A[i1+i][k1+k]*B[k1+k][j1+j];

                                
        t1=clock();
        double t_1 = (double(t1-t0)/CLOCKS_PER_SEC);
        //v1.push_back(t_1*1000);
        temp+=50;
    }
    /*
    for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                cout << C[i][j] << "  ";
            }
            cout << "\n";
        }   
    
    cout << "\n";
    for(int i=0; i < v1.size(); i++)
        cout << v1[i] << " , ";
    */
}
