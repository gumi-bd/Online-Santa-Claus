#include <bits/stdc++.h>
using namespace std;

const double e = 2.71;

void swap(int& x, int& y){ 
    int temp = x; 
    x = y; 
    y = temp; 
} 
  
void permutations(vector<vector<int> >& res, vector<int> nums, int l, int h){ 
    if (l == h) { 
        res.push_back(nums); 
        return; 
    } 

    for (int i = l; i <= h; i++) { 
        swap(nums[l], nums[i]); 
        permutations(res, nums, l + 1, h); 
        swap(nums[l], nums[i]); 
    } 
} 
  
vector<vector<int> > permute(vector<int>& nums){ 
    vector<vector<int> > res; 
    int x = nums.size() - 1; 
    permutations(res, nums, 0, x); 
    return res; 
} 

double logsumexpEstimate(vector<double> &values, double eps){
    double n = values.size();
    double mn = 1e9;
    for(int i = 0; i < n; i++){
        mn = min(values[i], mn);
    }
    int max_diff = 10000*log(n)/eps;
    double diff = (rand()%max_diff)/10000.0;
    return mn - diff;
}

double logSumExp(vector<double> &values, double eps){
    double n = values.size();
    double val = 0;
    for(int i = 0; i < n; i++){
        val += pow(e, -eps*values[i]);
    }
    val = log(val);
    val /= -eps;
    return val;
}

vector<vector<double>> getallCombos(int n, double x){
    if(n == 1){
        return {{x}};
    }
    vector<vector<double>> allCombos;
    for(double i = 0; i < x; i += 0.1){
        vector<vector<double>> nextCombos = getallCombos(n - 1, x - i);
        for(int j = 0; j < nextCombos.size(); j++){
            nextCombos[j].push_back(i);
            allCombos.push_back(nextCombos[j]);
        }
    }
    return allCombos;
}

double online(vector<vector<double>> &order, double eps, vector<int> perm){
    int m = order.size(), n = order[0].size();
    vector<double> values(n, 0);
    vector<vector<double>> allCombos = getallCombos(n, 1);
    for(int it = 0; it < m/2; it++){
        int maxIndex = 0; 
        double mx = -100000;
        int i = perm[it];
        for(int j = 0; j < allCombos.size(); j++){
            for(int k = 0; k < n; k++){
                values[k] += allCombos[j][k]*order[i][k];
            }
            double val = logSumExp(values, eps);
            for(int k = 0; k < n; k++){
                values[k] -= allCombos[j][k]*order[i][k];
            }
            if(val > mx){
                mx = val;
                maxIndex = j;
            }
        }
        for(int k = 0; k < n; k++){
            values[k] += allCombos[maxIndex][k]*order[i][k];
        }
    }
    vector<double> values2(n, 0);
    for(int it = m/2; it < m; it++){
        int maxIndex = 0; 
        double mx = -100000;
        int i = perm[it];
        for(int j = 0; j < allCombos.size(); j++){
            for(int k = 0; k < n; k++){
                values2[k] += allCombos[j][k]*order[i][k];
            }
            double val = logSumExp(values2, eps);
            for(int k = 0; k < n; k++){
                values2[k] -= allCombos[j][k]*order[i][k];
            }
            if(val > mx){
                mx = val;
                maxIndex = j;
            }
        }
        for(int k = 0; k < n; k++){
            values2[k] += allCombos[maxIndex][k]*order[i][k];
        }
    }

    for(int i = 0; i < n; i++){
        values[i] += values2[i];
    }

    sort(values.begin(), values.end());
    return values[0];
}

int main(){
    int n, m;
    cin >> m >> n;
    vector<vector<double>> a(m, vector<double>(n));
    int opt;
    //cin >> opt;
    srand(time(0));
    vector<int> mpermutations(m); 
    for(int i = 0; i < m; i++){
        mpermutations[i] = i;
    }
    vector<vector<int>> allPermutations = permute(mpermutations);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            cin >> a[i][j];
        }
    }
    double sum = 0, mnVal = INT_MAX, val = 0;
    for(unsigned int i = 0; i < allPermutations.size(); i++){
        // change the value of eps (0.1) to get the value with a different epsilon
        val = online(a, 0.1, allPermutations[i]);
        sum += val;
        mnVal = min(mnVal, val);
    }
    cout << "Average: " << sum/1000 << "\nWorst Case: " << mnVal << endl; 
}