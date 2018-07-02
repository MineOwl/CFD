#include <iostream> //for cout,cin
#include <vector> //for vector
#include <fstream> //for open
#include <string> //for string
#include <sstream> //for split


using namespace std;


// complement:::補間する
class LinerComplement{
public:

    template <typename MayBefloat>
    float XIntoY(MayBefloat x);

    LinerComplement(string filename);
private:

    vector <float> xarray;
    vector <float> yarray;
    float step;
    vector <string> split(const string& input , char delimiter);
    //毎回毎回forループを回すと壊れるかも？
    //続いている値を取り出すので前回どこまで回したかを記録しておく
    int forloopmark;
};
LinerComplement::LinerComplement(string filename){
    /*
    第一引数：ファイルの名前
    第二引数：結果の歩幅
    */

    //変数宣言
    string filestring;
    ifstream inputfile;
    vector<string> splitted;

    //ファイルダウンロード
    inputfile.open(filename);

    //ファイルの読み込み失敗時
    if (!inputfile){
        cerr << "Error: cannot open file" << endl;
        inputfile.close();
        exit(1);
    }

    //テキストの読み込み
    while (getline(inputfile,filestring)){
        splitted = this->split(filestring,'\t');
        float x = stof(splitted[0]);
        float y = stof(splitted[1]);
        (this->xarray).push_back(x);
        (this->yarray).push_back(y);
    }
    //ファイルを閉じる
    inputfile.close();


};

template <typename MayBefloat>
float LinerComplement::XIntoY(MayBefloat x){
    /*
    xを入力するとyを予想してくれます.
    どんな型でも入力できますが。floatで返します。
    */
    x = float(x);
    
    if (x < 0){
        return yarray[0];
    }
    else if (x > xarray[xarray.size()-1]){
        return yarray[yarray.size() -1];
    }
    
    //前回のループの最初からスタート
    int xindex = this->forloopmark;

    //一周ぶん回そうとするが前回の続きから
    for(int i=0; i <= xarray.size() -1 ; i++){
        xindex++;
        //一周回すために最後まで行ったら最初に戻る
        if(xindex > xarray.size()-1){
            xindex = 0;
        }
        if(xarray[xindex]<x && x<xarray[xindex + 1]){
            break;
        }
    }
    this->forloopmark = xindex;
    
   /*
    int xindex;
    for( xindex=0 ; xindex<xarray.size() ; xindex++){
        if(xarray[xindex]<x && x<xarray[xindex + 1]){
            break;
        }
    }
    */
    
    float x1 = xarray[xindex];
    float y1 = yarray[xindex];
    float x2 = xarray[xindex + 1];
    float y2 = yarray[xindex + 1];
    //傾き
    float a = float(y2 - y1)/float(x2 - x1);
    //切片
    float b = ((y1+y2)/2 - a*(x1+x2)/2);;
    
    return a*x+b;
}

//split関数をクラス内部に入れました。
vector <string> LinerComplement::split(const string& input ,char delimiter){
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)){
        result.push_back(field);
    }
    return result;
}

/*
float LinerComplement::XMin(){
    //xarrayの最小値を返す関数
    
    float min = xarray[0];
    for(int i = 0; i <= xarray.size() ; ++i) {
        if (min < xarray[i]){
            min = xarray[i];
        }
    }
    return min;
}

float LinerComplement::XMax(){
    //xarrayの最小値を返す関数

    float max = xarray[0];
    for(int i = 0; i <= xarray.size() ; ++i) {
        if (max > xarray[i]){
            max = xarray[i];
        }
    }
    return max;
}
*/

int main(void){
    LinerComplement Obj("input2.text");
    float x;
    ofstream file("classresult.text");
    for(x=0.000001;x<2.0;x+=0.00001){
        file<<x<<"    "<<Obj.XIntoY(x)<<endl;
    }
    file.close();
    return 0;
}

