#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>

using namespace std;


template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    int p(vel), i;
    do{
        i=p-1;
        p=0;
        for(int j=1; j<=i; j++){
            if(niz[j-1]>niz[j]){
                swap(niz[j-1],niz[j]);
                p=j;
            }
        }
    }while(p!=0);
}

template<typename Tip>
void selection_sort(Tip* niz, int vel){
    Tip min;
    int imin;
    for(int i=0; i<vel-1; i++){
        min=niz[i];
        imin=i;
        for(int j=i+1; j<vel; j++){
            if(niz[j]<min){
                imin=j;
                min=niz[j];
            }
        }
        niz[imin]=niz[i];
        niz[i]=min;
    }
}

template <typename Tip>
int particija(Tip* niz, int prvi, int zadnji){
    Tip pivot = niz[prvi];
    int p(prvi+1);
    while(p<=zadnji and niz[p]<pivot){
        p++;
    }
    for(int i=p+1; i<=zadnji; i++){
        if(niz[i]<pivot)
            swap(niz[i],niz[p++]);
    }
    swap(niz[prvi],niz[p-1]);
    return p-1;
}

template<typename Tip>
void quickSort(Tip* niz,int prvi, int zadnji){
    if(prvi<zadnji){
        int j=particija(niz,prvi,zadnji);
        quickSort(niz,prvi,j-1);
        quickSort(niz,j+1,zadnji);
    }
}

template<typename Tip>
void quick_sort(Tip* niz, int vel){
    quickSort(niz,0,vel-1);
}

template <typename Tip>
void merge(Tip* niz, int l, int p, int q, int u){
    int i(0),j(q-l),k(l);
    Tip* drugi = new Tip[u-l+1];
    for(int m=0; m<=u-l; m++)
        drugi[m]=niz[l+m];
    while(i<=p-l and j<=u-l){
        
        if(drugi[i]<drugi[j])
            niz[k]=drugi[i++];
        else
            niz[k]=drugi[j++];
        k++;
    }
    while(i<=p-l)
        niz[k++]=drugi[i++];
    
    while(j<=u-l)
        niz[k++]=drugi[j++];
    
    delete[] drugi;
}

template <typename Tip>
void mergeSort(Tip* niz, int l, int u){
    if(u>l){
        int p((u+l-1)/2);
        int q(p+1);
        mergeSort(niz,l,p);
        mergeSort(niz,q,u);
        merge(niz,l,p,q,u);
    }
}



template<typename Tip>
void merge_sort(Tip* niz, int vel){
    mergeSort(niz,0,vel-1);
}

void ucitaj(string filename, int*& niz, int& vel){
    ifstream ulazni(filename);
    if(!ulazni){
        cout<<"Datoteka '"<<filename<<"' ne postoji";
        return;
    }
    int a;
    vel=0;
    while(!ulazni.eof()){
        ulazni>>a;
        vel++;
        if(!ulazni.eof() && !ulazni){
            cout<<"GRESKA";
            return;
        }
    }
    ulazni.seekg(0,ios::beg);
    niz=new int[vel];
    int b(0);
    while(!ulazni.eof())
        ulazni>>niz[b++];
}

void generisi(string filename, int vel){
    ofstream dat(filename);
    for(int i=0; i<vel; i++){
        dat<<rand()%10000;
        if(i!=vel-1)dat<<" ";
    }
}

template <typename Tip>
void pozoviIspisi(void (*sortiranje)(Tip*,int),Tip* niz, int vel){
    clock_t vrijeme1 = clock();
    sortiranje(niz,vel);
    clock_t vrijeme2 = clock();
    cout<<endl<<"Vrijeme izvrsavanja: "<<setprecision(3)<<fixed<<(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000)<<"ms"<<endl;
}

int main() {
    int* niz(nullptr),vel(2000),n;
    generisi("test.txt",2000);
    ucitaj("test.txt",niz,vel);
    do{
        cout<<"Unesite redni broj opcije: \n0.Generisi nove brojeve \n1.Bubble Sort\n2.Selection sort\n3.Merge sort\n4.Quick sort\n5. Izlaz\nOpcija: ";
        cin>>n;
        switch(n){
            case 0:
                delete[] niz;
                cout<<"Koliko brojeva zelite generisati: ";
                do{
                    cin>>vel;
                    if(vel<=0) cout<<"Broj mora biti prirodan!\n";
                }while(vel<=0);
                
                generisi("test.txt",vel);
                ucitaj("test.txt",niz,vel);
                cout<<endl<<"Brojevi su uspjesno generisani."<<endl<<endl;
                break;
            case 1:
                pozoviIspisi<int>(bubble_sort<int>,niz,vel);
                break;
            case 2:
                pozoviIspisi<int>(selection_sort<int>,niz,vel);
                break;
            case 3:
                pozoviIspisi<int>(merge_sort<int>,niz,vel);
                break;
            case 4:
                pozoviIspisi<int>(quick_sort<int>,niz,vel);
                break;
            case 5:
                cout<<endl<<"Dovidjenja!";
                break;
            default:
                cout<<"Pogresan odabir!";
                delete[] niz;
                return 0;
        }
        if(n>=1 and n<=4){
            for(int i=0; i<vel-1; i++){
                if(niz[i]>niz[i+1]){
                    cout<<"NIJE SORTIRAN!";
                    delete[] niz;
                    return 0;
                }
            }
            cout<<endl<<endl<<"Sort je uspjesno prosao provjeru"<<endl<<endl;
        }
    }while(n!=5);
    ofstream izlaz("izlazna.txt");
    for(int i=0; i<vel; i++){
        izlaz<<niz[i];
        if(i!=vel-1) izlaz<<" ";
    }
    delete[] niz;
    return 0;
}
