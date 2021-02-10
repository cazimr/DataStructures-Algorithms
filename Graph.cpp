#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <queue>


//U SLUČAJU POJAVLJIVANJA PORUKE DA JE PREVISE KOPIRANO NA OVOJ ZADACI - KOPIRANI SU AUTOTESTOVI SA ZAMGERA RADI PROVJERE, JER JE ROK ISTI DAN KAD JE IZRADA
//SAMO U 12 SATI.

using namespace std;

template <typename TipOznake>
class UsmjereniGraf;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class Grana;


template <typename TipOznake>
class GranaIterator;


template <typename TipOznake>
class Cvor
{
    TipOznake oznaka;
    int redni_broj;
    UsmjereniGraf<TipOznake>* pripadniGraf;
    friend class Grana<TipOznake>;
public:
     Cvor(const Cvor<TipOznake>& cvoric){
         oznaka=cvoric.oznaka;
         redni_broj=cvoric.redni_broj;
         pripadniGraf=cvoric.pripadniGraf;
     }
    Cvor(UsmjereniGraf<TipOznake>* graf, int broj)
    {
        pripadniGraf=graf;
        redni_broj=broj;
        oznaka=TipOznake{};
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake oznaka)
    {
        this->oznaka=oznaka;
        pripadniGraf->postaviOznakuCvora(redni_broj,oznaka);
    }
    int dajRedniBroj() const
    {
        return redni_broj;
    }
};



template <typename TipOznake>
class Grana
{
    Cvor<TipOznake> *polazni,*dolazni;
    float tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake>* pripadniGraf;
    friend class UsmjereniGraf<TipOznake>;
public:
    Grana(const Cvor<TipOznake>& pol, const Cvor<TipOznake>& dol,float tezina=0):tezina(tezina)
    {
        polazni=new Cvor<TipOznake>(pol);
        dolazni = new Cvor<TipOznake>(dol);
        pripadniGraf=polazni->pripadniGraf;

    };

    ~Grana()
    {
        delete polazni;
        delete dolazni;
    }

    float dajTezinu()
    {
        return tezina;
    }
    void postaviTezinu(float tezina)
    {
        this->tezina=tezina;
        pripadniGraf->postaviTezinuGrane(polazni->dajRedniBroj(),dolazni->dajRedniBroj(),tezina);
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }

    void postaviOznaku(TipOznake oznaka)
    {
        this->oznaka=oznaka;
        pripadniGraf->postaviOznakuGrane(polazni->dajRedniBroj(),dolazni->dajRedniBroj(),oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor()
    {
        return *polazni;
    }

    Cvor<TipOznake> dajDolazniCvor()
    {
        return *dolazni;
    }
};


template <typename TipOznake>
class UsmjereniGraf{
protected:
    int br_cvorova;
public:
    UsmjereniGraf(int broj_cvorova){
        if(broj_cvorova<=0) throw logic_error("Broj cvorova mora biti prirodan broj!");
        br_cvorova=broj_cvorova;
    }
    virtual ~UsmjereniGraf(){};
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int novi)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina=0)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni,float tezina)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const=0;
    virtual bool postojiGrana(int polazni, int dolazni) const=0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor) const=0;
    virtual void postaviOznakuGrane(int polazni,int dolazni, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazni,int dolazni) const=0;
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) =0;
    virtual Cvor<TipOznake> dajCvor(int cvor) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj()  = 0;
};


template <typename TipOznake>
class MatricaGraf:public UsmjereniGraf<TipOznake>
{
    int** M; //matrica susjedstva
    TipOznake *oznakeCvorova;
    TipOznake **oznakeGrana;
    float** tezineGrana; //matrica tezina grana
    
    void validanPolDol(int polazni, int dolazni) const{
        if ((polazni<0 || polazni>=this->br_cvorova) && (dolazni<0 || dolazni>=this->br_cvorova)) throw logic_error("Nevalidani i polazni i dolazni cvor!");
        else if(polazni<0 || polazni>=this->br_cvorova) throw logic_error("Nevalidan polazni cvor!");
        else if(dolazni<0 ||dolazni>=this->br_cvorova) throw logic_error("Nevalidan dolazni cvor!");
    }
    
