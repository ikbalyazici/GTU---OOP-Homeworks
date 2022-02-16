//HEADER FILES
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

//CONSTANT VALUES: NUMBER OF EVERY TYPES FIELDS
const int BOOK_FIELD = 4;
const int MUSIC_FIELD = 4;
const int MOVIE_FIELD = 5;

//VECTOR CLASS FOR STORING ANY TYPE (book, music or movie) OF INPUT
template <typename T>
class database
{
public:
	void add(T* a); //Mutator: Adds element to vector
	T* show(int i); //Accesor: Returns vectors ith member
	int size(); //Returns size of the vector
private:
	vector<T*> datav; 
};

template <typename T>
void database<T>::add(T* a)	{ datav.push_back(a); } //Mutator: Adding element to vector

template <typename T>
T* database<T>::show(int i)	{ return datav[i]; } //Accesor: Returning necessary element of vector

template <typename T>
int database<T>::size()	{ return datav.size(); } //Returning size of the vector

//BASE CLASS OF 3 DIFFERENT CATALOG TYPE
class catalogType{
public:
	string getfl(); //Accesor
	void setfl(string s); //Mutator
	string gettitle(); //Accesor
	void settitle(string s); //Mutator
	string getyear(); //Accesor
	void setyear (string s); //Mutator
protected:
	string fl; //It stores full line info without parsing
	string title;
	string year;
};

class book : public catalogType {
public:
	string getauthors(); //Accesor
	void setauthors(string s); //Mutator
	string gettags(); //Accesor
	void settags(string s); //Mutator
private:
	string authors;
	string tags;
};

class music : public catalogType {
public:
	string getartists(); //Accesor
	void setartists(string s); //Mutator
	string getgenre(); //Accesor
	void setgenre (string s); //Mutator
private:
	string artists;
	string genre;
};

class movie : public catalogType {
public:
	string getdirector(); //Accesor
	void setdirector(string s); //Mutator
	string getgenre(); //Accesor
	void setgenre(string s); //Mutator
	string getstarring(); //Accesor
	void setstarring(string s); //Mutator
private:
	string director;
	string genre;
	string starring;
};

//MUTATOR FUNCTIONS
void catalogType::setfl    (string s)	{	fl = s;	  	  }
void catalogType::settitle (string s)	{	title = s;	  }
void catalogType::setyear  (string s)	{	year = s;	  }
void book::setauthors      (string s)	{	authors = s;  }
void book::settags         (string s)	{	tags = s;	  }
void music::setartists     (string s)	{	artists = s;  }
void music::setgenre       (string s)	{	genre = s;	  }
void movie::setdirector    (string s)	{	director = s; }
void movie::setgenre       (string s)	{	genre = s;	  }
void movie::setstarring    (string s)	{	starring = s; }

//ACCESOR FUNCTIONS
string catalogType::getfl    () { return fl;       }
string catalogType::gettitle ()	{ return title;    }
string catalogType::getyear	 ()	{ return year;     }
string book::getauthors		 ()	{ return authors;  }
string book::gettags	 	 ()	{ return tags;     }
string music::getartists	 ()	{ return artists;  } 
string music::getgenre		 ()	{ return genre;    } 
string movie::getdirector	 ()	{ return director; } 
string movie::getgenre		 ()	{ return genre;    } 
string movie::getstarring	 ()	{ return starring; } 

//Parsing function of three different type (all has some small differences from others)
void parse (book* p, string s1, int n);
void parse (music* p, string s1, int n);
void parse (movie* p, string s1, int n);

//Swaps a and b (both of them can be any type)
template <typename X>
void swap (X* a, X* b);

