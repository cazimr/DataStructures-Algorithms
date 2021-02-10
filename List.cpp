#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <string>

using namespace std;

// LISTA               LISTA                    LISTA               LISTA               LISTA

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

//JPLISTA                  JPLISTA                 JPLISTA             JPLISTA          JPLISTA

template<typename Tip>
class JednostrukaLista: public Lista<Tip>{
    struct Cvor{
        Tip el;
        Cvor* sljedeci;
        Cvor(const Tip& el, Cvor* sljedeci): el(el),sljedeci(sljedeci){}
    };
    mutable int br_brojElemenata, br_trenutni, br_prethodni, br_sljedeci,
        br_pocetak,br_kraj,br_obrisi,br_dodajIspred,br_dodajIza,br_operator;
    int duzina;
    Cvor* pocetakC;
    Cvor* krajC;
    Cvor* tekuci;
    void provjeriJeLiPrazna() const {               //Provjera na postojanje liste
        if(!duzina)
            throw std::logic_error("Lista je prazna");
    }
 public:
    JednostrukaLista(const JednostrukaLista<Tip>& kopija);  //Kopirajuci
    JednostrukaLista(JednostrukaLista<Tip>&& kopija);       //pomjerajuci
    JednostrukaLista<Tip>& operator =(JednostrukaLista<Tip> kopija);        //Operator dodjele
    
    JednostrukaLista(): br_brojElemenata(0), br_trenutni(0), br_prethodni(0), br_sljedeci(0),
        br_pocetak(0),br_kraj(0),br_obrisi(0),br_dodajIspred(0),br_dodajIza(0),br_operator(0),duzina(0),
        pocetakC(nullptr),krajC(nullptr),tekuci(nullptr){} //Klasicni konstruktor
        
    ~JednostrukaLista();                                        //Destruktor
    
    int brojElemenata() const override {                        //BrojElemenata
        br_brojElemenata++;
        return duzina; 
    }           
    
    Tip& trenutni() const override {                            //Trenutni
        br_trenutni++;
        provjeriJeLiPrazna();
        return tekuci->el;
    }
    
    bool prethodni() override;                              //Prethodni
    bool sljedeci() override;                               //Sljedeci
    
    inline void pocetak() override {                        //Pocetak
        br_pocetak++;
        provjeriJeLiPrazna();
        tekuci = pocetakC; 
    }
    
    inline void kraj() override {                           //Kraj
        br_kraj++;
        provjeriJeLiPrazna();
        tekuci = krajC; 
    }
    
    void obrisi() override ;                            //Obrisi
    void dodajIspred(const Tip& el) override;           //dodajIspred
    void dodajIza(const Tip& el) override;              //Dodaj Iza
    
    Tip& operator [](int i) override;                   //Operator [] indeksiranja nekonstantni
    const Tip& operator [](int i) const override;       //Operator [] indeksiranja KONSTANTNI
    void Prikazi();                                     //Prikazi
};  


template <typename Tip>
const Tip& JednostrukaLista<Tip>::operator [](int i) const {
    br_operator++;
    if(i<0 or i>=duzina) throw std::domain_error("Indeks van opsega");
    Cvor* p(pocetakC);
    for(int j=0; j<i; j++){
        p=p->sljedeci;
    }
    return p->el;
}

