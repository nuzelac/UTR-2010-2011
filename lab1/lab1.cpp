#include <cstdio>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <queue>

using namespace std;

vector<string> moguca_stanja,ulazna_abeceda, prihvatljiva_stanja, pocetna_stanja;
map< pair<string, string>, set<string> > prijelazi;
string s;

char temp[10000];

void ucit(vector<string> *v){
    scanf("%s",temp);
    s = temp;
    s += ",";
    while( s.find(",") != string::npos ){
        string sub_str = s.substr( 0, s.find(",") );
        if( sub_str.length() > 0 )
            (*v).push_back(sub_str);
        s = s.substr( s.find(",")+1 );
    }
}

void ucit_prijelaze(){
   while( scanf("%s",temp) == 1 ){
        s = temp;
        string substr_l = s.substr( 0, s.find("->") );
        string substr_r = s.substr( s.find("->") + 2 );

        string ime_stanja = substr_l.substr( 0, substr_l.find(",") );
        string ulazni_znak = substr_l.substr( substr_l.find(",") + 1 );

        s = substr_r;
        s += ",";
        while( s.find(",") != string::npos ){
            string sub_str = s.substr( 0, s.find(",") );
            if( sub_str.length() > 0 )
                prijelazi[make_pair(ime_stanja,ulazni_znak)].insert(sub_str);
            s = s.substr( s.find(",")+1 );
        }
    }
}

int main(int argc, char **argv){
    freopen(argv[1],"r",stdin);
    string s;
    
    ucit( &moguca_stanja );
    ucit( &ulazna_abeceda );
    ucit( &prihvatljiva_stanja );
    ucit( &pocetna_stanja );

    ucit_prijelaze();
 
    //dodaj pocetna stanja jos iz $-prijelaza  
    map<string, bool> bio;
    queue<string> Q;

    for(int i = 0;i<(int)pocetna_stanja.size();++i){
        Q.push( pocetna_stanja[i] );
        bio[pocetna_stanja[i]] = true;
    }

    string pocetni_epsilon;

    while(!Q.empty()){
        string t = Q.front();
        Q.pop();
        if( prijelazi.count(make_pair(t,"$")) > 0){
            set<string>::iterator it;
            for ( it = prijelazi[make_pair(t,"$")].begin() ; it != prijelazi[make_pair(t,"$")].end(); ++it ){
                if( bio.count(*it) == 0 ){
                    bio[*it] = true;
                    Q.push(*it);
                    pocetni_epsilon += t + ",$->" + (*it) + "\n";
                    //printf("%s,$->%s\n",t.c_str(),(*it).c_str());
                    pocetna_stanja.push_back(*it);
                }
            }   
        }
    }

    /*printf("pocetna stanja nakon dodavanja $-prijelaza:\n");
    for(int i = 0;i<(int)pocetna_stanja.size();++i){
        printf("%s\n",pocetna_stanja[i].c_str());
    }*/
    
    freopen(argv[2],"r",stdin);

    while( scanf("%s",temp) == 1 ){

        int l = strlen(temp);
        s = temp;
        printf("%s",pocetni_epsilon.c_str());
        vector<string> trenutna_stanja = pocetna_stanja;
        for(int i = 0;i < l; ++i){
            set<string> iduca_stanja;
            for(int j = 0; j < (int)trenutna_stanja.size(); ++j){
                if( prijelazi.count(make_pair(trenutna_stanja[j], s.substr(i,1))) > 0 ){
                    set<string>::iterator it;
                    vector<string> temp_stanja;
                    for ( it = prijelazi[make_pair(trenutna_stanja[j],s.substr(i,1))].begin() ; it != prijelazi[make_pair(trenutna_stanja[j],s.substr(i,1))].end(); ++it ){
                        printf("%s,%s->%s\n",trenutna_stanja[j].c_str(), (s.substr(i,1)).c_str(), (*it).c_str() );
                        temp_stanja.push_back(*it);
                        
                    }
                    map<string, bool> bio;
                    queue<string> Q;
                    
                    for(int i = 0;i<(int)temp_stanja.size();++i){
                        Q.push( temp_stanja[i] );
                        bio[temp_stanja[i]] = true;
                    }
                    
                    while(!Q.empty()){
                        string t = Q.front();
                        Q.pop();
                        if( prijelazi.count(make_pair(t,"$")) > 0){
                            set<string>::iterator it;
                            for ( it = prijelazi[make_pair(t,"$")].begin() ; it != prijelazi[make_pair(t,"$")].end(); ++it ){
                                if( bio.count(*it) == 0 ){
                                    bio[*it] = true;
                                    Q.push(*it);
                                    printf("%s,$->%s\n",t.c_str(), (*it).c_str());
                                    temp_stanja.push_back(*it);
                                }
                            }   
                        }
                    }

                    for(int i = 0;i<(int)temp_stanja.size();++i){
                        iduca_stanja.insert(temp_stanja[i]);
                    }
                }
            }
            set<string>::iterator it;
            trenutna_stanja.clear();
            for ( it = iduca_stanja.begin() ; it != iduca_stanja.end(); ++it ){
                trenutna_stanja.push_back(*it);
            }
        }
        bool ok = false;
        for(int i = 0; i < (int)trenutna_stanja.size(); ++i){
            for(int j = 0; j < (int)prihvatljiva_stanja.size();++j){
                if( trenutna_stanja[i] == prihvatljiva_stanja[j] ) ok = true;
            }
            if( ok ) break;
        }

        if(ok) printf("Prihvaca se ulazni niz %s\n",temp); else printf("Ne prihvaca se ulazni niz %s\n",temp);
    }

    
    return 0;
}
