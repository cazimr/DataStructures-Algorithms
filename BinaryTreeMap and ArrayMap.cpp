#include <iostream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>


using namespace std;

template <typename TKljuc, typename TVr>
class Mapa
{
public:

    Mapa() {};
    virtual ~Mapa() {};
    virtual int brojElemenata() const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const TKljuc& kljuc) = 0;
    virtual TVr& operator [](const TKljuc& kljuc)=0;
    virtual TVr operator [](const TKljuc& kljuc) const=0;
};






template<typename TKljuc, typename TVr>
class BinStabloMapa: public Mapa<TKljuc, TVr>
{
    struct Cvor {

        pair<TKljuc,TVr> el;
        Cvor* lijevi, *desni, *roditelj;

        Cvor(const pair<TKljuc,TVr>& el,Cvor* lijevi, Cvor* desni, Cvor* roditelj):
             el(el),lijevi(lijevi), desni(desni), roditelj(roditelj){}

    };

    Cvor* korijen;
    int broj_el;
    void Kopiraj(Cvor* kopirajuci,Cvor* &kopirani, Cvor* roditelj=nullptr);
    void obrisiStablo(Cvor* cvor);

    Cvor* traziCvor(Cvor* cvor,const TKljuc& kljuc) const{
        if (cvor==0 or cvor->el.first==kljuc)
            return cvor;
        else if(kljuc>cvor->el.first)
            return traziCvor(cvor->desni,kljuc);
        else
            return traziCvor(cvor->lijevi,kljuc);
    }

    TVr& traziUbaciCvor(Cvor*& cvor,const TKljuc& kljuc, Cvor* roditelj=nullptr){
        if (cvor==0){
            cvor=new Cvor(make_pair(kljuc,TVr{}),0,0,roditelj);
            broj_el++;
            return cvor->el.second;
        }
        else if(cvor->el.first==kljuc)
            return cvor->el.second;
        else if(kljuc>cvor->el.first)
            return traziUbaciCvor(cvor->desni,kljuc,cvor);
        else
            return traziUbaciCvor(cvor->lijevi,kljuc,cvor);
    }



public:
    ~BinStabloMapa(){
        obrisiStablo(korijen);
    }

    BinStabloMapa(const BinStabloMapa<TKljuc,TVr>& stablo):korijen(nullptr), broj_el(0){
        Kopiraj(stablo.korijen,korijen);
        
    }

    BinStabloMapa(BinStabloMapa<TKljuc,TVr>&& stablo):korijen(move(stablo.korijen)),broj_el(move(stablo.broj_el)){}

    BinStabloMapa& operator =(BinStabloMapa<TKljuc,TVr> stablo){
        obrisiStablo(korijen);
        
        swap(korijen,stablo.korijen);
        swap(broj_el,stablo.broj_el);
        return *this;
    }

    BinStabloMapa(): korijen(nullptr), broj_el(0) {
    };
    BinStabloMapa(const pair<TKljuc,TVr>& par): korijen(nullptr,nullptr,nullptr,nullptr), broj_el(1) {
        this->korijen = new Cvor(par,nullptr,nullptr,nullptr);
    }

    void obrisi() override {
        obrisiStablo(korijen);
    }

    void obrisi(const TKljuc& kljuc) override;

    int brojElemenata() const override{
        return broj_el;
    }

    TVr operator [](const TKljuc& kljuc) const override{
        Cvor* rez(traziCvor(korijen,kljuc));
        return rez ? rez->el.second:TVr{};
    }

    TVr& operator [](const TKljuc& kljuc) override{

        return traziUbaciCvor(korijen,kljuc);
    }






    void ispisiSve(Cvor* roditelj)const{
        if(roditelj){
            if(roditelj->desni)cout<<roditelj->desni->el.first<<" ";
            else cout<<"|";
            if(roditelj->lijevi)cout<<roditelj->lijevi->el.first<<" ";
            else cout<<"|";
            ispisiSve(roditelj->desni);
            ispisiSve(roditelj->lijevi);
        }
    }

    void ispisi() const{
        if(korijen!=nullptr) cout << korijen->el.first<<" ";
        ispisiSve(korijen);
    }




};
template<typename TKljuc, typename TVr>
void BinStabloMapa<TKljuc,TVr>::obrisi(const TKljuc& kljuc)
{
        Cvor* p=korijen, *q=0;
        Cvor* tmp=0,*pp=0,*rp=0;
        while(p!=0 and kljuc!=p->el.first){
            q=p;
            if(kljuc < p->el.first)
                p=p->lijevi;
            else
                p=p->desni;

        }
        if(p==0)
            return;
        if(p->lijevi==0)
            rp=p->desni;
        else if(p->desni==0)
            rp=p->lijevi;
        else{
            pp=p;
            rp=p->lijevi;
            tmp=rp->desni;
            while(tmp!=0){
                pp=rp;
                rp=tmp;
                tmp=rp->desni;
            }

            if(pp!=p){
                pp->desni=rp->lijevi;
                rp->lijevi=p->lijevi;
            }
            rp->desni=p->desni;
        }
        if(q==0){
            korijen=rp;
            if(rp!=nullptr)
                rp->roditelj=nullptr; //sad je on korijen i nema roditelja
        }
        else if(p==q->lijevi)
            q->lijevi=rp;
        else
            q->desni=rp;
        delete p;
        broj_el--;
}