    void validanCvor(int cvor) const{
        if(cvor<0 || cvor>=this->br_cvorova) throw logic_error("Cvor ne postoji!");
    }
    
public:
    
    MatricaGraf(int broj_cvorova):UsmjereniGraf<TipOznake>(broj_cvorova){
        
        //matrica susjedstva, int-ovi 0 i 1
        M=new int*[this->br_cvorova];
        for(int i=0; i<this->br_cvorova; i++){
            M[i]=new int[this->br_cvorova];
            for(int j=0; j<this->br_cvorova; j++)
                M[i][j]=0;
        }
        
        //oznake cvorova, niz od br_cvorova elemenata, svi postavljeni na default
        oznakeCvorova=new TipOznake[this->br_cvorova];
        for(int i=0; i<this->br_cvorova; i++)
            oznakeCvorova[i]=TipOznake{};
        
        //oznake grana opet matrica, svaka grana postavljena na default oznaku za dati tip oznake
        oznakeGrana=new TipOznake*[this->br_cvorova];
        for(int i=0; i<this->br_cvorova; i++){
            oznakeGrana[i]=new TipOznake[this->br_cvorova];
            for(int j=0; j<this->br_cvorova; j++)
                oznakeGrana[i][j]=TipOznake{};
        }
        
        //tezine svih grana
        tezineGrana=new float*[this->br_cvorova];
        for(int i=0; i<this->br_cvorova; i++){
            tezineGrana[i]=new float[this->br_cvorova];
            for(int j=0; j<this->br_cvorova; j++)
                tezineGrana[i][j]=0;
        }
    }
    
    
    ~MatricaGraf(){
        
        for(int i=0; i<this->br_cvorova; i++)
            delete[] M[i];
        delete[] M;
        
        delete[] oznakeCvorova;
        
        for(int i=0; i<this->br_cvorova; i++)
            delete[] oznakeGrana[i];
        delete[] oznakeGrana;
        
        for(int i=0; i<this->br_cvorova; i++)
            delete[] tezineGrana[i];
        delete[] tezineGrana;
        
    }
    
    int dajBrojCvorova() const override{
        return this->br_cvorova;
    }
    void postaviBrojCvorova(int novi) override{
        if(novi==this->br_cvorova) throw logic_error("Zeljeni broj cvorova je aktuelni broj!");
        if(novi<this->br_cvorova) throw logic_error("Nije dozvoljeno smanjenje broja cvorova!");
        
        //matrica susjedstva
        int** M2=new int*[novi];
        for(int i=0; i<novi; i++){
            M2[i]=new int[novi];
            for(int j=0; j<novi; j++){
                if(j<this->br_cvorova) M2[i][j]=M[i][j];
                else M2[i][j]=0;
            }
        }
        for(int i=0; i<this->br_cvorova; i++)
            delete[] M[i];
        delete[] M;
        
        M=M2;
        
        //oznake cvorova, niz od br_cvorova elemenata, svi postavljeni na default
        TipOznake* oznakeCvorova2=new TipOznake[novi];
        for(int i=0; i<novi; i++){
            if(i<this->br_cvorova) oznakeCvorova2[i]=oznakeCvorova[i];
            else oznakeCvorova2[i]=TipOznake{};
        }
        
        delete[] oznakeCvorova;
        oznakeCvorova=oznakeCvorova2;
        
        //oznake grana opet matrica, svaka grana postavljena na default oznaku za dati tip oznake
        TipOznake** oznakeGrana2=new TipOznake*[novi];
        for(int i=0; i<novi; i++){
            oznakeGrana2[i]=new TipOznake[novi];
            for(int j=0; j<novi; j++){
                if(j<this->br_cvorova) oznakeGrana2[i][j]=oznakeGrana[i][j];
                else oznakeGrana2[i][j]=TipOznake{};
            }
        }
        for(int i=0; i<this->br_cvorova; i++)
            delete[] oznakeGrana[i];
        delete[] oznakeGrana;
        
        oznakeGrana=oznakeGrana2;
        
        //oznake grana opet matrica, svaka grana postavljena na default oznaku za dati tip oznake
        float** tezineGrana2=new float*[novi];
        for(int i=0; i<novi; i++){
            tezineGrana2[i]=new float[novi];
            for(int j=0; j<novi; j++){
                if(j<this->br_cvorova) tezineGrana2[i][j]=tezineGrana[i][j];
                else tezineGrana2[i][j]=0;
            }
        }
        for(int i=0; i<this->br_cvorova; i++)
            delete[] tezineGrana[i];
        delete[] tezineGrana;
        
        tezineGrana=tezineGrana2;
        this->br_cvorova=novi;
    }
    
