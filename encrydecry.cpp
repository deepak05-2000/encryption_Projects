#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<Windows.h>
using namespace std;

struct del
{
   int key;
};
string& enc(string& s,struct del& d)
{
    string key=to_string(d.key);
    int a=0; int c;
    for(int i=0;i<s.length();i++)
    {
        c=(int)s[i]+key[a++]-'0';
        if(c>126) c=31+c%126;
       s[i]=(char)c; 
       if(a>=key.length()) a=0;
    }
    return s;
}
string& dec(string& s,del& d)
{
    string key=to_string(d.key);
    int a=0; int c;
    for(int i=0;i<s.length();i++)
    {
       c=(int)s[i]-(key[a++]-'0');
       if(c<32) c=127-(32%c);
       s[i]=(char)c;
       if(a>=key.length()) a=0;
    }
    //s.resize(s.size()-1);
    return s;
}
void progress()
{
    
   system("color 0C");      
   for (int i = 0; i < 100; i++) {      
       cout<<"\r"<<(i+1)<<"%";
      Sleep(100);   
      fflush(stdout);
       }
}
void enc_file(char *s)
{ 
    struct del d;
    cout<<"enter key(digits):";
    cin>>d.key;
    fstream f(s,ios::in|ios::binary);
    if(f.fail()) cerr<<"source file error";
    string n=s;
    n=n.substr(n.find_last_of('\\')+1);
    n+=".encrypt";
    fstream o(n,ios::out|ios::binary);
    if(o.fail()) cerr<<"\neroor";
    o.write((char*)&d,sizeof(del));
    string str;
    cout<<setw(20)<<"PLEASE WAIT...."<<endl;
    cout<<setw(20);
    progress();
    system("color 0A");
    while(getline(f,str))
    { 
        str=enc(str,d);
        o<<str;
        o<<"\n";
    }
    o.close();
    f.close();
    remove(s);
    cout<<"\n"<<setw(20)<<"FILE ENCRYPTED";
}
void dec_file(char *s,del& d)
{
    int key;
   cout<<"enter key to decrypt...:";
   cin>>key;
   fstream r(s,ios::in|ios::binary);
   if(r.fail()) cout<<"\nfile can't be read\n";
   r.read((char*)&d,sizeof(del));
   if(key!=d.key)
   {
       cout<<setw(20)<<"WRONG KEY";
        return ;
   }
   else {
    string n(s);
    n=n.erase(n.find_last_of('.'));
    fstream o(n,ios::out|ios::binary);
    string str;
    cout<<setw(20)<<"PLEASE WAIT...."<<endl;
    cout<<setw(20);
    progress();
    system("color 0A");
    while(getline(r,str))
    {
        o.clear();
        str=dec(str,d);
        o<<str;
        o<<"\n";
    }
     o.close();
   }
   r.close();
   remove(s);
   cout<<"\n"<<setw(20)<<"FILE DECRYPTED";
}
int main(int argc,char* argv[])
{  
    system("color 0A");
    string s;
    struct del d;
    string arg=argv[2];
    if(arg=="-e") enc_file(argv[1]);
    struct del read;
    if(arg=="-d") dec_file(argv[1],read);
    return 0;
}