template<typename Tip>
void JednostrukaLista<Tip>::Prikazi(){
    int i=1;
    for(Cvor* p=pocetakC; p!=nullptr;){
        cout<<i<<". "<<p->el<<endl;
        p=p->sljedeci;
        i++;
    }
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista<Tip>& kopija){
    br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
    pocetakC = krajC = tekuci = new Cvor(kopija.pocetakC->el,nullptr);
    duzina=1;
    Cvor* q(kopija.pocetakC->sljedeci);
    while(q!=nullptr){
        dodajIza(q->el);
        kraj();
        q=q->sljedeci;
    } 
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(JednostrukaLista<Tip>&& kopija): duzina(kopija.duzina), pocetakC(kopija.pocetakC), krajC(kopija.krajC),tekuci(kopija.tekuci){
    br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
    kopija.pocetakC=kopija.krajC=kopija.tekuci=nullptr;
    kopija.duzina=0;
}

template <typename Tip>
JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator =(JednostrukaLista<Tip> kopija){
    br_brojElemenata=br_trenutni=br_prethodni= br_sljedeci=br_pocetak=br_kraj=br_obrisi=br_dodajIspred=br_dodajIza=br_operator=0;
    swap(duzina,kopija.duzina);
    swap(pocetakC,kopija.pocetakC); swap(krajC,kopija.krajC);
    swap(tekuci,kopija.tekuci);
    return *this;
}


template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista(){
    Cvor* q(pocetakC);
    for(Cvor* p=pocetakC; p!=nullptr; p=q){
        q=p->sljedeci;
        delete p;
    }
        
}

template <typename Tip>
Tip& JednostrukaLista<Tip>::operator [](int i){
    br_operator++;
    if(i<0 or i>=duzina) throw std::domain_error("Indeks van opsega");
    Cvor* p(pocetakC);
    for(int j=0; j<i; j++){
        p=p->sljedeci;
    }
    return p->el;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip& el){
    br_dodajIza++;
     if(!duzina){
         tekuci=pocetakC=krajC=new Cvor(el,nullptr);
         duzina=1;
         return;
     }
     Cvor* sljedbenik(tekuci->sljedeci);
     
     if(tekuci==krajC){
         
         sljedbenik=new Cvor(el,nullptr);
         krajC->sljedeci=sljedbenik;
         krajC=sljedbenik;
         duzina++;
         return;
     }
     
     tekuci->sljedeci=new Cvor(el,sljedbenik);
     duzina++;
 }
 

template<typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip& element){
    br_dodajIspred++;
     if(!duzina){
         tekuci=pocetakC=krajC=new Cvor(element,nullptr);
         duzina=1;
         return;
     }
     Cvor* p(pocetakC);
     if(tekuci==pocetakC){
         pocetakC=new Cvor(element,tekuci);
         duzina++;
         return;
     }
     
     while(p->sljedeci!=tekuci)
        p=p->sljedeci;
     p->sljedeci=new Cvor(element,tekuci);
     duzina++;
 }

template<typename Tip>
void JednostrukaLista<Tip>::obrisi(){
    br_obrisi++;
    provjeriJeLiPrazna();
    if(duzina==1){
        delete tekuci;
        tekuci = pocetakC = krajC =nullptr;
        duzina--;
        return;
    }
    Cvor* p;
    if(tekuci==krajC){
        p=tekuci;
        prethodni();
        tekuci->sljedeci=nullptr;
        krajC=tekuci;
        duzina--;
        delete p;
        return;
    }
    else if(tekuci==pocetakC){
        p=tekuci;
        sljedeci();
        delete p;
        pocetakC=tekuci;
        duzina--;
    }
    else{
        p=tekuci;
        prethodni();
        Cvor* q(tekuci);
        sljedeci(); sljedeci();
        q->sljedeci = tekuci;
        delete p;
        duzina--;
    }
}


template<typename Tip>
bool JednostrukaLista<Tip>::sljedeci(){
    br_sljedeci++;
    provjeriJeLiPrazna();
    if(krajC==tekuci)
        return false;
    else
        tekuci=tekuci->sljedeci;
    return true;
}

template<typename Tip>
bool JednostrukaLista<Tip>::prethodni(){
    br_prethodni++;
    provjeriJeLiPrazna();
    if(pocetakC==tekuci)
        return false;
        
    if(pocetakC->sljedeci==tekuci)
        tekuci=pocetakC;
    
    else{
        Cvor* p(pocetakC);
        while(p->sljedeci!=tekuci)
            p=p->sljedeci;
        tekuci=p;
    }
    return true;
}