    void dodajGranu(int polazni, int dolazni, float tezina=0) override{
        validanPolDol(polazni,dolazni); 
        if(postojiGrana(polazni,dolazni)) throw logic_error("Grana vec postoji!");
        M[polazni][dolazni]=1;
        tezineGrana[polazni][dolazni]=tezina;
    }
    
    void obrisiGranu(int polazni, int dolazni) override {
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        M[polazni][dolazni]=0;
        tezineGrana[polazni][dolazni]=0;
        oznakeGrana[polazni][dolazni]=TipOznake{};
    };
    
    void postaviTezinuGrane(int polazni, int dolazni,float tezina) override{
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        tezineGrana[polazni][dolazni]=tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni) const override{
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        return tezineGrana[polazni][dolazni];
    }
    
    bool postojiGrana(int polazni, int dolazni) const override{
        validanPolDol(polazni,dolazni);
        return M[polazni][dolazni]==1;
    }
    
    void postaviOznakuCvora(int cvor, TipOznake oznaka) override{
        validanCvor(cvor);
        oznakeCvorova[cvor]=oznaka;
    }
    TipOznake dajOznakuCvora(int cvor) const override{
        validanCvor(cvor);
        return oznakeCvorova[cvor];
    }
    
    void postaviOznakuGrane(int polazni,int dolazni, TipOznake oznaka) override{
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        oznakeGrana[polazni][dolazni]=oznaka;
    }
    TipOznake dajOznakuGrane(int polazni,int dolazni) const override{
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        return oznakeGrana[polazni][dolazni];
    }
    
    Grana<TipOznake> dajGranu(int polazni, int dolazni) override{
        validanPolDol(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw logic_error("Grana ne postoji!");
        Cvor<TipOznake> pol(this,polazni);
        pol.postaviOznaku(oznakeCvorova[polazni]);
        Cvor<TipOznake> dol(this,dolazni);
        dol.postaviOznaku(oznakeCvorova[dolazni]);
        Grana<TipOznake> grancica(pol,dol,tezineGrana[polazni][dolazni]);
        grancica.postaviOznaku(oznakeGrana[polazni][dolazni]);
        return grancica;
    }
    
    virtual Cvor<TipOznake> dajCvor(int cvor) override{
        validanCvor(cvor);
        Cvor<TipOznake> cv(this,cvor);
        cv.postaviOznaku(oznakeCvorova[cvor]);
        return cv;
    }
    
    GranaIterator<TipOznake> dajGranePocetak() override{
        return GranaIterator<TipOznake>(this,0,0);
    }
    
    GranaIterator<TipOznake> dajGraneKraj() override{
        return GranaIterator<TipOznake>(this,this->br_cvorova, 0);   
    }
    
};



template<typename TipOznake>
class GranaIterator
{
    UsmjereniGraf<TipOznake> *graf;
    int pol,dol;
    void pomjeriNaSljedeci(){
        int zadnji=graf->dajBrojCvorova();
        while(pol<zadnji){
            dol++;
            if(dol==zadnji){
                dol=0;
                pol++;
            }
            if(pol==zadnji) break;
            if(graf->postojiGrana(pol,dol)) break;
        }
    }
public:

    GranaIterator(UsmjereniGraf<TipOznake>* graf, int polazni, int dolazni){
        this->graf=graf;
        pol=polazni; 
        dol=dolazni;
        if(pol<graf->dajBrojCvorova() && !(graf->postojiGrana(pol,dol))) pomjeriNaSljedeci();
    }
    