template<typename TKljuc, typename TVr>
 void BinStabloMapa<TKljuc,TVr>::obrisiStablo(Cvor* cvor){
     if(cvor!=nullptr){
         obrisiStablo(cvor->lijevi);
         obrisiStablo(cvor->desni);
         delete cvor;
     }
     korijen=nullptr;
     broj_el=0;
 }

template<typename TKljuc, typename TVr>
void BinStabloMapa<TKljuc,TVr>::Kopiraj(Cvor* kopirajuci,Cvor* &kopirani, Cvor* roditelj){
    
    if(kopirajuci==nullptr){
        return;
    }
    kopirani = new Cvor(kopirajuci->el, nullptr, nullptr,roditelj);
    broj_el++;
    Kopiraj(kopirajuci->lijevi, kopirani->lijevi,kopirani);
    Kopiraj(kopirajuci->desni, kopirani -> desni,kopirani);
    
    return;
}


//LISTA         LISTA           LISTA           LISTA           LISTA   

template <typename Tip>
class Lista
{
public:
    Lista() {};
    virtual ~Lista() { };
    virtual int brojElemenata() const=0;
    virtual Tip& trenutni() const=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip& el)=0;
    virtual void dodajIza(const Tip& el)=0;
    virtual Tip& operator [](int i)=0;
    virtual const Tip& operator [] (int i) const=0;
};

// NIZ LISTA              NIZ LISTA                NIZ LISTA               NIZ LISTA        NIZ LISTA           NIZ LISTA
template <typename Tip>
class NizLista: public Lista<Tip>
{
    Tip** niz;
    mutable int br_brojElemenata, br_trenutni, br_prethodni, br_sljedeci,
        br_pocetak,br_kraj,br_obrisi,br_dodajIspred,bH_dodajIza,br_operator;
    int velicina,trenEl,kapacitet;
    void baciIzuzetakPrazna() const{
        throw std::logic_error("Lista je prazna");
    }
public:
    NizLista(const NizLista<Tip>& kopija); //Kopirajuci konstruktor
    
    NizLista(NizLista<Tip>&& kopija):velicina(kopija.velicina),kapacitet(kopija.kapacitet),trenEl(kopija.trenEl), niz(move(kopija.niz)){ //Pomjerajuci konstuktor
        br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
        kopija.niz=nullptr; //jako bitno da ne bi destruktor unistio
        kopija.velicina=0;
    }
    
    NizLista(int vel=50); //Obicni konstruktor
    ~NizLista(); //Destruktor
    
    inline int brojElemenata() const override { //Broj elemenata
        br_brojElemenata++;
        return velicina;
    }
    
    Tip& trenutni() const {                     //Trenutni el
        br_trenutni++;
        if(!velicina) baciIzuzetakPrazna();
        return *niz[trenEl];
    }
    
    bool prethodni() override;                  //Prethodni
    bool sljedeci() override;                   //Sljedeci
    
    void pocetak() override {                   //Pocetak
        br_pocetak++;
        if(!velicina) baciIzuzetakPrazna();
        trenEl = 0;
    };
    
    void kraj() override {                      //Kraj
        br_kraj++;
        if(!velicina) baciIzuzetakPrazna();
        trenEl = velicina-1;
    };
    
    
    void obrisi() override;                     //Obrisi
    void dodajIspred (const Tip& el);           //DodajIspred
    void dodajIza (const Tip& el);              //DodajIza
    
    void Prikazi() const {                      //Prikazi
        for(int i=0; i<velicina; i++){
            cout<<i+1<<". "<<*niz[i]<<endl;
        }
    }
    
    void postaviTrenutni(int i){
        if(i<0 or i>=velicina) throw std::domain_error("Indeks van opsega");
        pocetak();
        for(int j=0; j<i; j++)
            sljedeci();
        
    }
    
    Tip& operator [] (int i) override {                 //Operator [] indeksiranja
        br_operator++;
        if(i<0 or i>=velicina) throw std::domain_error("Indeks van opsega");
        return *(niz[i]);
    };
    