// NIZ LISTA              NIZ LISTA                NIZ LISTA               NIZ LISTA        NIZ LISTA           NIZ LISTA
template <typename Tip>
class NizLista: public Lista<Tip>
{
    Tip** niz;
    mutable int br_brojElemenata, br_trenutni, br_prethodni, br_sljedeci,
        br_pocetak,br_kraj,br_obrisi,br_dodajIspred,br_dodajIza,br_operator;
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
        delete niz;
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
    br_dodajIza(0),br_operator(0),velicina(0),trenEl(-1),kapacitet(vel)
{

    niz = new Tip*[vel];
}


//FUNKCKIJE PROVJERE            FUNKCIEJ PROVJERE           FUNKCIJE PROVJERE           FUNKCIJE PROVJERE
    
template<typename Tip>
void provjeriBrojElemenata(Lista<Tip>* lista){                   //provjera za broj elemenata
    cout<<lista->brojElemenata()<<endl;
    Tip neki{};
    for(int i=1; i<=200; i++)
        lista->dodajIspred(neki);
    cout<<lista->brojElemenata()<<endl;
    for(int i=1; i<=100; i++)
        lista->obrisi();
    cout<<lista->brojElemenata()<<endl;
    cout<<"Uredu je\n";
}

template <typename Tip>
void provjeriTrenutni(Lista<Tip>* lista){
    if(lista->brojElemenata())
        cout<<lista->trenutni()<<endl;        //ako je prazna treba bai
    while(lista->brojElemenata()){   
        lista->obrisi();
    }
    try{
        lista->trenutni();
    }
    catch(...){
        cout<<"Uredu je\n";
    }
}

template <typename Tip>
void provjeriPrethodni(Lista<Tip>* lista){
    if(lista->brojElemenata()){
        lista->kraj();
        if((lista->brojElemenata())>1){
            if(!(lista->prethodni()))
                throw std::logic_error("Ne valja");
            lista->pocetak();
        }
        if(!(lista->prethodni()))
            cout<<"Uredu je"<<endl;
    }
}

template <typename Tip>
void provjeriSljedeci(Lista<Tip>* lista){
    if(lista->brojElemenata()){
        lista->pocetak();
        if(lista->brojElemenata()>1){
            if(!(lista->sljedeci()))
                cout<<"Ne valja";
            lista->kraj();
        }
        if(!(lista->sljedeci()))
            cout<<"Uredu je";
    }
}

template <typename Tip>
void provjeriPocetak(Lista<Tip>* lista){
    if(lista->brojElemenata()){
        lista->pocetak();
      if(lista->trenutni()==(*lista)[0]) cout<<"Uredu je";
    }
    else{
        try{
            lista->pocetak(); //mora bacit izuzetak
            cout<<"Nije uredu";
        }
        catch(...){
            cout<<"Uredu je";
        }
    }
}

template <typename Tip>
void provjeriKraj(Lista<Tip>* lista){
    if(lista->brojElemenata()){
        lista->kraj();
      if(lista->trenutni()==(*lista)[lista->brojElemenata()-1])
        cout<<"Uredu je";
    }
    else{
        try{
            lista->kraj(); //mora bacit izuzetak
            cout<<"Nije uredu";
        }
        catch(...){
            cout<<"Uredu je";
        }
    }
}

template <typename Tip>
void provjeriObrisi(Lista<Tip>* lista){
    if(lista->brojElemenata()){
        int brojEl=lista->brojElemenata();
        lista->obrisi();
        if(lista->brojElemenata()!=brojEl-1){
            cout<<"Nije uredu";
            return;
        }
        while(lista->brojElemenata())
            lista->obrisi();
        try{
            cout<<lista->brojElemenata();
            lista->obrisi();
            cout<<"Nije uredu";
        }
        catch(...){
            cout<<"Uredu je";
        }
    }
    else{
        try{
            lista->obrisi();
        }
        catch(...){
            cout<<"Uredu je";
        }
    }
}

template <typename Tip>
void provjeriDodajIspred(Lista<Tip>* lista){
    int brEl(lista->brojElemenata());
    lista->dodajIspred(Tip{});
    if(lista->brojElemenata()!=brEl+1){
        cout<<"Nije uredu";
        return;
    }
    lista->pocetak();
    lista->dodajIspred(Tip{});
    lista->pocetak();
    if(lista->trenutni()!=Tip{}){
        cout<<"Nije uredu";
        return;
    }
    cout<<"Uredu je";
    
}

template <typename Tip>
void provjeriDodajIza(Lista<Tip>* lista){
    int brEl(lista->brojElemenata());
    lista->dodajIspred(Tip{});
    if(lista->brojElemenata()!=brEl+1){
        cout<<"Nije uredu";
        return;
    }
    lista->kraj();
    lista->dodajIza(Tip{});
    if(lista->trenutni()!=Tip{}){
        cout<<"Nije uredu";
        return;
    }
    cout<<"Uredu je";
}

template <typename Tip>
void provjeriOperatorIndeksiranja (Lista<Tip>* lista){
    if(lista->brojElemenata()){
        lista->pocetak();
        if(lista->trenutni()!=(*lista)[0]){
            cout<<"Nije uredu";
            return;
        }
        lista->kraj();
        if(lista->trenutni()!=(*lista)[lista->brojElemenata()-1]){
            cout<<"Nije uredu";
            return;
        }
        
    }
    cout<<"Uredu je";
}

template <typename Tip>
void glavnaProvjera(Lista<Tip>* lista){
    try{
        cout<<"Provjera za br elemenata: "<<endl;
        provjeriBrojElemenata(lista);
        cout<<endl;
        cout<<"Provjera za trenutni: "<<endl;
        provjeriTrenutni(lista);
        for(int i=0; i<10; i++)
            lista->dodajIspred(Tip{});
        cout<<endl<<"Provjera za sljedeci: "<<endl;
        provjeriSljedeci(lista);
        cout<<endl<<endl<<"Provjera za prethodni: "<<endl;
        provjeriPrethodni(lista);
        cout<<endl<<endl<<"Provjera za pocetak: "<<endl;
        provjeriPocetak(lista);
        cout<<endl<<endl<<"Provjera za kraj: "<<endl;
        provjeriKraj(lista);
        cout<<endl<<endl<<"Provjera za obrisi"<<endl;
        provjeriObrisi(lista);
        for(int i=0; i<10; i++)
            lista->dodajIspred(Tip{});
        cout<<endl<<endl<<endl<<"Provjera za dodajIza: "<<endl;
        provjeriDodajIza(lista);
        cout<<endl<<endl<<"Provjera za dodajIspred: "<<endl;
        provjeriDodajIspred(lista);
        cout<<endl<<endl<<"Provjera za indeksaciju: "<<endl;
        provjeriOperatorIndeksiranja(lista);
        
    }
    catch(...){
        
        cout<<"Ne valja";
    }
}


int main(){
    Lista<int>* Nizlista = new NizLista<int>();
    Lista<int>* Jlista = new JednostrukaLista<int>();
    for(int i=0; i<10; i++){
        Nizlista->dodajIspred(i);
        Jlista->dodajIspred(i);
    }
    glavnaProvjera(Nizlista);
    glavnaProvjera(Jlista);
    delete Nizlista;
    delete Jlista;
    
    //Sad za string
    Lista<string>* lista = new NizLista<string>();
    Lista<string>* lista2 = new JednostrukaLista<string>();
    string a("abc");
    for(int i=0; i<10; i++){
        lista->dodajIspred(a);
        lista2->dodajIspred(a);
        a[0]+=i;
    }
    glavnaProvjera(lista);
    glavnaProvjera(lista2);
    delete lista;
    delete lista2;
}

