#include <iostream>
#include <stdexcept>
#include <vector> // OVO JE SAMO ZA FUNKCIJE PROVJERE
#include <string>
using namespace std;




//NAPOMENA
//PREPISAO "SAM OD SEBE" OD PROSLE GODINE


template <typename Tip>
class Stek
{
    struct Cvor {
        Tip el;
        Cvor* sljedeci;
        Cvor(const Tip& el, Cvor* sljedeci): el(el),sljedeci(sljedeci) {}
    };
    Cvor* vrhh;
    int duzina;
    void Kopiraj(const Stek &s);
 public:
    Stek(): vrhh(nullptr),duzina(0){}       //Obicni konstruktor
    
    ~Stek() {               //Obicni destruktor
        brisi(); 
    }
    
    Stek(const Stek &s){            //Kopirajuci konstruktor
        Kopiraj(s);
    }
    
    Stek(Stek&& s): vrhh(move(s.vrhh)), duzina(s.duzina){               //Pomjerajuci konstruktor
       s.vrhh=s.duzina=0;
    }
    
    
    Stek& operator =(Stek s){           //copy and swap             //Operator dodjele
        swap(vrhh,s.vrhh); swap(duzina,s.duzina); 
        return *this;
    }
    
    void brisi(){                       //BRISI
        Cvor* q(vrhh);
        for(Cvor* p=vrhh; p!=nullptr; p=q){
            q=p->sljedeci;
            delete p;
        }
        duzina=0;
        vrhh=0;
    }
    
    void stavi(const Tip& el){              //Stavi
        vrhh=new Cvor(el,vrhh);
        duzina++;
    }
    
    Tip skini();            //Skini
    
    Tip& vrh(){                 //VRH
        if(!duzina) throw std::logic_error("Prazan stek");
        return vrhh->el;
    }
    
    int brojElemenata() const {             //Broj elemenata
        return duzina; 
    }
};

template <typename Tip> // Kopiranje steka
Tip Stek<Tip>::skini(){
    if(!duzina)
        throw std::logic_error("Prazan stek");
    Tip element(vrhh->el);
    Cvor* p(vrhh);
    vrhh=vrhh->sljedeci;
    duzina--;
    delete p;
    return element;
}



template <typename Tip> 
void Stek<Tip>::Kopiraj(const Stek &s)
{
   Cvor* q(nullptr), *p(s.vrhh);
   duzina=s.duzina;
   vrhh=nullptr;
   while(p){
        Cvor* n(new Cvor(p->el,nullptr));
        if(!vrhh)
            vrhh=n;
        else
            q->sljedeci=n;
        p=p->sljedeci;
        q=n;
   }
}


//Kroz sve funkcije se provjerava i konstruktor kopije jer paramter nije referenca niti pokazivac

//FUNKCIJE PROVJERE          FUNKCIEJ PROVJERE             FUNKCIJE PROVJERE               FUNKCIJE PROVJERE

template <typename Tip>
void provjeriBrisanje(Stek<Tip> stek){
    if(stek.brojElemenata()){
        stek.brisi();
        if(stek.brojElemenata()==0){
            cout<<"Uredu je";
            return;
        }
        else{
            cout<<"Nije uredu";
            return;
        }
    }
}

template <typename Tip>
void provjeriStavljanje(Stek<Tip> stek){
    int br_el(stek.brojElemenata());
    stek.stavi(Tip{});
    if(stek.brojElemenata()==br_el+1 && stek.vrh()==Tip{})
        cout<<"Uredu je";
    else
        cout<<"Nije uredu";
    return;
}

template <typename Tip>
void provjeriSkidanje(Stek<Tip> stek){
    if(stek.brojElemenata()){
        vector<Tip> vektor;
        Stek<Tip> stek2(stek);
        while(stek2.brojElemenata()){
            vektor.push_back(stek2.skini());
        }
        
        int br_el(stek.brojElemenata());
        stek.skini();
        if(stek.brojElemenata()!=br_el-1){
            cout<<"Nije uredu";
            return;
        }
        for(int i=1; i<vektor.size(); i++){
            if(vektor.at(i)!=stek.skini()){
                cout<<"Nije uredu";
                return;
            }
        }
        cout<<"Uredu je";
    }
}

template <typename Tip>
void provjeriVrh(Stek<Tip> stek){
    int br1(stek.brojElemenata());
    Tip el(stek.vrh());
    int br2(stek.brojElemenata());
    Tip el2(stek.skini());
    if(el==el2 and br1==br2)
        cout<<"Uredu je";
    else
        cout<<"Nije uredu";
    return;
}

template <typename Tip>
void provjeriBrojElemenata(Stek<Tip> stek){
    int br1(stek.brojElemenata());
    stek.skini();
    int br2(stek.brojElemenata());
    stek.vrh();
    int br3(stek.brojElemenata());
    stek.brisi();
    if(br1==br2+1 and br2==br3 and stek.brojElemenata()==0){
        cout<<"Uredu je";
    }
    else
        cout<<"Nije uredu";
}

template <typename Tip>
void glavnaProvjera(Stek<Tip> stek){
    try{
        cout<<"Provjera za br elemenata: "<<endl;
        provjeriBrojElemenata(stek);
        cout<<endl;
        cout<<endl<<"Provjera za brisanje: "<<endl;
        provjeriBrisanje(stek);
        cout<<endl<<endl<<"Provjera za stavljanje: "<<endl;
        provjeriStavljanje(stek);
        cout<<endl<<endl<<"Provjera za skidanje: "<<endl;
        provjeriSkidanje(stek);
        cout<<endl<<endl<<"Provjera za vrh: "<<endl;
        provjeriVrh(stek);
        
    }
    catch(...){
        
        cout<<"Ne valja";
    }
}




int main()
{
    Stek<int> s;
    for (int i(1); i<=100; i++)
	    s.stavi(i);
    glavnaProvjera<int>(s);
    cout<<endl<<endl;
    Stek<string> s2;
    string a("abcd");
    for(int i=0; i<100; i++){
        s2.stavi(a);
        a[i%3]++;
    }
    cout<<endl;
    glavnaProvjera<string>(s2);
    return 0;
}