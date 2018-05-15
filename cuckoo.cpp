#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
vector<int> knuth(int n, int m);

vector<int> knuth(int n, int m)//find n numbers in m with equal probability
{ 
    vector<int> result;
    srand((unsigned int)time(0)); 
    for (int i = 0; i < n; i++) { 
        if (rand()%(n-i)<m) { 
            result.push_back(i);
            m--;
        }
    }
    printf("generate:");
    for(auto a:result) printf("%d ",a);
    return result;
}

class CuckooHash{
private:  
    int lnBucket;   //size of bucket  
    int *mpKeyBucket1;  //the first bucket for first hash  
    int *mpKeyBucket2;  //the second bucket for second hash  
    const int MaxLoop = 1000;  //used to control rehash loop  
    int lnCantInsertNum;  
  
private:  
    //first hash function  
    int hHashOne(int irKey)  
    {  
        int lHashKey = 0;  
  
        lHashKey = irKey % lnBucket;  
  
        return lHashKey;
    }  
  
    //second hash function  
    int hHashTwo(int irKey)  
    {  
        int lHashkey = 0;

        lHashkey = lnBucket - irKey % lnBucket;

        return lHashkey;

        /*int lHashkey = 0;
		while(irKey){
			lHashkey += irKey%7;
			irKey /= 3;
		}
		lHashkey = lHashkey % lnBucket;
        return lHashkey;*/
    }  

  
    //todo: juge one num is Prime NUM or not  
    bool hIsPrime(int inN)  
    {  
        if(inN <= 0) return false;  
  
        int last = sqrt((double)inN);  
  
        for(int i = 2; i<= last; i++)  
        {  
            if(inN % i == 0)  
                return false;  
        }  
  
        return true;  
    }  
    int hGetMinPrime(int inNum)  
    {  
        while( !hIsPrime(inNum) ) inNum ++;  
  
        return inNum;   
    }  
  
    //try to rehash all the other key  
    bool hReHash(int iKey, int deeps)  
    {  
        if(deeps <= 0) {
            cout<<" The whole thing was in an infinite loop !"<<endl;
            lnCantInsertNum++;
            return false;  
        }

  
        int lHashKey1 = hHashOne(iKey);  
        int lHashKey2 = hHashTwo(iKey);  
  
        if(iKey == mpKeyBucket1[lHashKey1])  
        {  
            if(mpKeyBucket2[lHashKey2] == 0)  
            {  
                mpKeyBucket2[lHashKey2] = iKey;  
                return true;  
            }  // 第一个槽有元素，第二个槽空
            else  
            {  
                if( hReHash(mpKeyBucket2[lHashKey2], deeps - 1) )  
                {   mpKeyBucket2[lHashKey2] = iKey;  
                    return true;  
                }  //递归寻找空槽
                      
            }  
        }  
        else if(iKey == mpKeyBucket2[lHashKey2])  
        {  
            if(mpKeyBucket1[lHashKey1] == 0)  
            {  
                mpKeyBucket1[lHashKey1] = iKey;  
                return true;  
            }  
            else  
            {  
                if( hReHash(mpKeyBucket1[lHashKey1], deeps - 1))  
                {  
                    mpKeyBucket1[lHashKey1] = iKey;  
                    return true;  
                }  
            }  
        }  
  
        return false;  
  
    }  
  
public:  
    CuckooHash(int inNum)  
    {  
        lnBucket = inNum;  
  
        mpKeyBucket1 = NULL;  
  
        mpKeyBucket2 = NULL;  
  
        lnCantInsertNum = 0;  
    }  
  
    void InitHashTable()  
    {  
        lnBucket = hGetMinPrime(lnBucket); //哈希槽的容量取质数，可以避免位失效 
        printf("the size of bucket is %d\n",lnBucket);  
        mpKeyBucket1 = new int[lnBucket]{};  //给分配的槽位置0
          
        mpKeyBucket2 = new int[lnBucket]{};  
          
    }  
  
    ~CuckooHash()  
    {  
        if(mpKeyBucket1)  
            delete[] mpKeyBucket1;  
  
        if(mpKeyBucket2)  
            delete[] mpKeyBucket2;  
    }  
  
    void Insert(int irKey)  
    {  
        if(find(irKey)) return;  
  
        int lHashKey1 = hHashOne(irKey);  
        int lHashKey2 = hHashTwo(irKey);  
  
        if(mpKeyBucket1[lHashKey1]  == 0)  
            mpKeyBucket1[lHashKey1] = irKey;  
        else if(mpKeyBucket2[lHashKey2] == 0)  
            mpKeyBucket2[lHashKey2] = irKey;  //简化为优先插入槽1
        else  
        {  
            if(hReHash(mpKeyBucket1[lHashKey1], MaxLoop))  
                mpKeyBucket1[lHashKey1] = irKey;  
            else if(hReHash(mpKeyBucket2[lHashKey2], MaxLoop))  
                mpKeyBucket2[lHashKey2] = irKey;    
        }  
  
        cout << "After insert : " << irKey << endl;  
        cout << "Position 1: "<<lHashKey1 << "   Position 2: " << lHashKey2 << endl;  
        PrintBucket();  
      
    }  
  
    bool find(int& irKey)  //查看元素是否已经存在
    {  
        int lHashKey1 = hHashOne(irKey);  
        if(mpKeyBucket1 && mpKeyBucket1[lHashKey1] == irKey)  
            return true;  
  
        int lHashKey2 = hHashTwo(irKey);  
        if(mpKeyBucket2 && mpKeyBucket2[lHashKey2] == irKey)  
            return true;  
  
        return false;  
    }  
  
    void PrintBucket()  //遍历输出所有元素值
    {     
        for(int i = 0; i<lnBucket; i++ )  
            cout << mpKeyBucket1[i] << ' ';  
        cout << endl;  
  
        for(int i = 0; i<lnBucket; i++ )  
            cout << mpKeyBucket2[i] << ' ';  
        cout << endl;  
    }  

    void PrintSuccessrate(){
        cout<<endl<<"Fail: "<<lnCantInsertNum/2<<" times"<<endl;
        cout<<"Finish!";
    }
};

int main(){
    CuckooHash cucucu(20);
    cucucu.InitHashTable();

    vector<int> numbers;
    numbers = knuth(1000,30);
    cout<<endl<<endl;
	clock_t start = clock();
    for(int i = 0;i < numbers.size();i++) {
		cout<<numbers[i]<<endl;
		cucucu.Insert(numbers[i]);
	}
    cucucu.PrintSuccessrate();
	clock_t end = clock();
	cout <<"Running time: "<<end - start<<" ms"<< endl;

    return 0;
}