#include <iostream>
#include <ctime> 
#include <vector>
using namespace std;

unsigned t0, t1, t2, t3;
const int MAX = 10000;
double A[MAX][MAX], x[MAX], y[MAX];
vector<double> v1;
vector<double> v2;

void load(){
    /*Initialize A and x*/
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            A[i][j] = rand() % 1000;
    
    for (int i = 0; i < MAX; i++)
        x[i] = rand() % 1000;
}

int main(){
    
    int temp = 20;
    load();
    while(temp<=MAX){
        //cout<< temp << " , ";
        /* Assign y = 0 */
        for (int i = 0; i < temp; i++)
            y[i] = 0;

        /* First pair of loops */
        t0=clock();
        for (int i = 0; i < temp; i++)
            for (int j = 0; j < temp; j++)
                y[i] += A[i][j]*x[j];

        t1 = clock();

        double t_1 = (double(t1-t0)/CLOCKS_PER_SEC);
        v1.push_back(t_1*1000);
        
        /* Assign y = 0 */
        for (int i = 0; i < temp; i++)
            y[i] = 0;
        /* Second pair of loops */

        t2 = clock();

        for (int j = 0; j < temp; j++)
            for (int i = 0; i < temp; i++)
                y[i] += A[i][j]*x[j];

        t3 = clock();

        double t_2 = (double(t3-t2)/CLOCKS_PER_SEC);
        v2.push_back(t_2*1000);

        //cout << "T: " << t_1 * 1000 <<" vs "<< t_2 * 1000 << endl;
        temp+=20;
    }
    /*
    for(int i=0; i < v1.size(); i++)
        cout << v1[i] << " , ";
    
    cout<<endl;

    for(int i=0; i < v2.size(); i++)
        cout << v2[i] << " , ";
    */
    return 0;
}