    NizLista<Tip>& operator = (NizLista<Tip> kopija); //Operator dodjele nekonstantni
    
    const Tip& operator [] (int i) const override {                 //Operator dodjele konstantni
        br_operator++;
        if(i<0 or i>=velicina) throw std::domain_error("Indeks van opsega");
        return *(niz[i]);
    };
};


template <typename Tip>
NizLista<Tip>::NizLista(const NizLista<Tip>& kopija):velicina(kopija.velicina),kapacitet(kopija.kapacitet),trenEl(kopija.trenEl){
    br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
    niz = new Tip*[kapacitet];
    for(int i=0; i<velicina; i++){
        niz[i] = new Tip(*(kopija.niz[i]));
    }
}


template <typename Tip>
NizLista<Tip>& NizLista<Tip>::operator =(NizLista<Tip> kopija){ //copy and swap - ponoviti
    br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
    swap(trenEl,kopija.trenEl);
    swap(velicina,kopija.velicina);
    swap(kapacitet,kopija.kapacitet);
    swap(niz,kopija.niz);
    return *this;
    
}

template <typename Tip>
NizLista<Tip>::~NizLista(){
    for(int i=0; i<velicina; i++)
        delete niz[i];
    delete[] niz;
}

template <typename Tip>
void NizLista<Tip>::dodajIza(const Tip& el)
{
    br_dodajIza++;
    if(velicina==kapacitet) { //Moramo prosiriti listu za novi el
        Tip** privremeni = new Tip*[kapacitet*2];
        for(int i=0; i<velicina; i++) {
            privremeni[i]=niz[i];
        }
        kapacitet*=2;
        delete[] niz;
        niz=privremeni;
    }
    if(trenEl==-1) { //Prazna lista, dodajemo novi
        niz[0] = new Tip(el);
        trenEl=0;
        velicina++;
        return;
    }
    for(int i=velicina; i>trenEl+1; i--) //Nije prazna lista, pomjerimo sve za jedno mjesto u desno uklj trenutni
        niz[i]=niz[i-1];
    niz[trenEl+1]=new Tip(el);
    velicina++;
}

template <typename Tip>
void NizLista<Tip>::dodajIspred(const Tip& el)
{
    br_dodajIspred++;
    if(velicina==kapacitet) { //Moramo prosiriti listu za novi el
        Tip** privremeni = new Tip*[kapacitet*2];
        for(int i=0; i<kapacitet; i++) {
            privremeni[i]=niz[i];
        }
        kapacitet*=2;
        delete[] niz;
        niz=privremeni;
    }
    if(trenEl==-1) { //Prazna lista, dodajemo novi
        niz[0] = new Tip(el);
        trenEl=0;
        velicina++;
        return;
    }
    for(int i=velicina; i>trenEl; i--) //Nije prazna lista, pomjerimo sve za jedno mjesto u desno uklj trenutni
        niz[i]=niz[i-1];
    niz[trenEl]=new Tip(el);
    trenEl++;
    velicina++;
}

template <typename Tip>
void NizLista<Tip>::obrisi()
{
    br_obrisi++;
    if(!velicina) baciIzuzetakPrazna();
    if(velicina>0 and trenEl==velicina-1) {
        delete niz[trenEl--];
        velicina--;
        return;
    }
    delete niz[trenEl];
    for(int i=trenEl; i<velicina-1; i++)
        niz[i]=niz[i+1];
    velicina--;
    
}

template <typename Tip>
bool NizLista<Tip>::sljedeci()
{
    br_sljedeci++;
    if(!velicina) baciIzuzetakPrazna();
    if(trenEl==velicina-1) return false;
    trenEl++;
    return true;
}

template <typename Tip>
bool NizLista<Tip>::prethodni()
{
    br_prethodni++;
    if(!velicina) baciIzuzetakPrazna();
    if(trenEl==0) return false;
    trenEl--;
    return true;
}

template <typename Tip>
NizLista<Tip>::NizLista(int vel): br_brojElemenata(0),br_trenutni(0),
    br_prethodni(0),br_sljedeci(0),br_pocetak(0),
    br_kraj(0),br_obrisi(0),br_dodajIspred(0),
    br_dodajIza(0),br_operator(0),velicina(0),trenEl(-1),kapacitet(10)
{

    niz = new Tip*[vel];
}



//NIZ MAPA NIZ MAPA NIZ MAPA





template <typename TKljuc, typename TVr>
class NizMapa: public Mapa<TKljuc,TVr>{
    
    NizLista<pair<TKljuc,TVr>> elementi;
    
