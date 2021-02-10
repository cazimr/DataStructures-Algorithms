#include <iostream>
#include <stdexcept>


using namespace std;


//NAPOMENA
//PREPISAO "SAM OD SEBE" OD PROSLE GODINE

template <typename Tip>
class Red
{
    struct Cvor { // Struktura čvora
        Tip el; // Informacioni dio čvora
        Cvor *sljedeci; // Pokazivač na sljedbenika
        Cvor (const Tip &el, Cvor *sljedeci): // Konstruktor čvora
            el(el), sljedeci(sljedeci) {}
    };
    Cvor *pocetak; // Pokazivač na čelo reda
    Cvor *kraj; // Pokazivač na kraj reda
    int velicina; // Veličina reda
    void Unisti(){
        while(velicina)
            skini();
    }
    void Kopiraj (const Red &red); // Kopiranje reda
public:
    Red(): pocetak(nullptr), kraj(nullptr),velicina(0){}
    Red(const Red& red): pocetak(0), kraj(0), velicina(0) { 
        Kopiraj(red); 
        
    }
    Red(Red&& red):pocetak(move(red.pocetak)), kraj(move(red.kraj)), velicina(velicina){
        red.pocetak=red.kraj=red.velicina=0;
    }
    Red& operator =(Red red){
        swap(pocetak,red.pocetak); swap(kraj,red.kraj);
        swap(velicina,red.velicina);
        return *this;
    }
    ~Red(){ Unisti(); }
    void brisi() { Unisti(); }
    void stavi(const Tip& el);
    Tip skini();
    Tip& celo(){
        if(!velicina) throw std::logic_error("Red je prazan!");
        return pocetak->el;
    }
    Tip& celo() const{
        if(!velicina) throw std::logic_error("Red je prazan!");
        return pocetak->el;
    }
    int brojElemenata() const { return velicina; };
    
    
};


template <typename Tip>
Tip Red<Tip>::skini(){
    if(pocetak==nullptr)
        throw std::logic_error("Red je prazan");
    Tip elem(pocetak->el);
    Cvor* p(pocetak);
    if(pocetak==kraj)
        pocetak=kraj=0;
    else
        pocetak=pocetak->sljedeci;
    delete p;
    velicina--;
    return elem;
}

template<typename Tip>
void Red<Tip>::stavi(const Tip& ele){
    Cvor* p = new Cvor(ele,nullptr);
    if(pocetak==nullptr)
        kraj=pocetak=p;
    else{
        kraj->sljedeci=p;
        kraj=kraj->sljedeci;
    }
    velicina++;
}

template <typename Tip>
void Red<Tip>::Kopiraj(const Red& red){
    Cvor* p(red.pocetak);
    while(p!=nullptr){
        stavi(p->el);
        p=p->sljedeci;
    }
}


//Funkcije provjere

template <typename Tip>
void provjeriBrisi(Red<Tip>& r){
    if(r.brojElemenata()==0){
        for(int i=0; i<=10; i++)
            r.stavi(Tip{});
    }
    r.brisi();
    if(r.brojElemenata()!=0) throw std::logic_error("Ne valja");
    try{ r.skini(); cout<<"Ne valja";}
    catch(...) { cout<<"Uredu je"; }
    cout<<"Uredu je";
}

template <typename Tip>
void provjeriStavi(Red<Tip>& r){
    int vel(r.brojElemenata());
    for(int i=0; i<10; i++)
        r.stavi(Tip{});
    if(r.brojElemenata()!=vel+10) throw ("Ne valja");
    cout<<"Uredu je";
    
}

template <typename Tip>
void provjeriSkini(Red<Tip>& r){
    int vel(r.brojElemenata());
    if(vel==0){
        for(int i=0; i<10; i++)
            r.stavi(Tip{});
        vel=10;
    }
    r.skini();
    if(r.brojElemenata()!=vel-1) throw ("Ne valja");
    r.brisi();
    try{r.skini(); cout<<"Ne valja";}
    catch(...) { cout<<"Uredu je"; }
}

template <typename Tip>
void provjeriCelo(Red<Tip>& r){
    if(!r.brojElemenata()){
        for(int i=0; i<10; i++)
            r.stavi(Tip{});
    }
    const Red<Tip> r2(r);
    try{cout<<r2.celo();}
    catch(...){cout<<"Ne valja";}
    
    r.celo()=Tip{};
    if(r.skini()!=Tip{}) throw ("Ne valja!");
    r.brisi();
    try{r.celo(); cout<<"Ne valja";}
    catch(...) { cout<<"Uredu je"; }
}

template <typename Tip>
void provjeriBrojElemenata(Red<Tip>& r){
    cout<<r.brojElemenata()<<endl;
    int vel(r.brojElemenata());
    if(r.brojElemenata()){
        try{r.skini(); cout<<"Uredu je";}
        catch(...){cout<<"Ne valja!";}
    }
    r.skini();
    if(r.brojElemenata()!=vel-1) cout<<"Ne valja";
    r.brisi();
    if(r.brojElemenata()!=0) cout<<"Ne valja";
    cout<<"Uredu je";
    
}


int main()
{
Red<int> r1;
for(int i=0; i<5; i++)
    r1.stavi(i);
Red<int> r2(r1);
for(int i=0; i<5; i++)
    cout<<r2.skini()<<" ";
    
//PROVJERA
cout<<"\nProvjera brisanja:\n";
provjeriBrisi(r1);
for(int i=0; i<5; i++)
    r1.stavi(i);

cout<<"\nProvjera brojElemenata:\n";
for(int i=0; i<5; i++)
    r1.stavi(i);
provjeriBrojElemenata(r1);


cout<<"\nProvjera skini:\n";
for(int i=0; i<5; i++)
    r1.stavi(i);
provjeriSkini(r1);

cout<<"\nProvjera celo:\n";
for(int i=0; i<5; i++)
    r1.stavi(i);
provjeriCelo(r1);

cout<<"\nProvjera stavi:\n";
for(int i=0; i<5; i++)
    r1.stavi(i);
provjeriStavi(r1);
    
    return 0;
}

