#include <cstdio>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <queue>

using namespace std;

char temp[10000];

class triple{
public:
    string first,second,third;
    friend bool operator <(triple A, triple B){
        if( A.first == B.first ){
            if( A.second == B.second ){
                if( A.third == B.third ){
                    return false;
                }
                return A.third < B.third;
            }
            return A.second < B.second;
        }
        return A.first < B.first;
    }
};

vector<string> moguca_stanja,ulazna_abeceda, prihvatljiva_stanja, pocetna_stanja;
map< triple, set< pair<string, string> > > prijelazi;
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

        int pr = substr_l.find(","), dr = substr_l.find(",", pr+1);
        string ime_stanja = substr_l.substr( 0, pr );
        string ulazni_znak = substr_l.substr( pr+1, 1 );
        string vrh_stoga = substr_l.substr( dr+1 );

        // printf("%s %s %s\n", ime_stanja.c_str(), ulazni_znak.c_str(), vrh_stoga.c_str() );

        s = substr_r;
        s += "#";
        while( s.find("#") != string::npos ){
            string sub_str = s.substr( 0, s.find("#") );
            // printf("%s\n",sub_str.c_str());
            s = s.substr( s.find("#") + 1 );

            string lijevo = sub_str.substr(0, sub_str.find(",") );
            string desno = sub_str.substr( sub_str.find(",") + 1 );
            triple tr_temp;
            tr_temp.first = ime_stanja;
            tr_temp.second = ulazni_znak;
            tr_temp.third = vrh_stoga;

            prijelazi[tr_temp].insert( make_pair( lijevo, desno ) );
        }        
    }
}

bool navali(vector<string> stanje, string niz, stack<char> stog){
    if( !stog.empty() ){
        //printf("niz: %s\nvrh stoga: %c\n",niz.c_str(), stog.top());
    }else{
        //printf("niz: %s\nstog je prazan!\n",niz.c_str());
    }
    for(int i = 0;i  < (int)stanje.size(); ++i){
        //printf("stanje: %s\n", stanje[i].c_str());
    }
    if( niz.length() == 0 ){
        for(int i = 0; i < (int)stanje.size(); ++i){
            for(int j = 0; j < (int)prihvatljiva_stanja.size(); ++j){
                if( stanje[i] == prihvatljiva_stanja[j] ) return true;
            }
        }
    }
    vector<string> trenutna_stanja = stanje;
    set<string> iduca_stanja;
    char vrh_stoga;
    if( !stog.empty() ){
        vrh_stoga = stog.top();
        stog.pop();
    }else{
        vrh_stoga = '$';
    }
    //    printf("%d\n",(int)trenutna_stanja.size());
    for(int j = 0; j < (int)trenutna_stanja.size(); ++j){
        triple tr_temp;
        tr_temp.first = trenutna_stanja[j];
        tr_temp.second = niz[0];
        tr_temp.third = vrh_stoga;
        if( prijelazi.count(tr_temp) > 0 ){
            set< pair<string,string> >::iterator it;
            vector<string> temp_stanja;
            // printf("%d\n", (int)prijelazi[tr_temp].size());
            for ( it = prijelazi[tr_temp].begin() ; it != prijelazi[tr_temp].end(); ++it ){
                //printf("za stanje %s sam pronasao prijelaz u %s,%s\n", trenutna_stanja[j].c_str(), it->first.c_str(), it->second.c_str() );
                vector< string > iduca_stanja;

                if( it->second.length() > 1 || it->second[0] != '$' ){
                    for(int i = it->second.length()-1; i >= 0; --i)
                        stog.push( it->second[i] );
                }
                
                iduca_stanja.push_back( it->first );

                map<string, bool> bio;
                queue<string> Q;
                
                for(int i = 0;i<(int)iduca_stanja.size();++i){
                    Q.push( iduca_stanja[i] );
                    bio[iduca_stanja[i]] = true;
                }
                //printf("bok3\n");
                while(!Q.empty()){
                    string t = Q.front();
                    Q.pop();
                    triple tr_temp;
                    tr_temp.first = t;
                    tr_temp.second = "$";
                    if( stog.empty() ) continue;
                    tr_temp.third = stog.top();
                    if( prijelazi.count( tr_temp ) > 0){
                        set< pair<string, string> >::iterator it2;
                        for ( it2 = prijelazi[tr_temp].begin() ; it2 != prijelazi[tr_temp].end(); ++it2 ){
                            if( bio.count(it2->first) == 0 ){
                                bio[it2->first] = true;
                                Q.push(it2->first);
                                iduca_stanja.push_back(it2->first);
                            }
                        }   
                    }
                }
                    //                    printf("%s\n", niz.substr(1).c_str());
                if( navali( iduca_stanja, niz.substr(1), stog ) == true ){
                    return true;
                }
                if( it->second.length() > 1 || it->second[0] != '$' ){
                    for(int i = it->second.length()-1; i >= 0 && !stog.empty(); --i){
                        stog.pop();
                    }
                }
            }
        }
    }
    return false;
}