int main(){
	int n,i=0,unq=0; //n: Number of field - i: loop counter - unq: number of unique entries
	string type,s1; //type: stores type of data (book, music or movie) - s1: general string for reading and parsing
	database <book> bookv;  	//Vector of books
	database <music> musicv;	//Vector of musics
	database <movie> moviev;	//Vector of movies

	ifstream catalog ("data.txt");  //Accesing input file 
	ofstream out ("output.txt");    //Accesing output file
	ifstream com ("commands.txt");  //Accesing another input file

	if(out.is_open()){  	
		if(catalog.is_open()){
			getline(catalog,type); //Learning the type 
			out << "Catalog Read: " << type << endl;

			while(catalog){
				getline(catalog,s1); //taking one full line
				if(s1=="") break;

				if(type=="book"){ //Program runs depending on the type. Every type has its own specialities
					n=BOOK_FIELD; //Determining field number of type
					book* b = new book; //Creating new empty book object
					
					try{
						if(count(s1.begin(),s1.end(),'"') != n*2) //controlling field number of input by counitng "s
							throw 1;
						parse (b,s1,n); //Parsing the line to its categories
						for(i=0;i<bookv.size();i++){
							if(bookv.show(i)->gettitle()== b->gettitle()){ //duplicate entry 
								delete b;
								throw false;
							}
						}
						bookv.add(b); //unique entry stores in vector
						unq++;	//counting number of unique entries
					}
					catch(int e){
						out << "Exception: missing field" << endl << s1 << endl;
					}
				}
				else if (type=="music") //Program runs depending on the type. Every type has its own specialities
				{
					n=MUSIC_FIELD; //Determining field number of type
					music* m = new music; //Creating new empty music object
					
					try{
						if(count(s1.begin(),s1.end(),'"') != n*2) //controlling field number of input by counitng "s
							throw 1;
						parse (m,s1,n); //Parsing the line to its categories
						for(i=0;i<musicv.size();i++){
							if(musicv.show(i)->gettitle()== m->gettitle()){ //duplicate entry 
								delete m;
								throw false;
							}
						}
						musicv.add(m); //unique entry stores in vector
						unq++;	//counting number of unique entries
					}
					catch(int e){
						out << "Exception: missing field" << endl << s1 << endl;
					}
				}
				else if (type=="movie"){ //Program runs depending on the type. Every type has its own specialities
					n=MOVIE_FIELD; //Determining field number of type
					movie* m = new movie; //Creating new empty movie object
					
					try{
						if(count(s1.begin(),s1.end(),'"') != n*2) //controlling field number of input by counitng "s
							throw 1;
						parse (m,s1,n); //Parsing the line to its categories
						for(i=0;i<moviev.size();i++){
							if(moviev.show(i)->gettitle()== m->gettitle()){ //duplicate entry 
								delete m;
								throw false;
							}
						}
						moviev.add(m);  //unique entry stores in vector
						unq++;  //counting number of unique entries
					}
					catch(int e){
						out << "Exception: missing field" << endl << s1 << endl;
					}
					catch(bool e){
						out << "Exception: duplicate entry" << endl << s1 << endl;
					}
				}
			}
			out << unq << " unique entries" << endl;
		}

		else
			exit(1);

		catalog.close(); //All input entries are readed. So we can close this file

		if(com.is_open()){ 
			int j; //loop counter
			string command,str,field,control; 
			//command: search or sort - str: searching string - field: where str will search - control: for controlling command format
			size_t pos1,pos2; //they stores position of given or found string or characters

			while(com){
				getline(com,s1);
				if(s1=="") break;

				try{
					pos1 = s1.find(' ');
					command = s1.substr(0,pos1); //first word (string until first space) is command

					if (command == "sort"){ 
						pos1 = s1.find('"')+1;
						pos2 = s1.find('"',pos1);
						field = s1.substr(pos1,pos2-pos1); //determining the sort wanted field by parsing full line string

						if(count(s1.begin(),s1.end(),' ')!= 1) throw 'e'; //Controlling truth of the command via standarts
						else if(type=="book"){ //Program runs depending on the type. Every type has its own specialities
							
							//Wrong command exception
							if(field != "title" && field != "year" && field != "authors" && field != "tags") throw 'e';

							//SORTING ACCORDING THE FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(bookv.show(j)->gettitle() > bookv.show(j+1)->gettitle())
											swap(bookv.show(j),bookv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << bookv.show(i)->getfl() << endl;
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(bookv.show(j)->getyear() > bookv.show(j+1)->getyear())
											swap(bookv.show(j),bookv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << bookv.show(i)->getfl() << endl;
							}
							else if (field == "authors"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(bookv.show(j)->getauthors() > bookv.show(j+1)->getauthors())
											swap(bookv.show(j),bookv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << bookv.show(i)->getfl() << endl;
							}
							else if (field == "tags"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(bookv.show(j)->gettags() > bookv.show(j+1)->gettags())
											swap(bookv.show(j),bookv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << bookv.show(i)->getfl() << endl;
							}
						}
						else if(type=="music"){ //Program runs depending on the type. Every type has its own specialities
							
							//Wrong command exception
							if(field != "title" && field != "year" && field != "artists" && field != "genre") throw 'e';

							//SORTING ACCORDING THE FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(musicv.show(j)->gettitle() > musicv.show(j+1)->gettitle())
											swap(musicv.show(j),musicv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << musicv.show(i)->getfl() << endl;
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(musicv.show(j)->getyear() > musicv.show(j+1)->getyear())
											swap(musicv.show(j),musicv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << musicv.show(i)->getfl() << endl;
							}
							else if (field == "artists"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(musicv.show(j)->getartists() > musicv.show(j+1)->getartists())
											swap(musicv.show(j),musicv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << musicv.show(i)->getfl() << endl;
							}
							else if (field == "genre"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(musicv.show(j)->getgenre() > musicv.show(j+1)->getgenre())
											swap(musicv.show(j),musicv.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << musicv.show(i)->getfl() << endl;
							}
						}
						else if(type=="movie"){ //Program runs depending on the type. Every type has its own specialities
							
							//Wrong command exception
							if(field != "title" && field != "year" && field != "director" && field != "genre" && field != "starring") throw 'e';
							
							//SORTING ACCORDING THE FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(moviev.show(j)->gettitle() > moviev.show(j+1)->gettitle())
											swap(moviev.show(j),moviev.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << moviev.show(i)->getfl() << endl;
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(moviev.show(j)->getyear() > moviev.show(j+1)->getyear())
											swap(moviev.show(j),moviev.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << moviev.show(i)->getfl() << endl;
							}
							else if (field == "director"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(moviev.show(j)->getdirector() > moviev.show(j+1)->getdirector())
											swap(moviev.show(j),moviev.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << moviev.show(i)->getfl() << endl;
							}
							else if (field == "genre"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(moviev.show(j)->getgenre() > moviev.show(j+1)->getgenre())
											swap(moviev.show(j),moviev.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << moviev.show(i)->getfl() << endl;
							}
							else if (field == "starring"){
								out << s1 << endl;
								for(i=0; i<unq-1; i++){
									for(j=0; j<unq-i-1; j++){
										if(moviev.show(j)->getstarring() > moviev.show(j+1)->getstarring())
											swap(moviev.show(j),moviev.show(j+1));
									}
								}
								for(i=0;i<unq;i++)
									out << moviev.show(i)->getfl() << endl;
							}
						}
					}
					else if (command == "search"){
						pos1 = s1.find('"')+1;
						pos2 = s1.find('"',pos1);
						str = s1.substr(pos1,pos2-pos1); //Determining the input will search in a field
						pos1 = s1.find('"',pos2+1)+1;
						control = s1.substr(pos2+1,pos1-pos2-2); //Controlling commands format
						
						if(control != " in ") throw 'e'; //Command is in wrong format

						pos2 = s1.find('"',pos1);
						if(pos2+1 != s1.size()) throw 'e'; //Command is wrong format (includes extra things)
						field = s1.substr(pos1,pos2-pos1); //Determining the field is str will search inside
						
						if(type=="book"){ //Program runs depending on the type. Every type has its own specialities
							//Wrong command exception
							if(field != "title" && field != "year" && field != "authors" && field != "tags") throw 'e';
							//SEARCING IN DIFFERENT FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(bookv.show(i)->gettitle().find(str) != string::npos){
										out << bookv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(bookv.show(i)->getyear().find(str) != string::npos){
										out << bookv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "authors"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(bookv.show(i)->getauthors().find(str) != string::npos){
										out << bookv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "tags"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(bookv.show(i)->gettags().find(str) != string::npos){
										out << bookv.show(i)->getfl() << endl;
										break;
									}
								}
							}
						}
						else if(type=="music"){ //Program runs depending on the type. Every type has its own specialities
							//Wrong command exception
							if(field != "title" && field != "year" && field != "artists" && field != "genre") throw 'e';
							//SEARCING IN DIFFERENT FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(musicv.show(i)->gettitle().find(str) != string::npos){
										out << musicv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(musicv.show(i)->getyear().find(str) != string::npos){
										out << musicv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "artists"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(musicv.show(i)->getartists().find(str) != string::npos){
										out << musicv.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "genre"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(musicv.show(i)->getgenre().find(str) != string::npos){
										out << musicv.show(i)->getfl() << endl;
										break;
									}
								}
							}
						}
						else if(type=="movie"){ //Program runs depending on the type. Every type has its own specialities
							//Wrong command exception
							if(field != "title" && field != "year" && field != "director" && field != "genre" && field != "starring") throw 'e';
							//SEARCING IN DIFFERENT FIELDS
							else if (field == "title"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(moviev.show(i)->gettitle().find(str) != string::npos){
										out << moviev.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "year"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(moviev.show(i)->getyear().find(str) != string::npos){
										out << moviev.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "director"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(moviev.show(i)->getdirector().find(str) != string::npos){
										out << moviev.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "genre"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(moviev.show(i)->getgenre().find(str) != string::npos){
										out << moviev.show(i)->getfl() << endl;
										break;
									}
								}
							}
							else if (field == "starring"){
								out << s1 << endl;
								for(i=0;i<unq;i++){
									if(moviev.show(i)->getstarring().find(str) != string::npos){
										out << moviev.show(i)->getfl() << endl;
										break;
									}
								}
							}
						}
					}
					else throw 'e';	
				}
				catch(char e){
					out << "Exception: command is wrong" << endl << s1 << endl;
				}			
			}
		}
		else 
			exit(1);
		com.close();
	}
	out.close();
	return 0;
}

//PARSING FUNCTIONS
//Algorithm:
//save full line
//find opening "
//find closing "
//save the word between "s necessary field of object
//erase saved field of full line string
void parse (book* p, string s1, int n){
	size_t pos1,pos2,found;
	string s2;
	int i;

	p->setfl(s1);
	for(i=0;i<n;i++){
		found = s1.find('"');
		pos1 = found;
		found = s1.find('"',found+1);
		pos2 = found;
		s2 = s1.substr(pos1,pos2+1);

		if(i==0) p->settitle(s2);
		else if(i==1) p->setauthors(s2);
		else if(i==2) p->setyear(s2);
		else if(i==3) p->settags(s2);
		s1.erase(0,found+1);
	}
}

void parse (music* p, string s1, int n){
	size_t pos1,pos2,found;
	string s2;
	int i;
	
	p->setfl(s1);
	for(i=0;i<n;i++){
		found = s1.find('"');
		pos1 = found;
		found = s1.find('"',found+1);
		pos2 = found;
		s2 = s1.substr(pos1,pos2+1);

		if(i==0) p->settitle(s2);
		else if(i==1) p->setartists(s2);
		else if(i==2) p->setyear(s2);
		else if(i==3) p->setgenre(s2);
		s1.erase(0,found+1);
	}
}

void parse (movie* p, string s1, int n){
	size_t pos1,pos2,found;
	string s2;
	int i;
	
	p->setfl(s1);
	for(i=0;i<n;i++){
		found = s1.find('"');
		pos1 = found;
		found = s1.find('"',found+1);
		pos2 = found;
		s2 = s1.substr(pos1,pos2+1);
		if(i==0)	  p->settitle(s2);
		else if(i==1) p->setdirector(s2);
		else if(i==2) p->setyear(s2);
		else if(i==3) p->setgenre(s2);
		else if(i==4) p->setstarring(s2);
		s1.erase(0,found+1);
	}
}

template <typename X>
void swap (X* a, X* b){
	X temp = *a;
	*a = *b;
	*b = temp;
}