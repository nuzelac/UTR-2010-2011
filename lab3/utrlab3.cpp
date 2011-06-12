#include <cstdio>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <queue>

using namespace std;

char temp[10000], traka[10000], sol[1000];

class triple{
public:
    string first;
    char second, third;
};

vector<string> moguca_stanja,ulazna_abeceda, prihvatljiva_stanja, pocetna_stanja;
map< pair<string, char> , triple > prijelazi;
stack< char > st;
string s;

void ucit(vector<string> *v){
    s += ",";
    while( s.find(",") != string::npos ){
        string sub_str = s.substr( 0, s.find(",") );
        if( sub_str.length() > 0 )
            (*v).push_back(sub_str);
        s = s.substr( s.find(",")+1 );
    }
}

void ucit_prijelaze(string t){
    while( t.length() > 0 ){
        s = t.substr(0, t.find("%") );
        t = t.substr( t.find("%") + 1 );
        
        string substr_l = s.substr( 0, s.find("->") );
        string substr_r = s.substr( s.find("->") + 2 );

        int pr = substr_l.find(","), dr;
        string ime_stanja = substr_l.substr( 0, pr );
        string bleh = substr_l.substr(pr+1, 1);
        char ulazni_znak = bleh[0];

        pr = substr_r.find(","), dr = substr_r.find(",", pr+1);
        string stanje = substr_r.substr( 0, pr );
        string zapisi = substr_r.substr( pr+1, 1 );
        string pomak = substr_r.substr( dr+1 );

        triple temp;
        temp.first = stanje;
        temp.second = zapisi[0];
        temp.third = pomak[0];
        
        prijelazi[ make_pair( ime_stanja, ulazni_znak) ] = temp;
    }
}


char* utrlab3(char* definicijaPA, char* ulazniNiz) {
    string tempstr = definicijaPA, prazno, stanje;
    int offset = 0, cnt = 0;
    if( tempstr[0] == '%' ) tempstr = tempstr.substr(1);
    
    s = tempstr.substr(0, tempstr.find("%") );
    ucit( &moguca_stanja );
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    s = tempstr.substr(0, tempstr.find("%") ); // ulazna abeceda, ne treba nam
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    s = tempstr.substr(0, tempstr.find("%") ); // oznaka za praznu celiju, ne treba nam
    prazno = s;
    tempstr = tempstr.substr( tempstr.find("%") + 1 );

    s = tempstr.substr(0, tempstr.find("%") );
    stanje = s;
    tempstr = tempstr.substr( tempstr.find("%") + 1 );

    s = tempstr.substr(0, tempstr.find("%") );
    ucit( &prihvatljiva_stanja );
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    ucit_prijelaze(tempstr);

    int l = strlen(ulazniNiz);
    for(int i = 0;i < l;++i) traka[i] = ulazniNiz[i];
    for(int i = l; i < 10000; ++i ) traka[i] = prazno[0];
    
    bool ok = false;

    while(1){
        if( offset < 0 || ok) break;
        //printf("%s %c\n",stanje.c_str(), traka[offset]);
        if( prijelazi.count(make_pair( stanje, traka[offset] )) > 0 ){
            triple temp = prijelazi[make_pair(stanje, traka[offset])];
            stanje = temp.first;
            traka[offset] = temp.second;
            if( temp.third == 'R' ) ++offset; else --offset;
            ++cnt;            
        }else{
            for(int i = 0; i < (int)prihvatljiva_stanja.size(); ++i ){
                if( prihvatljiva_stanja[i] == stanje ){
                    ok = true;
                    break;
                }
            }
            if(!ok) break;
        }
    }
    sprintf(sol, "%d%%%d", cnt, (ok) ? 1 : 0);
    return sol;
}
/*
int main(int argc, char **argv){
    //char defi[] = "%q0,q1,q2,q3,q4%0,1,X,Y,B%B%q0%q4%q0,0->q1,X,R%q1,0->q1,0,R%q1,Y->q1,Y,R%q1,1->q2,Y,L%q2,0->q2,0,L%q2,Y->q2,Y,L%q2,X->q0,X,R%q0,Y->q3,Y,R%q3,Y->q3,Y,R%q3,B->q4,B,R%";
    //char niz[] = "000111";
    char definicijaTS[] = "%q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,qf%a,b,c,d,A,B,C,D,#,E%E%q0%qf%q0,a->q1,A,R%q0,B->q5,B,L%q0,#->q0,#,R%q1,a->q1,a,R%q1,b->q1,b,R%q1,E->q4,E,L%q1,B->q1,B,R%q1,d->q4,d,L%q1,c->q2,C,L%q1,C->q1,C,R%q2,b->q2,b,L%q2,C->q2,C,L%q2,a->q3,a,R%q2,A->q3,A,R%q2,B->q3,B,R%q3,b->q1,B,R%q4,C->q4,c,L%q4,b->q4,b,L%q4,B->q4,B,L%q4,a->q4,a,L%q4,A->q0,A,R%q5,A->q5,a,L%q5,#->q6,#,R%q6,a->q7,A,R%q6,B->q9,B,R%q7,a->q7,a,R%q7,B->q7,B,R%q7,C->q7,C,R%q7,c->q8,C,L%q8,C->q8,C,L%q8,B->q8,B,L%q8,a->q8,a,L%q8,A->q6,A,R%q9,B->q9,B,R%q9,C->q9,C,R%q9,E->qf,E,L%q9,c->q10,C,R%q9,D->q12,D,R%q10,c->q10,c,R%q10,d->q11,D,L%q10,D->q10,D,R%q11,D->q11,D,L%q11,c->q11,c,L%q11,C->q9,C,R%q12,D->q12,D,R%q12,E->qf,E,L%";
    char sadrzajTrake[] = "#aaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbabecedabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccdddddddddddddddddddd";

    printf("%s\n", utrlab3(definicijaTS, sadrzajTrake));
}

*/