    int pronadjiBinarno(const TKljuc& kljuc) const{
        int l(0),r(elementi.brojElemenata()-1),m;
        while(l<=r){
            m=l+(r-l)/2; //Izvlacimo sredinu uvijek, kako od cijelog vektora tako od segmenata binarne pretreage
            
            if(elementi[m].first==kljuc){
                return m;
            }
            else if(elementi[m].first<kljuc)
                l=m+1;
            else if(elementi[m].first>kljuc)
                r=m-1;
        }
        throw std::logic_error("Element nije pronadjen");
    }
public:
    NizMapa(){}
    int brojElemenata() const override{
        return elementi.brojElemenata();
    }
    void obrisi() override{
        while(elementi.brojElemenata())
            elementi.obrisi();
    }
    void obrisi(const TKljuc& kljuc) override{
        int m(pronadjiBinarno(kljuc));
        elementi.postaviTrenutni(m);
        elementi.obrisi();
    }
    TVr& operator [](const TKljuc& kljuc){
        try{
            int m = pronadjiBinarno(kljuc);
            return elementi[m].second;
        }
        catch(std::logic_error){
            if(!elementi.brojElemenata()){
                elementi.dodajIza(pair<TKljuc,TVr>(kljuc,TVr{}));
                return elementi[0].second;
            }
            int l(0),r(elementi.brojElemenata()-1),m;
            while(l<=r){
                m=l+(r-l)/2; //Izvlacimo sredinu uvijek, kako od cijelog vektora tako od segmenata binarne pretreage
                
                if(m==elementi.brojElemenata()-1 or m==0 or (elementi[m].first<kljuc and elementi[m+1].first>kljuc)){
                    elementi.postaviTrenutni(m);
                    elementi.dodajIza(pair<TKljuc,TVr>(kljuc,TVr{}));
                    break;
                }
                else if(elementi[m].first<kljuc)
                    l=m+1;
                else if(elementi[m].first>kljuc)
                    r=m-1;
            }
            return elementi[m+1].second;
        }
        
    }
    TVr operator [](const TKljuc& kljuc) const{
        try{
            int m = pronadjiBinarno(kljuc);
            return elementi[m].second;
        }
        catch(std::logic_error){
            return TVr{};
        }
    }
};




int main()
{
    NizMapa<int,int> nizMapa;
    BinStabloMapa<int,int> binMapa;
    srand(time(NULL));
    for(int i=0; i<1500; i++){
        nizMapa[i] = rand()%20;
        binMapa[i] = (rand()+1)%12;
    }
    
    //Generalno je kompleksnost pretrazivanja nekog elementa u stablu visine h - O(h), dok za NizMapu koja je implementirana 
    // u svojoj bazi preko niza, mi u najgorem slučaju moramo pretražiti sve elemente, sto bi bila kompleksnost O(n), međutim
    //možemo to poboljšati ako drzimo elemente sortiranim pa koristimo binarnu pretragu, pa da kompleksnost bude O(log n)
    //, ali što opet komplikuje dodavanje novog elementa jer mora uvijek biti očuvan taj sortirani poredak. Dakle očekivano je da
    //binarno stablo drastično brže pristupa elementu
    
    auto vrijeme1 = clock();
    nizMapa[4];
    auto vrijeme2 = clock();
    auto ukupno = ((vrijeme2-vrijeme1)*1.)/((CLOCKS_PER_SEC/1000)*1.);
    cout<<endl;
    cout<<"Vrijeme izvrsavanja pristupa postojecem elementu nizMape iznosi: "<<ukupno<<"ms"<<endl;
    
    vrijeme1 = clock();
    binMapa[352];
    vrijeme2 = clock();
    ukupno = ((vrijeme2-vrijeme1)*1.)/((CLOCKS_PER_SEC/1000)*1.);
    cout<<endl;
    cout<<"Vrijeme izvrsavanja pristupa postojecem elementu binStabloMape iznosi: "<<ukupno<<"ms"<<endl;
    
    //Kao sto smo ranije rekli, dodavanje je očekivano da je sporije svakako za NizMapu:
    
    
    vrijeme1 = clock();
    nizMapa[1600]=4124;
    vrijeme2 = clock();
    ukupno = ((vrijeme2-vrijeme1)*1.)/((CLOCKS_PER_SEC/1000)*1.);
    cout<<endl;
    cout<<"Vrijeme izvrsavanja dodavanja novog elementa u nizMapu iznosi: "<<ukupno<<"ms"<<endl;
    
    vrijeme1 = clock();
    binMapa[1600]=4124;
    vrijeme2 = clock();
    ukupno = ((vrijeme2-vrijeme1)*1.)/((CLOCKS_PER_SEC/1000)*1.);
    cout<<endl;
    cout<<"Vrijeme izvrsavanja dodavanja novog elementa u binStabloMapu iznosi: "<<ukupno<<"ms"<<endl;

}