int utrlab2(char* definicijaPA, char* ulazniNiz) {
    string tempstr = definicijaPA;
    if( tempstr[0] == '%' ) tempstr = tempstr.substr(1);
    
    s = tempstr.substr(0, tempstr.find("%") );
    ucit( &moguca_stanja );
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    s = tempstr.substr(0, tempstr.find("%") ); // ulazna abeceda, ne treba nam
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    s = tempstr.substr(0, tempstr.find("%") ); // slova u stogu, ne treba nam
    tempstr = tempstr.substr( tempstr.find("%") + 1 );

    s = tempstr.substr(0, tempstr.find("%") );
    ucit( &pocetna_stanja );
    tempstr = tempstr.substr( tempstr.find("%") + 1 );

    s = tempstr.substr(0, tempstr.find("%") ); // pocetno stanje stoga...
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    int l = s.length();
    for(int i = 0; i < l; ++i){
        st.push( s[i] );
    }
    s = tempstr.substr(0, tempstr.find("%") );
    ucit( &prihvatljiva_stanja );
    tempstr = tempstr.substr( tempstr.find("%") + 1 );
    
    ucit_prijelaze(tempstr);

    //dodaj pocetna stanja jos iz $-prijelaza
    map<string, bool> bio;
    queue<string> Q;

    for(int i = 0;i<(int)pocetna_stanja.size();++i){
        Q.push( pocetna_stanja[i] );
        bio[pocetna_stanja[i]] = true;
    }

    while(!Q.empty()){
        string t = Q.front();
        Q.pop();
        triple tr_temp;
        tr_temp.first = t;
        tr_temp.second = "$";
        tr_temp.third = st.top();
        //printf("%s\n",tr_temp.third.c_str());
        if( prijelazi.count( tr_temp ) > 0){
            set< pair<string, string> >::iterator it;
            for ( it = prijelazi[tr_temp].begin() ; it != prijelazi[tr_temp].end(); ++it ){
                if( bio.count(it->first) == 0 ){
                    bio[it->first] = true;
                    Q.push(it->first);
                    pocetna_stanja.push_back(it->first);
                }
            }   
        }
    }

    bool k = navali(pocetna_stanja, (string)ulazniNiz, st);
    
    return k;
}


/*

int main(int argc, char **argv){

    char defi[] = "%q1,q2,q3%0,1,2%N,J,K%q1%K%q3%q1,0,K->q1,NK%q1,1,K->q1,JK%q1,0,N->q1,NN%q1,1,N->q1,JN%q1,0,J->q1,NJ%q1,1,J->q1,JJ%q1,2,K->q2,K%q1,2,N->q2,N%q1,2,J->q2,J%q2,0,N->q2,$%q2,1,J->q2,$%q2,$,K->q3,$%";
    char niz[] = "10010201001";

    printf("%d\n", utrlab2(defi, niz));
}
*/