    Grana<TipOznake> operator*(){
        if(pol==graf->dajBrojCvorova()) throw ("Iterator izasao van ospega!");
        return graf->dajGranu(pol,dol);
    }
    
    bool operator ==(const GranaIterator &iter){
        return this->graf==iter.graf && this->pol==iter.pol && this->dol==iter.dol;
    }
    
    bool operator !=(const GranaIterator &iter) const{
        return this->graf!=iter.graf || this->pol!=iter.pol || this->dol!=iter.dol;
    }
    
    GranaIterator& operator ++(){
        pomjeriNaSljedeci();
        return *this;
    }
    
    GranaIterator operator ++(int){
        pomjeriNaSljedeci();
        return GranaIterator<TipOznake>(graf,pol,dol);
    }
    
};



template<typename TipOznake>
int PrviSusjed(UsmjereniGraf<TipOznake> *g, int v){
    int max=g->dajBrojCvorova();
    for(int i=0; i<max; i++){
        if(g->postojiGrana(v,i))
            return i;
            
    }
    return g->dajBrojCvorova();
}

template<typename TipOznake>
int SljedeciSusjed(UsmjereniGraf<TipOznake> *g,int v1, int v2){
    int max=g->dajBrojCvorova();
    for(int i=v2+1; i<max; i++){
        if(g->postojiGrana(v1,i))
            return i;
    }
    return g->dajBrojCvorova();
}

template<typename TipOznake>
void TraziPoSirini(UsmjereniGraf<TipOznake> *g,int s,  vector<Cvor<TipOznake>> &rez,vector<bool> &posjecenja){
    queue<int> Q;
    Q.push(s);
    posjecenja[s]=true;
    while(!Q.empty()){
        int v=Q.front();
        Q.pop();
        rez.push_back(g->dajCvor(v));
        int max=g->dajBrojCvorova();
        for(int w=PrviSusjed(g,v); w<max; w=SljedeciSusjed(g,v,w)){
            if(posjecenja[w]==false){
                posjecenja[w]=true;
                Q.push(w);
            }
        }
    }
}


template<typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g,vector<Cvor<TipOznake>> &rez,Cvor<TipOznake> pocetni){
    int redni=pocetni.dajRedniBroj();
    int max=g->dajBrojCvorova();
    vector<bool> posjecenja(max,false);

    for(int v=redni; v<max; v++){
        if(posjecenja[v]==false)
            TraziPoSirini(g,v,rez,posjecenja);
    }
}


template<typename TipOznake>
void TraziPoDubini(UsmjereniGraf<TipOznake> *g,int s,  vector<Cvor<TipOznake>> &rez,vector<bool> &posjecenja){
    rez.push_back(g->dajCvor(s));
    posjecenja[s]=true;
    int max=g->dajBrojCvorova();
    for(int w=PrviSusjed(g,s); w<max; w=SljedeciSusjed(g,s,w)){
        if(posjecenja[w]==false){
            TraziPoDubini(g,w,rez,posjecenja);
        }
    }
}

template<typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g,vector<Cvor<TipOznake>> &rez,Cvor<TipOznake> pocetni){
    int redni=pocetni.dajRedniBroj();
    int max=g->dajBrojCvorova();
    vector<bool> posjecenja(max,false);

    for(int v=redni; v<max; v++){
        if(posjecenja[v]==false)
            TraziPoDubini(g,v,rez,posjecenja);
    }
}



int main()
{
UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g, dfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < dfs_obilazak.size(); i++)
      cout << dfs_obilazak[i].dajRedniBroj() << ",";
  delete g;
  
  cout<<endl<<endl;
  
  UsmjereniGraf<bool> *g2 = new MatricaGraf<bool>(6);
    g2->dodajGranu(0, 1, 2.5);
    g2->dodajGranu(1, 2, 1.2);
    g2->dodajGranu(1, 3, 0.1);
    g2->dodajGranu(2, 4, 3.14);
    g2->dodajGranu(2, 3, 2.73);
    g2->dodajGranu(3, 5, 1);
    g2->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g2, bfs_obilazak, g2->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
  delete g2;

    return 0;
}
